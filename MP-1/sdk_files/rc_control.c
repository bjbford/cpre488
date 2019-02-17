/**
 * Responsible for observing register values and configuring
 * corresponding modes of operation.
 *
 * Author: Jared Danner
 */

// Self-made Libraries.
#include "rc_control.h"
#include "platform.h"

/**
 * Main pulling loop of program.
 */
int main()
{
	// Clears potentially uncleared memory.
	initialize_system();
	frame_counter_previous = *slv_reg1;
	// Program will run until exit command is given.
	while(exit_flag == false)
	{
		// Only accepts frame input when the End Of Frame pulse has been
		// detected.
		if(frame_counter_previous != *slv_reg1)
		{
			// New cycle. Reset needed status flags.
			button_flag = false;
			// Checks register values of certain buttons & switches.
			// Sets modes appropriately.
			check_inputs();
			// Controls how the PPM_Output is generated.
			relay_mode_handler();
			// Outputs current channel values over UART.
			debug_mode_handler();
			// Records & rewinds the current PPM capture.
			record_mode_handler();
			// Verifies all values being sent to the drone.
			filter_mode_handler();
			// Replays the PPM Frame values to the axi_ppm.
			replay_mode_handler();
			// No buttons were pressed during the last cycle. 
			if(button_flag == false)
			{
				// Reset the counter used for button debouncing.
				debounce_counter = 0;
			}
		}
		// Assigns new previous values.
		frame_counter_previous = *slv_reg1;
		
	}
	// From platform.c : Disables cache.
	cleanup_platform();
	// Exit command has been given.
	return 0;
}


/**
 * Initializes various components of the system. Uart, memory, caches, etc..
 */
void initialize_system()
{
	// From platform.c : Setups up cache and uart.
	init_platform();
	// Clears potentially uncleared memory on startup.
	clear_memory();
}

/**
 * Ensures memory is cleared.
 */
void clear_memory()
{
	// Ensure cleared memory for the PPM Frame array.
	for(int i = 0; i < 50; i++)
	{
		// Clears the channel values for each frame index.
		for(int j = 0; j < 6; j++)
		{
			// Sets all record values to 0.
			record[i][j] = 0;
		}
	}
}

/**
 * Checks the register values for any active
 * button or switch gpio lines.
 */
void check_inputs()
{
    /************ SWITCH 0 ************/

	// Checks SWITCH 0's register value.
	// If True -> Hardware Relay Mode
	// If False -> Software Relay Mode
	if(*sw_ptr & SW_0)
	{
		// Register value 1 detected.
		// Switching to Hardware Relay Mode.
		relay_mode = RELAY_HARDWARE;
	}
	else
	{
		// Register value 0 detected.
		// Switching to Software Relay Mode.
		relay_mode = RELAY_SOFTWARE;
	}

	/************ BUTTON Center ************/

	// Checks for valid BUTTON Center.
	if(*btn_ptr & BTN_CENTER)
	{
		// Center button press detected.
		// Exit application flag set.
		exit_flag = true;
	}

	/************ SWITCH 1 ************/

    // Checks for valid SWITCH 1.
    if(*sw_ptr & SW_1)
    {
    	// Register value 1 detected.
    	// Switching to Software Debug Mode.
    	debug_mode = DEBUG_SOFTWARE;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of debug mode.
		debug_mode = DEBUG_NONE;
	}

	/************ SWITCH 2 ************/

	// Checks for valid SWITCH 2.
    if(*sw_ptr & SW_2)
    {
    	// Register value 1 detected.
    	// Switching to Software Record Mode.
    	record_mode = RECORD;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of Software Record Mode.
		record_mode = REC_NONE;
	}

	/************ SWITCH 3 ************/

	// Checks for valid SWITCH 3.
    if(*sw_ptr & SW_3)
    {
    	// Register value 1 detected.
    	// Switching to Software Replay Mode.
    	replay_mode = REPLAY;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of Software Replay Mode.
		replay_mode = REPLAY_NONE;
	}

	/************ SWITCH 4 ************/

	// Checks for valid SWITCH 4.
    if(*sw_ptr & SW_4)
    {
    	// Register value 1 detected.
    	// Switching to Software Filter Mode.
    	filter_mode = FILTER;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of Software Filter Mode.
		filter_mode = FILTER_NONE;
	}

}


