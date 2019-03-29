#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


/*-------- CONSTANTS --------*/

#define DEVICE_TREE_BTN 0x41200000
#define DEVICE_TREE_SW 0x41240000
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


/*-------- METHOD DECLARATIONS --------*/

static void launcher_cmd(int fd, int cmd);
int get_command(int* btn_ptr);
static void launcher_cmd(int fd, int cmd);


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
    // Usb address.
    char *dev = LAUNCHER_NODE;
    // Delay between commands.
    unsigned int duration = 100;
	int value_sw = 0;
	int value_btn = 0;
	printf("Hello There\n\n\n");
    // Memory maps the Zed Board switches to the the sw_ptr.
    int* sw_ptr = mmap(&value_sw, // Address of map. Kernal chooses since NULL.
                        4, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDONLY), // Pointer.
                        DEVICE_TREE_SW); // Address of switches in the device tree.
    // Memory maps the Zed Board buttons to the the btn_ptr.
    int* btn_ptr = mmap(&value_btn, // Address of map. Kernal chooses since NULL.
                        4, // Length of map.
                        PROT_READ, // Pages may be read.
                        MAP_SHARED, // Shared map between processes.
                        open("/dev/mem", O_RDONLY), // Pointer.
                        DEVICE_TREE_BTN); // Address of buttons in the device tree.


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
    while(!(*sw_ptr & 80))
    {
        /*-------- DETECT BUTTON PRESS --------*/

        // Checks for valid memory map pointer.
        if(!(*btn_ptr))
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

        // Sends command to launcher.
        launcher_cmd(fd, cmd);
        // Allows the launcher to perform the given command
        // for a certain duration.
        usleep(duration * 1000);




        // ASK BRIAN IF WE WANT THIS (probably).
        launcher_cmd(fd, LAUNCHER_STOP);
    }

    /*-------- PROGRAM CLEANUP --------*/

    // Closes connection to the launcher.
    close(fd);
    // Exit with success flag.
    return EXIT_SUCCESS;
}


/**
 * Obersves the push buttons & returns the appropriate
 * codes to move the missle launcher.
 */
int get_command(int* btn_ptr)
{
    // Switch statement to detect and determine the buttons
    // being pressed.
    switch(*btn_ptr){
        // CENTER BUTTON 0x01
        case 1:
            return LAUNCHER_FIRE;
        // DOWN BUTTON 0x02
        case 2:
            return LAUNCHER_DOWN;
        // LEFT BUTTON 0x04
        case 4:
            return LAUNCHER_LEFT;
        // RIGHT BUTTON 0x08
        case 8:
            return LAUNCHER_RIGHT;
        // UP BUTTON 0x16
        case 16: 
            return LAUNCHER_UP;
        // Default value to return in no buttons are pressed.
        default:
            return LAUNCHER_STOP;
    }
}


/**
 * Responsible for transmitting the user's command
 * to the missle launcher.
 */
static void launcher_cmd(int fd, int cmd) 
{
    // Holds the amount of bytes sent over usb.
    int retval = 0;
    // Sends cmd to missle launcher.
    // Returns # of bytes sent to launcher.
    retval = write(fd, &cmd, 1);
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
    if (cmd == LAUNCHER_FIRE) 
    {
        usleep(5000000);   
    }
}
