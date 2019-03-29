/*
 * launcher_fire_camera.c
 *
 *  Created on: Mar 28, 2019
 *      Author: bjbford
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include "launcher_commands.h"


/*-------- CONSTANTS --------*/
#define DEVICE_TREE_BTN 0x41200000
#define DEVICE_TREE_SW 0x41240000
#define FRAME_BUFF_ADDR 0x10000000
#define HEIGHT 1080
#define WIDTH 1920
#define FRAME_BUFF_LENGTH (WIDTH * HEIGHT)

static void launcher_cmd(int fd, int cmd) {
  int retval = 0;
  
  retval = write(fd, &cmd, 1);
  while (retval != 1) {
    if (retval < 0) {
      fprintf(stderr, "Could not send command to %s (error %d)\n", LAUNCHER_NODE, retval);
    } 

    else if (retval == 0) {
      fprintf(stdout, "Command busy, waiting...\n");
    }
  }

  if (cmd == LAUNCHER_FIRE) {
    usleep(5000000);
  }
}

int main() {
    /*-------- LOCAL VARIABLES --------*/
    int fd;
	int i;
    // command to be sent to launcher.
    // Defaults as stop.
    int cmd = LAUNCHER_STOP;
    // Usb address.
    char *dev = LAUNCHER_NODE;
    // Delay between commands.
    unsigned int duration = 100;
	// pointer to frame buffer
	uint16_t *frame_buff = NULL;
	
	/*-------- OPEN MEMORY MAP ----------*/

	// Attempts to open dev mem
	int mem_map_ptr = open("/dev/mem", O_RDONLY);
    // Checks for errors while opening.
    if (mem_map_ptr == -1)
    {
        perror("UNABLE TO OPEN DEV MEM: %m");
        exit(EXIT_FAILURE);
    }
	
    /*-------- CONNECT TO LAUNCHER --------*/

    // Attempts to establish connection to the launcher.
    fd = open(dev, O_RDWR);
    // Checks for errors while opening.
    if (fd == -1)
    {
        perror("UNABLE TO CONNECT TO LAUNCHER: %m");
        exit(EXIT_FAILURE);
    }
	
    // Memory maps the Zed Board switches to the the sw_ptr.
    uint16_t* sw_ptr = mmap(NULL, // Address of map. Kernel chooses since NULL.
                        4, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        mem_map_ptr, // Pointer.
                        DEVICE_TREE_SW); // Address of switches in the device tree.
    // Memory maps the Zed Board buttons to the the btn_ptr.
    int* btn_ptr = mmap(NULL, // Address of map. Kernal chooses since NULL.
                        4, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        mem_map_ptr, // Pointer.
                        DEVICE_TREE_BTN); // Address of buttons in the device tree.
    // Frame Buffer memory map
    frame_buff = mmap(NULL, // Address of map. Kernel chooses since NULL.
				(sizeof(*frame_buff) * FRAME_BUFF_LENGTH), // Length of map.
				PROT_READ, // Pages may be read.
				MAP_SHARED, // Shared map between processes.
				mem_map_ptr, // Pointer.
				FRAME_BUFF_ADDR); // Address of frame buffer in the device tree.

	// Did the memory map buffer map?
	if(frame_buff == MAP_FAILED) {
        perror("Memory mapping failed: %m");
        // Exit.
        exit(EXIT_FAILURE);
	}
	
	// Write frame buffer to file to analyze
	FILE *fp = fopen("frame_buff.txt", "w");
	
    /*-------- MAIN PROGRAM LOOP --------*/

    // Pulls the button input until switch 7 is
    // flipped high. Switch hex value is 0x80.
    while(!(*sw_ptr & 80))
    {
        /*-------- READ FROM FRAME BUFFER --------*/
		if(!(*btn_ptr)) {
			for(i=0; i<FRAME_BUFF_LENGTH; i++) {
				// read rows
				fprintf(fp, frame_buff[i]);
				fprintf(fp, "\n");
			}
		}
		fclose(fp);

        /*-------- SEND COMMAND --------*/

        // Sends command to launcher.
        launcher_cmd(fd, cmd);
        // Allows the launcher to perform the given command
        // for a certain duration.
        usleep(duration * 1000);

        launcher_cmd(fd, LAUNCHER_STOP);
    }

    /*-------- PROGRAM CLEANUP --------*/
	
	// Remove mappings for address ranges
	munmap(sw_ptr, 4);
    munmap(frame_buff, (sizeof(*frame_buff) * FRAME_BUFF_LENGTH));
	close(mem_map_ptr);
    close(fd);
    // Exit with success flag.
    return EXIT_SUCCESS;
}