/**
 * Controls how the PPM_Output is generated.
 * Either by routing the PPM_Input directly
 * to the PPM_Output or by copying over the PPM
 * frames into temporary registers and allowing
 * the FSM to generate the output.
 */
void relay_mode_handler()
{
	// HARDWARE Relay Mode active.
	if(relay_mode == RELAY_HARDWARE)
	{
		// Set config register 0 to tell axi_ppm that hardware relay mode
		// has been enabled. PPM_Input routes directly to PPM_Output.
		*slv_reg0 = 0;
	}
	// SOFTWARE Relay Mode active.
	else if(relay_mode == RELAY_SOFTWARE)
	{
		// Signals the coping of PPM frames from slave registers 2 -> 7
		// into slave registers 8 -> 13. 8 thru 13 registers are
		// then read by axi_ppm module to generate output.

		// Channel 1
		*slv_reg8 = *slv_reg2;
		// Channel 2
		*slv_reg9 = *slv_reg3;
		// Channel 3
		*slv_reg10 = *slv_reg4;
		// Channel 4
		*slv_reg11 = *slv_reg5;
		// Channel 5
		*slv_reg12 = *slv_reg6;
		// Channel 6
		*slv_reg13 = *slv_reg7;

		// Set config register 1 to tell axi_ppm that software relay mode
		// has been enabled. PPM_Output is generated from the copied values.
		*slv_reg0 = 1;
	}
}


/**
 * Prints (via UART) values of PPM channels stored in slave
 * registers from the axi_ppm module if debug_mode is active.
 */
void debug_mode_handler()
{
	// SOFTWARE Debug Mode active.
	if(debug_mode == DEBUG_SOFTWARE)
	{
		// Output PPM Channel values via UART.
		// Current PPM Channel values are stored in slave registers 2 thru 7.
//		xil_printf("Ch 1: %6d    Ch 2: %6d    Ch 3: %6d    Ch 4: %6d    Ch 5: %6d    Ch 6: %6d \r", *slv_reg2, *slv_reg3, *slv_reg4, *slv_reg5, *slv_reg6, *slv_reg7);
//		xil_printf("Ch 1: %d \t\t", *slv_reg2);
//		xil_printf("Ch 2: %d \t\t", *slv_reg3);
		xil_printf("Ch 3: %d \t\t", *slv_reg4);
//		xil_printf("Ch 4: %d \t\t", *slv_reg5);
//		xil_printf("Ch 5: %d \t\t", *slv_reg6);s
//		xil_printf("Ch 6: %d \r", *slv_reg7);
	}
}


/**
 * BTN_DOWN stores next PPM in array and increments index.
 * BTN_UP rewinds the recording by decrementing array index.
 */
