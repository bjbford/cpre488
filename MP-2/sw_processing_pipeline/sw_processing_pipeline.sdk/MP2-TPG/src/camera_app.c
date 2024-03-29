/*****************************************************************************
 * Joseph Zambreno
 * Phillip Jones
 *
 * Department of Electrical and Computer Engineering
 * Iowa State University
 *****************************************************************************/

/*****************************************************************************
 * camera_app.c - main camera application code. The camera configures the various
 * video in and video out peripherals, and (optionally) performs some
 * image processing on data coming in from the vdma.
 *
 *
 * NOTES:
 * 02/04/14 by JAZ::Design created.
 *****************************************************************************/

#include "camera_app.h"
#include "platform.h"


camera_config_t camera_config;

// Main function. Initializes the devices and configures VDMA
int main() {

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);
	camera_loop(&camera_config);
	//while(exit_flag == false)
	//{
	//	camera_interface();
	//}
	// From platform.c : Disables cache.
	//cleanup_platform();
	// Exit command has been given.
	return 0;
}


// Initialize the camera configuration data structure
void camera_config_init(camera_config_t *config) {

    config->uBaseAddr_IIC_FmcIpmi =  XPAR_FMC_IPMI_ID_EEPROM_0_BASEADDR;   // Device for reading HDMI board IPMI EEPROM information
    config->uBaseAddr_IIC_FmcImageon = XPAR_FMC_IMAGEON_IIC_0_BASEADDR;    // Device for configuring the HDMI board

    // Uncomment when using VITA Camera for Video input
    config->uBaseAddr_VITA_SPI = XPAR_ONSEMI_VITA_SPI_0_S00_AXI_BASEADDR;  // Device for configuring the Camera sensor
    config->uBaseAddr_VITA_CAM = XPAR_ONSEMI_VITA_CAM_0_S00_AXI_BASEADDR;  // Device for receiving Camera sensor data

    // Uncomment as Video Hardware pipeline IP cores are added
 	//config->uDeviceId_RGBYCC = XPAR_RGB2YCRCB_0_DEVICE_ID;

    // Uncomment when using the TPG for Video input
//    config->uBaseAddr_TPG_PatternGenerator = XPAR_V_TPG_0_S_AXI_CTRL_BASEADDR; // TPG Device

    config->uDeviceId_VTC_tpg   = XPAR_V_TC_0_DEVICE_ID;                        // Video Timer Controller (VTC) ID
    config->uDeviceId_VDMA_HdmiFrameBuffer = XPAR_AXI_VDMA_0_DEVICE_ID;         // VDMA ID
    config->uBaseAddr_MEM_HdmiFrameBuffer = XPAR_DDR_MEM_BASEADDR + 0x10000000; // VDMA base address for Frame buffers
    config->uNumFrames_HdmiFrameBuffer = XPAR_AXIVDMA_0_NUM_FSTORES;            // NUmber of VDMA Frame buffers

    return;
}

uint8_t to8(Xuint16 data){
	data &= 0x00FF;
	return (uint8_t)(data);
}
const int res = 1920 * 1080;
uint8_t bayer[1920 * 1080];


