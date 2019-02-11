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
void UAVInterfacing_Run()
{
	// Program will run until exit command is given.
	while(exit_status == false)
	{
		// Checks register values of certain buttons & switches.
		// Sets modes appropriately.
		check_registers();
		// Controls how the PPM_Output is generated.
		run_relay_mode();
		// Records & rewinds the current PPM capture.
		run_record_mode();
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
}


/**
 * Controls how the PPM_Output is generated.
 * Either by routing the PPM_Input directly
 * to the PPM_Output or by copying over the PPM
 * frames into temporary registers and allowing
 * the FSM to generate the output.
 */
void run_relay_mode()
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
void run_debug_mode()
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
void run_record_mode()
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