void record_mode_handler()
{
	// Software Record Mode active.
	if(record_mode == RECORD)
	{
		// Stores next PPM Frame w/ channel vaues in array and increments Frame index.
		if(*btn_ptr & BTN_DOWN)
		{
			// Store values.
			// Channel 1 Value
			record[frame_index][0] = *slv_reg2;
			// Channel 2 Value
			record[frame_index][1] = *slv_reg3;
			// Channel 3 Value
			record[frame_index][2] = *slv_reg4;
			// Channel 4 Value
			record[frame_index][3] = *slv_reg5;
			// Channel 5 Value
			record[frame_index][4] = *slv_reg6;
			// Channel 6 Value
			record[frame_index][5] = *slv_reg7;

			// Array boundary detection. Checks if next move will cause out-of-bounds error.
			if(!((frame_index + 1) > MAX_FRAMES_TO_RECORD))
			{
				// Increments the frame index.
				frame_index++;
			}
		}
		// Rewinds the recording by decrementing array index.
		else if(*btn_ptr & BTN_UP)
		{
			// Clear current Frame data.
			// Channel 1 Value
			record[frame_index][0] = 0;
			// Channel 2 Value
			record[frame_index][1] = 0;
			// Channel 3 Value
			record[frame_index][2] = 0;
			// Channel 4 Value
			record[frame_index][3] = 0;
			// Channel 5 Value
			record[frame_index][4] = 0;
			// Channel 6 Value
			record[frame_index][5] = 0;

			// Array boundary detection. Checks if next move will cause out-of-bounds error.
			if(!((frame_index - 1) < 0))
			{
				// Moves left one column in the 2D array.
				frame_index--;
			}
		}
	}
}


/**
 * BTN_RIGHT transmits any stored PPM values over axi_ppm.
 * BTN_LEFT decrements the current play index.
 */
void replay_mode_handler()
{
	// Software Replay Mode active.
	if(replay_mode == REPLAY)
	{
		// Transmits stored PPM values over axi_ppm.
		if(*btn_ptr & BTN_RIGHT)
		{
			// A button has been pressed.
			button_flag = true;
			// The debounce counter has yet to hit the needed cycles. 
			if(debounce_counter < debounce_threshold)
			{
				debounce_counter++;
				// Signals the a button has started the debounce process.
				debounce_finished = false;
			}
			// Enough cycles have passed of constant button press && this button's functions
			// have yet to be executed for this press.
			else if((debounce_counter >= debounce_threshold) && (debounce_finished == true))
			{
				// Debounce process finished.
				debounce_finished = true;
				xil_printf("Right button pressed. #: %d", counter++);
				// Output indexed PPM Frame channel values to axi_ppm.
				// Channel 1
				*slv_reg8 = record[replay_index][0];
				// Channel 2
				*slv_reg9 = record[replay_index][1];
				// Channel 3
				*slv_reg10 = record[replay_index][2];
				// Channel 4
				*slv_reg11 = record[replay_index][3];
				// Channel 5
				*slv_reg12 = record[replay_index][4];
				// Channel 6
				*slv_reg13 = record[replay_index][5];

				// Array boundary detection. Checks if next move will cause out-of-bounds error.
				if(!((replay_index + 1) > MAX_FRAMES_TO_RECORD))
				{
					// Array will be inbounds.
					replay_index++;
				}
			}
		}
		// Decrement the current play index.
		if(*btn_ptr & BTN_LEFT)
		{
			// Array boundary detection. Checks if next move will cause out-of-bounds error.
			if(!((replay_index - 1) < 0))
			{
				// Array will be inbounds.
				replay_index--;
			}
		}
	}
}


/**
 * All values to be sent to the drone will be verified
 * to ensure they will not put the craft in an unstable
 * position.
 */
void filter_mode_handler()
{
	// Software Filter Mode active.
	if(filter_mode == FILTER)
	{
		// Ensures channel width value is within its min and max values.
		channel_boundary_correction();
	}
}


/**
 * Ensures each channel's width are within its specified min and max values.
 */