// Main (SW) processing loop. Recommended to have an explicit exit condition
void camera_loop(camera_config_t *config) {

	Xuint32 parkptr;
	Xuint32 vdma_S2MM_DMACR, vdma_MM2S_DMACR;
	int i, j;


	xil_printf("Entering main SW processing loop\r\n");


	// Grab the DMA parkptr, and update it to ensure that when parked, the S2MM side is on frame 0, and the MM2S side on frame 1
	parkptr = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET);
	parkptr &= ~XAXIVDMA_PARKPTR_READREF_MASK;
	parkptr &= ~XAXIVDMA_PARKPTR_WRTREF_MASK;
	parkptr |= 0x1;
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET, parkptr);


	// Grab the DMA Control Registers, and clear circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);


	// Pointers to the S2MM memory frame and M2SS memory frame
	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);


	xil_printf("Start processing 1000 frames!\r\n");
	xil_printf("pS2MM_Mem = %X\n\r", pS2MM_Mem);
	xil_printf("pMM2S_Mem = %X\n\r", pMM2S_Mem);
	
	uint8_t red = 0, green = 0, blue = 0, lastCb = 0, lastCr = 0, Y0 = 0;
	//0x80DD

	// u64's for FPS computation
	XTime tStart, tEnd;
	// Start time
	XTime_GetTime(&tStart);

	// Run for 1000 frames before going back to HW mode
	for (j = 0; j < 100; j++) {
		for (i = 0; i < res; i++)
			bayer[i] = to8(pS2MM_Mem[i]);
		
		//convert from bayer to normal
		//convert from rgb to yuv
		//convert from 4:4:4 to 4:2:2
		for (i = 1; i < res - 1; i++) {
//	       pMM2S_Mem[i] = 0x0A21; // 12-bit with 4-bit padding, 4-bit per color (all green screen)
			//horizontal red/green row
			if(((i / 1920) % 2) == 0){
            
				//green pixel
				if ((i % 2) == 1){
					red = (bayer[i + 1] + (bayer[i - 1])) / 2;
					green = (bayer[i]);
					blue = ((bayer[i + 1920]) + (bayer[i - 1920])) / 2;
				}
				
				//red pixel
				else{
					red = (bayer[i]);
					green = ((bayer[i+ 1]) + (bayer[i - 1]) + (bayer[i + 1920]) + (bayer[i - 1920])) / 4;
					blue = ((bayer[i + 1920 + 1]) + (bayer[i - 1920 - 1]) + (bayer[i + 1920 - 1]) + (bayer[i - 1920 + 1])) / 4;
				}
			}
				 
			//horizontal blue/green row
			else{
				
				//blue pixel
				if ((i % 2) == 1){
					red = ((bayer[i + 1920 + 1]) + (bayer[i - 1920 - 1]) + (bayer[i + 1920 - 1]) + (bayer[i - 1920 + 1])) / 4;
					green = ((bayer[i + 1]) + (bayer[i - 1]) + (bayer[i + 1920]) + (bayer[i - 1920])) / 4;
					blue = (bayer[i]);
				}
				//green pixel
				else{
					red = ((bayer[i + 1]) + (bayer[i - 1])) / 2;
					green = (bayer[i]);
					blue = ((bayer[i + 1920]) + (bayer[i - 1920])) / 2;
				}
			}
			
			//rgb to yuv
//			double conversion[] = {{0.183, 0.614, 0.062}, {-0.101, -0.338, 0.439}, {0.439, -0.399, -0.04}};
			uint8_t Cb = (uint8_t)(128 + (((((int)red) * -101) - (((int)green) * 338) + (((int)blue) * 439)) / 1000));
			uint8_t Cr = (uint8_t)(128 + (((((int)red) * 439) - (((int)green) * 399) - (((int)blue) * 40)) / 1000));
			uint8_t Y1 = (uint8_t)(16 + (((((int)red) * 183) + (((int)green) * 614) + (((int)blue) * 62)) / 1000));
			uint16_t avgCb = ((uint16_t)Cb + (uint16_t)lastCb) / 2;
			uint16_t avgCr = ((uint16_t)Cr + (uint16_t)lastCr) / 2;
			
			//0xCrY0CbY1
			if((i % 2) == 1){
				//xil_printf("Cr: %x  Y0: %x  Cb: %x  Y1: %x\r\n", Cr, Y0, Cb, Y1);
				uint16_t final1 = (((uint16_t)(avgCb) << 8) | ((uint16_t)(Y0)));
				uint16_t final2 = (((uint16_t)(avgCr) << 8) | ((uint16_t)(Y1)));
				//xil_printf("Final value1: %x Final value2: %x\r\n", final1, final2);
				pMM2S_Mem[i - 1] = final1;
				pMM2S_Mem[i] = final2;
			}
			//write final to mem	
			Y0 = Y1;
			lastCb = Cb;
			lastCr = Cr;
//			pMM2S_Mem[i] = pS2MM_Mem[1920*1080-i-1];
		}
		//xil_printf("loop %d\r\n", j);
	}

	// End time
	XTime_GetTime(&tEnd);
	// Compute time: from https://0xstubs.org/measuring-time-in-a-bare-metal-zynq-application/
	// Compute fps
	double fps = 100.0 / (((tEnd - tStart)*COUNTS_PER_SECOND));
	printf("FPS: %lf", fps);

	// Grab the DMA Control Registers, and re-enable circular park mode.
	vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);


	xil_printf("Main SW processing loop complete!\r\n");

	sleep(5);

	// Uncomment when using TPG for Video input
//	fmc_imageon_disable_tpg(config);

	sleep(1);


	return;
}


/**
 * Oversees the capture and playback of captured image frames.
 */
