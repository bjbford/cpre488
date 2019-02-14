/**
 * Responsible for observing register values and configuring corresponding
 * modes of operation
 *
 * Author: Jared Danner
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <xparameters.h>
#include <xil_exception.h>
#include <xgpiops.h>
#include <xgpio.h>
#include <stdbool.h>

// SWITCH bitmasks.
#define SW_0       0x01
#define SW_1       0x02
#define SW_2       0x04
#define SW_3       0x08
#define SW_4	   0x10 // Unconfirmed

// BUTTON bitmasks.
#define BTN_CENTER 0x01
#define BTN_DOWN   0x02
#define BTN_LEFT   0x04
#define BTN_RIGHT  0x08
#define BTN_UP     0x10  // Unconfirmed

// Used in the record array to store PPM frames w/ their channel values.
#define MAX_FRAMES_TO_RECORD 50

// Register address for the switch and button gpio ports.
uint32_t *sw_ptr = XPAR_SWS_8BITS_BASEADDR;
uint32_t *btn_ptr = XPAR_BTNS_5BITS_BASEADDR;

// Relay mode for axi_ppm module.
// HARDWARE - axi_ppm directly passes PPM_Input to PPM_Output.
// SOFTWARE - CPU reads in 6 frames into tempoary registers
//            that the FSM will use to generate the output.
enum Relay {HARDWARE, SOFTWARE};
enum Relay relay_mode = HARDWARE;

// Debug mode for axi_ppm module.
// NONE     - No UART output.
// SOFTWARE - Prints (via UART) values of PPM channels stored
//			  in slave registers from the axi_ppm module.
enum Debug {NONE, SOFTWARE};
enum Debug debug_mode = NONE;

// Recoding mode for axi_ppm module.
// NONE     - No PPM recording.
// RECORD   - BTN_DOWN stores next PPM in array and increments index.
//			  BTN_UP rewinds the recording by decrementing array index.
enum Software_Record {NONE, RECORD};
enum Software_Record record_mode = NONE;

// Replay mode for axi_ppm module.
// NONE     - No PPM replay.
// REPLAY   - BTN_RIGHT transmits any stored PPM values over axi_ppm.
// 			  BTN_LEFT decrements the current play index.
enum Software_Replay {NONE, REPLAY};
enum Software_Replay replay_mode = NONE;

// Filter mode for the axi_ppm module.
// NONE     - No value verification.
// FILTER   - Verifies all values being sent to the drone to ensure
//			  those values do not place the craft in an unstable position.
enum Software_Filter {NONE, FILTER};
enum Software_Filter filter_mode = NONE;

// User given command to exit the program.
// True = Yes
// False = No
bool exit_flag = false;

// PPM Frame recoding array.
// I = Specific PPM Frame
// J = Channels in each PPM frame.
//                   I           J
int record[MAX_FRAMES_TO_RECORD][6];

// Refers to the above record array.
// This index corresponds to the I or row value to keep track of the
// current recorded PPM Frame.
int frame_index = 0;

// Holds the current index of the PPM replay array.
int replay_index = 0;


// Main pulling loop of program.
void main();
// Initializes various system components.
void initialize_system();
// Checks register values and sets corresponding modes.
void check_registers();
// Controls how the PPM_Output is generated.
void relay_mode_handler();
// Sends PPM Channel values via UART for debugging.
void debug_mode_handler();
// Records & rewinds the current PPM capture.
void record_mode_handler();
// Replays the PPM Frame values to the axi_ppm.
void replay_mode_handler();
// Verifies all values being sent to the drone.
void filter_mode_handler();
// Checks min and max values of each PPM channel.
void channel_boundary_correction()