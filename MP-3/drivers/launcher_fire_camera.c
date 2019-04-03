#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>


/*-------- CONSTANTS --------*/

#define DEVICE_TREE_BTN 0x41200000
#define DEVICE_TREE_SW 0x41210000
#define DEVICE_TREE_BUFF 0x10000000
#define LAUNCHER_NODE           "/dev/launcher0"
#define LAUNCHER_FIRE           0x10
#define LAUNCHER_STOP           0x20
#define LAUNCHER_UP             0x02
#define LAUNCHER_DOWN           0x01
#define LAUNCHER_LEFT           0x04
#define LAUNCHER_RIGHT          0x08
#define LAUNCHER_UP_LEFT        (LAUNCHER_UP | LAUNCHER_LEFT)
#define LAUNCHER_DOWN_LEFT      (LAUNCHER_DOWN | LAUNCHER_LEFT)
#define LAUNCHER_UP_RIGHT       (LAUNCHER_UP | LAUNCHER_RIGHT)
#define LAUNCHER_DOWN_RIGHT     (LAUNCHER_DOWN | LAUNCHER_RIGHT)

// CAMERA APP
#define XAXIVDMA_PARKPTR_OFFSET 0x00000014
#define XAXIVDMA_TX_OFFSET 0x00000000
#define XAXIVDMA_CR_OFFSET 0x00000000
#define XAXIVDMA_RX_OFFSET 0x00000018
#define XAXIVDMA_CR_TAIL_EN_MASK 0x00000002
#define VDMA_BASE_ADDR 0x43000000

/*-------- METHOD DECLARATIONS --------*/

static void launcher_cmd(int fd, int *cmd);
int get_command(int* btn_ptr);
void find_target(uint16_t image[1080][1920],uint16_t output[1080][1920],int fd, int debug);
void move(uint32_t x,uint32_t y,int fd);
void disable_hardware(uint16_t* vdma_config);
void enable_hardware(uint16_t* vdma_config);

/*-------- MAIN METHOD --------*/

/**
 * Main program application to read input and drive
 * the usb missle launcher.
 */
int main() 
{
    /*-------- LOCAL VARIABLES --------*/
    char c;
    int fd;
    // command to be sent to launcher.
    // Defaults as stop.
    int cmd = LAUNCHER_STOP;
	int prevCmd = 0;
	int stop = LAUNCHER_STOP;
    // Usb address.
    char *dev = LAUNCHER_NODE;
	char lastmode =0;

    // Delay between commands.
    unsigned int duration = 100;
    // Memory maps the Zed Board switches to the the sw_ptr.
    int* sw_ptr = mmap(NULL, // Address of map. Kernal chooses since NULL.
                        1, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDONLY), // Pointer.
                        DEVICE_TREE_SW); // Address of switches in the device tree.
    // Memory maps the Zed Board buttons to the the btn_ptr.
    int* btn_ptr = mmap(NULL, // Address of map. Kernal chooses since NULL.
                        1, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDONLY), // Pointer.
                        DEVICE_TREE_BTN); // Address of buttons in the device tree.
	
    uint16_t* frame_buff = mmap(NULL, // Address of map. Kernal chooses since NULL.
                        (1920*1080*2*2), // Length of map.
                        PROT_READ | PROT_WRITE, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDWR), // Pointer.
                        DEVICE_TREE_BUFF); // Address of buttons in the device tree.
	uint16_t* frame_buff_write = frame_buff + (1920*1080);
	printf("frame_buff virt base addr: %x\r\n", frame_buff);
	printf("frame_buff_write virt base addr: %x\r\n", frame_buff_write);

    uint16_t* vdma_config = mmap(NULL, // Address of map. Kernal chooses since NULL.
                        (0xFFFF), // Length of map.
                        PROT_READ | PROT_WRITE, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDWR), // Pointer.
                        VDMA_BASE_ADDR); // Address of buttons in the device tree.	
	printf("vdma_config virt base addr: %x\r\n", vdma_config);

    /*-------- CONNECT TO LAUNCHER --------*/

    // Attempts to establish connection to the launcher.
    fd = open(dev, O_RDWR);
    // Checks for errors while opening.
    if (fd == -1) 
    {
        // Failure to connect.
        perror("UNABLE TO CONNECT TO LAUNCHER: %m");
        // Exit.
        exit(1);
    }


    /*-------- MAIN PROGRAM LOOP --------*/
    // Pulls the button input until switch 7 is 
    // flipped high. Switch hex value is 0x80.
	// !(*(sw_ptr) & 80)
	printf("sw_ptr value: %x\r\n", *sw_ptr);
    while(!(*(sw_ptr) & 80))
    {
		if(*(sw_ptr) & 1){
			// manual buttons
			/*-------- DETECT BUTTON PRESS --------*/
		    // Checks for valid memory map pointer.
		    if(*btn_ptr)
		    {
		        // Pulls buttons. Gets int value
		        // associated with each button.
		        cmd = get_command(btn_ptr);
		    }
		    // Invalid memory map pointer.
		    else
		    {
		        // Incase of invalid memory map. Default
		        // command to stop.
		        cmd = LAUNCHER_STOP;
		    }
		    /*-------- SEND COMMAND --------*/
			//printf("Received command: %x\n", cmd);
			if(!(cmd == prevCmd && cmd == LAUNCHER_STOP)){
				//printf("Sending new command: %x\n", cmd); 
				launcher_cmd(fd, &cmd);
				usleep(duration * 1000);
				if(cmd != LAUNCHER_STOP)
				{
					launcher_cmd(fd, &stop);
				}
			}
			//usleep(duration * 1000);
			prevCmd = cmd;
		}
		// debug mode (parks image on screen with filtered image)
		else if(*(sw_ptr) & 2){
			if(lastmode != 2){
				printf("Disabling hardware...\r\n");
				disable_hardware(vdma_config);
			}
			lastmode =2;
			find_target(frame_buff,frame_buff_write,fd,1);
		}
		else{
			if(lastmode != 1){
				printf("Starting enable hardware...\r\n");
				enable_hardware(vdma_config);
			}
			lastmode =1;
			find_target(frame_buff,frame_buff_write,fd,0);
			sleep(1);
		}
    }

    /*-------- PROGRAM CLEANUP --------*/

    // Closes connection to the launcher.
    close(fd);
	munmap(sw_ptr, 1);
	munmap(btn_ptr, 1);
	munmap(frame_buff,1080*1920*2*2);
	munmap(vdma_config, 0xFFFF);
    // Exit with success flag.
    return EXIT_SUCCESS;
}


