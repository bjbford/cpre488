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


camera_config_t camera_config;

// Main function. Initializes the devices and configures VDMA
int main() {

	camera_config_init(&camera_config);
	fmc_imageon_enable(&camera_config);
	camera_loop(&camera_config);

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
//    config->uDeviceId_RGBYCC = XPAR_RGB2YCRCB_0_DEVICE_ID;

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


	// Run for 1000 frames before going back to HW mode
	for (j = 0; j < 1000; j++) {
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
			Cb = (Cb + lastCb) / 2;
			Cr = (Cr + lastCr) / 2;
			
			//0xCrY0CbY1
			if((i % 2) == 1){
				//xil_printf("Cr: %x  Y0: %x  Cb: %x  Y1: %x\r\n", Cr, Y0, Cb, Y1);
				uint16_t final1 = (((uint16_t)(Cb) << 8) | ((uint16_t)(Y0)));
				uint16_t final2 = (((uint16_t)(Cr) << 8) | ((uint16_t)(Y1)));
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