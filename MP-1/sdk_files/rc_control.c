/**
 * Responsible for observing register values and configuring
 * corresponding modes of operation.
 *
 * Author: Jared Danner
 */

// Self-made Libraries.
#include "rc_control.h"

/**
 * Main pulling loop of program.
 */
void main()
{
	// Program will run until exit command is given.
	while(exit_status == false)
	{
		// Checks register values of certain buttons & switches.
		// Sets modes appropriately.
		check_registers();
		// Controls how the PPM_Output is generated.
		relay_mode_handler();
		// Records & rewinds the current PPM capture.
		record_mode_handler();
		// Replays the PPM Frame values to the axi_ppm.
		replay_mode_handler();
		// Verifies all values being sent to the drone.
		filter_mode_handler();
	}
	// Exit command has been given.
	return;
}


/**
 * Checks the register values for any active
 * button or switch gpio lines.
 */
void check_registers()
{
    /************ SWITCH 0 ************/

	// Checks SWITCH 0's register value.
	// If True -> Hardware Relay Mode
	// If False -> Software Relay Mode
	if(*sw_ptr & SW_0)
	{
		// Register value 1 detected.
		// Switching to Hardware Relay Mode.
		relay_mode = HARDWARE;
	}
	else
	{
		// Register value 0 detected.
		// Switching to Software Relay Mode.
		relay_mode = SOFTWARE;
	}

	/************ BUTTON Center ************/

	// Checks for valid BUTTON Center.
    if(*btn_ptr & BTN_CENTER)
    {
    	// Center button press detected.
    	// Exit application flag set.
    	exit_status = true;
	}

	/************ SWITCH 1 ************/

    // Checks for valid SWITCH 1.
    if(*sw_ptr & SW_1)
    {
    	// Register value 1 detected.
    	// Switching to Software Debug Mode.
    	debug_mode = SOFTWARE;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of debug mode.
		debug_mode = NONE;
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
		record_mode = NONE;
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
		replay_mode = NONE;
	}

	/************ SWITCH 4 ************/

	// Checks for valid SWITCH 4.
    if(*sw_ptr & SW_4)
    {
    	// Register value 1 detected.
    	// Switching to Software Filter Mode.
    	filter_mode = REPLAY;
	}
	else
	{
		// Register value 0 detected.
		// Switching out of Software Filter Mode.
		filter_mode = NONE;
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
	if(relay_mode == HARDWARE)
	{
		// Pass PPM_Input directly to PPM_Output.
		// PPM_Output = PPM_Input;
	}
	// SOFTWARE Relay Mode active.
	else if(relay_mode == SOFTWARE)
	{
		// Copy over PPM frames from slave registers 10 -> 15
		// into slave registers 20 -> 25. 20 thru 25 registers
		// then read by axi_ppm module to generate output.
		int i;
		for(i = 10; i <= 15; i++)
		{
			// slave_register[i+10] = slave_register[i];
		}
	}
}


/**
 * Prints (via UART) values of PPM channels stored in slave
 * registers from the axi_ppm module if debug_mode is active.
 */
void debug_mode_handler()
{
	// SOFTWARE Debug Mode active.
	if(debug_mode == SOFTWARE)
	{
		// Output PPM Channel values via UART.
		int i;
		for(i = #; i <= #; i++)
		{
			//uart_output = slave_register[i];
		}
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
		// Stores next PPM in array and increments index.
		if(*btn_ptr & BTN_DOWN)
		{
			// record[recording_index] = next PPM Frame;
			recording_index++;
		}
		// Rewinds the recording by decrementing array index.
		else if(*btn_ptr & BTN_UP)
		{
			recording_index--;
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
	if(record_mode == REPLAY)
	{
		// Transmits stored PPM values over axi_ppm.
		if(*btn_ptr & BTN_RIGHT)
		{
			// Output PPM Channel values to axi_ppm.
			int i;
			for(i = #; i <= #; i++)
			{
				//axi_ppm = slave_register[i];
			}
		}
		// Decrement the current play index.
		else if(*btn_ptr & BTN_LEFT)
		{
			replay_index--;
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
	if(record_mode == RECORD)
	{
		// magic
	}
}