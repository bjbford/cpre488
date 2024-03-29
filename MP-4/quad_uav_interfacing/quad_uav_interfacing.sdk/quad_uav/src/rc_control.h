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
#include "platform.h"
#include "xil_printf.h"
#include "xuartps.h"
#include "xil_cache.h"
#include "math.h"

// SWITCH bitmasks.
#define SW_0       0x01
#define SW_1       0x02
#define SW_2       0x04
#define SW_3       0x08
#define SW_4	   0x10
#define SW_5	   0x20
#define SW_6	   0x40
#define SW_7	   0x80

// BUTTON bitmasks.
#define BTN_CENTER 0x01
#define BTN_DOWN   0x02
#define BTN_LEFT   0x04
#define BTN_RIGHT  0x08
#define BTN_UP     0x10

// Used in the record array to store PPM frames w/ their channel values.
// 3,000 frames = 1 minute (1 min / 20ms = 3000 frames per second)
#define MAX_FRAMES_TO_RECORD 3000
// Holds the 32 bit offset between slave registers.
#define register_offset 0x4

// Slave register macros.
volatile uint32_t *slv_reg0 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR;
volatile uint32_t *slv_reg1 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset;
volatile uint32_t *slv_reg2 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 2;
volatile uint32_t *slv_reg3 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 3;
volatile uint32_t *slv_reg4 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 4;
volatile uint32_t *slv_reg5 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 5;
volatile uint32_t *slv_reg6 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 6;
volatile uint32_t *slv_reg7 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 7;
volatile uint32_t *slv_reg8 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 8;
volatile uint32_t *slv_reg9 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 9;
volatile uint32_t *slv_reg10 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 10;
volatile uint32_t *slv_reg11 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 11;
volatile uint32_t *slv_reg12 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 12;
volatile uint32_t *slv_reg13 = XPAR_AXI_PPM_0_S00_AXI_BASEADDR + register_offset * 13;

// Register address for the switch and button gpio ports.
uint32_t *sw_ptr = XPAR_SWS_8BITS_BASEADDR;
uint32_t *btn_ptr = XPAR_BTNS_5BITS_BASEADDR;

// Relay mode for axi_ppm module.
// RELAY_HARDWARE - axi_ppm directly passes PPM_Input to PPM_Output.
// RELAY_SOFTWARE - CPU reads in 6 frames into tempoary registers
//            that the FSM will use to generate the output.
enum Relay {RELAY_HARDWARE, RELAY_SOFTWARE};
enum Relay relay_mode = RELAY_HARDWARE;

// Debug mode for axi_ppm module.
// DEBUG_NONE     - No UART output.
// DEBUG_SOFTWARE - Prints (via UART) values of PPM channels stored
//			        in slave registers from the axi_ppm module.
enum Debug {DEBUG_NONE, DEBUG_SOFTWARE};

enum Debug debug_mode = DEBUG_NONE;
// Recoding mode for axi_ppm module.
// REC_NONE     - No PPM recording.
// RECORD   - BTN_DOWN stores next PPM in array and increments index.
//			  BTN_UP rewinds the recording by decrementing array index.
enum Software_Record {REC_NONE, RECORD};
enum Software_Record record_mode = REC_NONE;

// Replay mode for axi_ppm module.
// REPLAY_NONE     - No PPM replay.
// REPLAY   - BTN_RIGHT transmits any stored PPM values over axi_ppm.
// 			  BTN_LEFT decrements the current play index.
enum Software_Replay {REPLAY_NONE, REPLAY};
enum Software_Replay replay_mode = REPLAY_NONE;

// Filter mode for the axi_ppm module.
// FILTER_NONE     - No value verification.
// FILTER   - Verifies all values being sent to the drone to ensure
//			  those values do not place the craft in an unstable position.
enum Software_Filter {FILTER_NONE, FILTER};
enum Software_Filter filter_mode = FILTER_NONE;

// User given command to exit the program.
// True = Yes
// False = No
bool exit_flag = false;
bool pid_flag = false;
bool debug_flag = false;

XUartPs uart_ps;
XUartPs_Config *uart_config_ptr;

float yaw_gps = 0.0;

// Refers to the above record array.
// This index corresponds to the I or row value to keep track of the
// current recorded PPM Frame.
int frame_index = 0;

// Holds the current index of the PPM replay array.
int replay_index = 0;

// Holds the previous value of slv_reg1. (Frame counter)
int frame_counter_previous = 0;

// General purpose debug counter for number of button presses.
int counter = 0;

// Initializes various system components.
void initialize_system();
// Checks register values and sets corresponding modes.
void check_inputs();
// Controls how the PPM_Output is generated.
void relay_mode_handler();
// Verifies all values being sent to the drone.
void filter_mode_handler(uint32_t ch1, uint32_t ch2, uint32_t ch3, uint32_t ch4, uint32_t ch5, uint32_t ch6);
// Checks min and max values of each PPM channel.
void channel_boundary_correction(uint32_t ch1, uint32_t ch2, uint32_t ch3, uint32_t ch4, uint32_t ch5, uint32_t ch6);