/**
 * Obersves the push buttons & returns the appropriate
 * codes to move the missle launcher.
 */
int get_command(int* btn_ptr)
{
	//printf("Checking for button press.\n");
    // Switch statement to detect and determine the buttons
    // being pressed.
	if(*btn_ptr & 0x01)
		return LAUNCHER_FIRE;
	if(*btn_ptr & 0x02)
        return LAUNCHER_DOWN;
	if(*btn_ptr & 0x04)
        return LAUNCHER_LEFT;
	if(*btn_ptr & 0x08)
        return LAUNCHER_RIGHT;
	if(*btn_ptr & 0x10)
        return LAUNCHER_UP;

	return LAUNCHER_STOP;
}


/**
 * Responsible for transmitting the user's command
 * to the missle launcher.
 */
static void launcher_cmd(int fd, int *cmd) 
{
    // Holds the amount of bytes sent over usb.
    int retval = 0;
    // Sends cmd to missle launcher.
    // Returns # of bytes sent to launcher.
    retval = write(fd, cmd, 1);
	//printf("Bytes sent to launcher: %d\n", retval);
    // Loops until sent bytes is not equal to 1.
    while (retval != 1) 
    {
        // Transmission failed upon sending.
        if (retval < 0) 
        {
            fprintf(stderr, "Could not send command to %s (error %d)\n", LAUNCHER_NODE, retval);
        } 
        // No reply from launcher. (BUSY)
        else if (retval == 0) {
            fprintf(stdout, "Command busy, waiting...\n");
        }
    }
    // Launch command given. Pause for 5 seconds.
    if (*cmd == LAUNCHER_FIRE) 
    {
        usleep(2000000);   
    }
}




void find_target(uint16_t image[1080][1920],uint16_t output[1080][1920],int fd,int debug){
  uint32_t i,j;
  //uint8_t green_pixels[1080][1920/2];
  uint32_t count = 0;
  uint32_t xrow = 0;
  uint32_t yrow = 0;
  uint64_t xsum = 0;
  uint64_t ysum = 0;
	uint32_t		aver  =0;
	uint32_t	aveb = 0;
	uint32_t	avey = 0;

  //FILE *fp = fopen("frame_buff.txt", "w");
 
  for (i = 0; i < 1080; i++) {
    for (j =0; j < 1919; j=j+2) {
      // if brightness is more than 10%,              less than 75%        and the color is red
      if(  ((image[i][j] & 0x00FF) > 0 ) && ((image[i][j] & 0x00FF) < 100 ) && (((image[i][j+1]>>8) & 0x00FF) > 160 ) ){  // find green_pixels      
		//green_pixels[i][j] |= 1;
        // find centroid of green_pixels
			if(debug==1) {//print color
				output[i][j] = image[i][j];
				output[i][j+1] = image[i][j+1];
	  		}
        xrow += j;
        yrow += i;
        count++;
      }
      else{
		if(debug==1) {// print black
			output[i][j] = 0x00FF;//((image[i][j] & 0x00FF) | ( ((image[i][j] & 0xFF00) + (10<<8) > 0xFF00) ? (0xFF00) : ((image[i][j] & 0xFF00) + (10<<8)) ) );//0x8000; //blue offset
			output[i][j+1] = 0x00FF;//((image[i][j+1] & 0x00FF) | ( ((image[i][j+1] & 0xFF00) + (5<<8) > 0xFF00) ? (0xFF00) : ((image[i][j+1] & 0xFF00) + (5<<8)) ) );//0x8000; //red offset
			//aver += ((image[i][j] >> 8) & 0x00FF);
			//aveb += ((image[i][j+1]>> 8) & 0x00FF);
			//avey += (image[i][j] & 0xFF);
			//printf("r=%d,b=%d,y=%d\n\r",((output[i][j] >> 8) & 0x00FF),((output[i][j+1] >> 8) & 0x00FF),(output[i][j] & 0x00FF));
  		}
        //fprintf(fp, "%x, ",image[i][j]);
      }
    }
	xsum += xrow;
	ysum += yrow;
	xrow = 0;
	yrow = 0;
	//fprintf(fp, "\n\r\n\r");
  }
  //printf("r=%d,b=%d,y=%d\n\r",aver/(1920*1080),aveb/(1920*1080),avey/(1920*1080));
  if (count != 0){
  	xrow = xsum/count; // finds centroid
  	yrow = ysum/count;
  	//move(xrow,yrow,fd);
  }
  printf("found %d pixels\n\r",count);
  printf("Coordinates:x=%d,y=%d\n\r",xrow,yrow);
  //fclose(fp);
}