void channel_boundary_correction()
{
	// Ensure that value of each channel is between its min and max.
	//
	//      PPM Frames            units = ms
	// Channel 1 (Roll)    :   0.68 < x < 1.517
	// Channel 2 (Pitch)   :   0.70 < x < 1.512
	// Channel 3 (Thrust)  :   0.70 < x < 1.523
	// Channel 4 (Yaw)     :   0.67 < x < 1.590
	// Channel 5 (PIT)     :   0.60 < x < 1.625
	// Channel 6 (HOX PIT) :   0.60 < x < 1.625

	// Same table but converted from millsecond to clk cycles.
	// ASSUMING 100MHZ CLOCK.
	//
	//      PPM Frames            units = clk cycles
	// Channel 1 (Roll)    :   68,000 < x < 151,700
	// Channel 2 (Pitch)   :   70,000 < x < 151,200
	// Channel 3 (Thrust)  :   70,000 < x < 152,300
	// Channel 4 (Yaw)     :   67,000 < x < 159,000
	// Channel 5 (PIT)     :   60,000 < x < 162,500
	// Channel 6 (HOX PIT) :   60,000 < x < 162,500

	#define CHANNEL_1_MIN 68000
	#define CHANNEL_1_MAX 151700

	#define CHANNEL_2_MIN 70000
	#define CHANNEL_2_MAX 151200

	#define CHANNEL_3_MIN 70000
	#define CHANNEL_3_MAX 152300

	#define CHANNEL_4_MIN 67000
	#define CHANNEL_4_MAX 159000

	#define CHANNEL_5_MIN 60000
	#define CHANNEL_5_MAX 162500

	#define CHANNEL_6_MIN 60000
	#define CHANNEL_6_MAX 162500

	/************ CHANNEL 1 ************/

	// Checks for out-of-bounds Channel 1 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_1_clk_cycles = *slv_reg2;
	if(channel_1_clk_cycles < CHANNEL_1_MIN)
	{
		*slv_reg2 = CHANNEL_1_MIN;
	}
	if(channel_1_clk_cycles > CHANNEL_1_MAX)
	{
		*slv_reg2 = CHANNEL_1_MAX;
	}

	/************ CHANNEL 2 ************/

	// Checks for out-of-bounds Channel 2 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_2_clk_cycles = *slv_reg3;
	if(channel_2_clk_cycles < CHANNEL_2_MIN)
	{
		*slv_reg3 = CHANNEL_2_MIN;
	}
	if(channel_2_clk_cycles > CHANNEL_2_MAX)
	{
		*slv_reg3 = CHANNEL_2_MAX;
	}

	/************ CHANNEL 3 ************/

	// Checks for out-of-bounds Channel 3 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_3_clk_cycles = *slv_reg4;
	if(channel_3_clk_cycles < CHANNEL_3_MIN)
	{
		*slv_reg4 = CHANNEL_3_MIN;
	}
	if(channel_3_clk_cycles > CHANNEL_3_MAX)
	{
		*slv_reg4 = CHANNEL_3_MAX;
	}

	/************ CHANNEL 4 ************/

	// Checks for out-of-bounds Channel 4 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_4_clk_cycles = *slv_reg5;
	if(channel_4_clk_cycles < CHANNEL_4_MIN)
	{
		*slv_reg5 = CHANNEL_4_MIN;
	}
	if(channel_4_clk_cycles > CHANNEL_4_MAX)
	{
		*slv_reg5 = CHANNEL_4_MAX;
	}

	/************ CHANNEL 5 ************/

	// Checks for out-of-bounds Channel 5 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_5_clk_cycles = *slv_reg6;
	if(channel_5_clk_cycles < CHANNEL_5_MIN)
	{
		*slv_reg6 = CHANNEL_5_MIN;
	}
	if(channel_5_clk_cycles > CHANNEL_5_MAX)
	{
		*slv_reg6 = CHANNEL_5_MAX;
	}

	/************ CHANNEL 6 ************/

	// Checks for out-of-bounds Channel 6 width.
	// Adjusts to nearest acceptable boundary if beyond limit.
	int channel_6_clk_cycles = *slv_reg7;
	if(channel_6_clk_cycles < CHANNEL_6_MIN)
	{
		*slv_reg7 = CHANNEL_6_MIN;
	}
	if(channel_2_clk_cycles > CHANNEL_6_MAX)
	{
		*slv_reg7 = CHANNEL_6_MAX;
	}
}