//void camera_interface()
//{
//	// New cycle. Reset needed status flags.
//	button_flag = false;
//	// Checks which buttons or switches are active.
//	check_inputs();
//	// Replays the PPM Frame values to the axi_ppm.
//	replay_mode_handler();
//	if(button_flag == false)
//	{
//		// Reset the counter used for button debouncing.
//		debounce_counter = 0;
//	}
//}
//
//
///**
// * Checks the register values for any active
// * button or switch gpio lines.
// */
//void check_inputs()
//{
//	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
//	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);
//	/************ BUTTON Center ************/
//
//	// Checks for valid BUTTON Center.
//	if(*btn_ptr & BTN_CENTER)
//	{
//		// A button has been pressed.
//			button_flag = true;
//		// The debounce counter has yet to hit the needed cycles.
//		if(debounce_counter < debounce_threshold)
//		{
//			debounce_counter++;
//			// Signals the a button has started the debounce process.
//			debounce_finished = false;
//		}
//		// Enough cycles have passed of constant button press && this button's functions
//		// have yet to be executed for this press.
//		else if((debounce_counter >= debounce_threshold) && (debounce_finished != true))
//		{
//			// Debounce process finished.
//			debounce_finished = true;
//			xil_printf("Center button pressed.");
//			// Capture and store the current image for 2 seconds.
//			xil_printf("Frame Recorded @ index: %d\r\n", frame_index);
//			// Store image into memeory.
//			images[1920*1080*frame_index] = pS2MM_Mem;
//			// Flash image to screen.
//			pS2MM_Mem = images[1920*1080*image_index];
//			// Holds for 2 seconds.
//			sleep(2);
//			// Array boundary detection. Checks if next move will cause out-of-bounds error.
//			if(!((image_index + 1) > MAX_IMAGES_TO_RECORD))
//			{
//				// Increments the frame index.
//				image_index++;
//				// Recorded images have been changed. Reset Replay index.
//				replay_index = 0;
//			}
//		}
//	}
//
//	/************ SWITCH 0 ************/
//
//    // Checks for valid SWITCH 0.
//    if(*sw_ptr & SW_0)
//    {
//    	// Register value 1 detected.
//    	// Switching to Replay Mode.
//    	replay_mode = REPLAY;
//	}
//	else
//	{
//		// Register value 0 detected.
//		// Switching out of replay Mode.
//		replay_mode = NONE;
//	}
//
//	/************ SWITCH 2 ************/
//
//	// Checks for valid SWITCH 2.
//    if(*sw_ptr & SW_2)
//    {
//    	xil_printf("Switch 2 activated");
//		// Exit application flag set.
//		exit_flag = true;
//	}
//
//	/************ SWITCH 7 ************/
//
//	// Checks for valid SWITCH 7.
//	if(*sw_ptr & SW_7)
//	{
//		//reset record array
//		image_index = 0;
//		replay_index = 0;
//		xil_printf("\nReset indexs.\r\n");
//	}
//}
//
//
///**
// * BTN_RIGHT displays the next stored image from memory.
// * BTN_LEFT decrements the play index & plays previous image.
// */
//void replay_mode_handler()
//{
//	volatile Xuint16 *pS2MM_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_S2MM_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET);
//	volatile Xuint16 *pMM2S_Mem = (Xuint16 *)XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_MM2S_ADDR_OFFSET+XAXIVDMA_START_ADDR_OFFSET+4);
//	// Replay Mode active.
//	if(replay_mode == REPLAY)
//	{
//		// Increments the 'replay_index'. Plays the next image.
//		if(*btn_ptr & BTN_RIGHT)
//		{
//			// A button has been pressed.
//			button_flag = true;
//			// The debounce counter has yet to hit the needed cycles.
//			if(debounce_counter < debounce_threshold)
//			{
//				debounce_counter++;
//				// Signals the a button has started the debounce process.
//				debounce_finished = false;
//			}
//			// Enough cycles have passed of constant button press && this button's functions
//			// have yet to be executed for this press.
//			else if((debounce_counter >= debounce_threshold) && (debounce_finished != true))
//			{
//				// Debounce process finished.
//				debounce_finished = true;
//				xil_printf("RIGHT button pressed.");
//
//				// Does not play indexs that hold no data. (Unrecorded)
//				if(pMM2S_Mem[replay_index] == 0)
//				{
//					// Pointers to the S2MM memory frame and M2SS memory frame.
//					xil_printf("Image Replayed @ index: %d\r\n", replay_index);
//					// Play stored array.
//					pMM2S_Mem = images[1920*1080*replay_index];
//					// Check for max images.
//					if(!((replay_index + 1) > MAX_IMAGES_TO_RECORD))
//					{
//						// Array will be inbounds.
//						replay_index++;
//					}
//				}
//			}
//
//		}
//		// Decrement the 'replay_index'. Plays the previous image.
//		if(*btn_ptr & BTN_LEFT)
//		{
//			// A button has been pressed.
//			button_flag = true;
//			// The debounce counter has yet to hit the needed cycles.
//			if(debounce_counter < debounce_threshold)
//			{
//				debounce_counter++;
//				// Signals the a button has started the debounce process.
//				debounce_finished = false;
//			}
//			// Enough cycles have passed of constant button press && this button's functions
//			// have yet to be executed for this press.
//			else if((debounce_counter >= debounce_threshold) && (debounce_finished != true))
//			{
//				// Debounce process finished.
//				debounce_finished = true;
//				xil_printf("LEFT button pressed.");
//				// Array boundary detection. Checks if next move will cause out-of-bounds error.
//				if(!((replay_index - 1) < 0))
//				{
//					// Array will be inbounds.
//					replay_index--;
//					xil_printf("Replay index decremented to: %d\r\n", replay_index);
//					// Display frame @ current index.
//					pMM2S_Mem = images[1920*1080*replay_index];
//				}
//			}
//		}
//	}
//}
//