void move(uint32_t x,uint32_t y,int fd ){
	uint16_t xcenter = 1920/2;
	uint16_t ycenter = 500;
	int cmd = 0;
	uint16_t xoff = xcenter - x;
	uint8_t arm =0;

	if (xoff<10){
		xoff = 0 - xoff;
		cmd = LAUNCHER_LEFT;
		launcher_cmd(fd, &cmd);
		usleep(xoff* 2);//gets to x position
		cmd = LAUNCHER_STOP;
		launcher_cmd(fd, &cmd);
	}
	else if(xoff>10){
		cmd = LAUNCHER_RIGHT;
		launcher_cmd(fd, &cmd);
		usleep(xoff* 2);//gets to x position
		cmd = LAUNCHER_STOP;
		launcher_cmd(fd, &cmd);
	}
	else{arm = 1;}
	
	cmd = 0;
	uint16_t yoff = ycenter - y;
	if (yoff<10){
		yoff = 0 - yoff;
		cmd = LAUNCHER_DOWN;
		launcher_cmd(fd, &cmd);
		cmd = LAUNCHER_STOP;
		usleep(yoff* 2);//gets to x position
		launcher_cmd(fd, &cmd);
	}
	else if(yoff>10){
		cmd = LAUNCHER_UP;
		launcher_cmd(fd, &cmd);
		usleep(yoff* 2);//gets to x position
		cmd = LAUNCHER_STOP;
		launcher_cmd(fd, &cmd);
	}
	else if(arm == 2){
		cmd = LAUNCHER_FIRE;
		launcher_cmd(fd, &cmd);
	}
}


/**
 * Halts the hardware pipeline.
 */
void disable_hardware(uint16_t* vdma_config)
{
	uint32_t parkptr;
	uint32_t vdma_S2MM_DMACR, vdma_MM2S_DMACR;
	// Grab the DMA parkptr, and update it to ensure that when parked, the S2MM side is on frame 0, and the MM2S side on frame 1
	//parkptr = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET);
	parkptr = *(vdma_config+XAXIVDMA_PARKPTR_OFFSET);
	parkptr &= ~0x0000001F; //XAXIVDMA_PARKPTR_READREF_MASK
	parkptr &= ~0x00001F00; //XAXIVDMA_PARKPTR_WRTREF_MASK
	parkptr |= 0x1;
	// Address, offset, data
	//XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_PARKPTR_OFFSET, parkptr);
	*(vdma_config+XAXIVDMA_PARKPTR_OFFSET) = parkptr;
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_PARKPTR_OFFSET), parkptr);
	// Grab the DMA Control Registers, and clear circular park mode.
	//vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	vdma_MM2S_DMACR = *(vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_MM2S_DMACR);
	// Address, offset, data
	//XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	*(vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET) = vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK;
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_MM2S_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	//vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	vdma_S2MM_DMACR = *(vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_S2MM_DMACR);
	// Address, offset, data
	//XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
	*(vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET) = vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK;
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_S2MM_DMACR & ~XAXIVDMA_CR_TAIL_EN_MASK);
}


/**
 * Re-enables the hardware pipeline.
 */
void enable_hardware(uint16_t* vdma_config)
{
	uint32_t vdma_S2MM_DMACR, vdma_MM2S_DMACR;
	// Grab the DMA Control Registers, and re-enable circular park mode.
	//vdma_MM2S_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	vdma_MM2S_DMACR = *(vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET);
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_MM2S_DMACR);
	// Address, offset, data
	//XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	*(vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET) = vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK;
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_TX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_MM2S_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	//vdma_S2MM_DMACR = XAxiVdma_ReadReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	vdma_S2MM_DMACR = *(vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET);
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_S2MM_DMACR);
	// Address, offset, data
	//XAxiVdma_WriteReg(config->vdma_hdmi.BaseAddr, XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET, vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
	*(vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET) = vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK;
	printf("addr: %x\r\nvalue: %x\r\n", (vdma_config+XAXIVDMA_RX_OFFSET+XAXIVDMA_CR_OFFSET), vdma_S2MM_DMACR | XAXIVDMA_CR_TAIL_EN_MASK);
}
