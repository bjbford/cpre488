/*
 * imu_test.c
 *
 *  Created on: Apr 3, 2019
 *      Author: bjbford
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xiicps.h"

#define IMU_I2C_ADDR 0x68	//slave address

void i2c_reg_write(XIicPs *InstPtr, u8 *MsgBufPtr, s32 count);
void i2c_reg_read(XIicPs *InstPtr, u8 reg_addr, s32 count);
void i2c_startup(XIicPs *i2c0_ps);

u8 rec_msg_data_buf[14];

int main(){
    init_platform();

    xil_printf("Hello World\n\r");

    // Setup i2c0 interface
    // Refer to page 619 of https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf
    XIicPs i2c0_ps;
    XIicPs_Config *i2c0_config_ptr;
    i2c0_config_ptr = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
    if(i2c0_config_ptr == NULL) {
    	xil_printf("i2c config info was not found...\r\n");
    	return EXIT_FAILURE;
    }
    s32 status = XIicPs_CfgInitialize(&i2c0_ps, i2c0_config_ptr, i2c0_config_ptr->BaseAddress);
    if(status != XST_SUCCESS) {
    	xil_printf("i2c configuration failed to initialize...\r\n");
		return EXIT_FAILURE;
    }

    xil_printf("I2C startup initializing...\r\n");
    // MPU-9250 IMU start-up sequence, refer to given MPU-9250.pdf
    i2c_startup(&i2c0_ps);
    xil_printf("I2C startup completed\r\n");

    // Receive message buffer, data count and reg addr
	s32 rec_data_count;
	u8 read_reg_addr;
	u16 raw_accel_x, raw_accel_y, raw_accel_z;
	u16 raw_gyro_x, raw_gyro_y, raw_gyro_z;
	s16 accel_x, accel_y, accel_z;
	s16 gyro_x, gyro_y, gyro_z;

	// Read 14 bytes starting at address 0x3B
	rec_data_count = 14;
	read_reg_addr = 0x3B;
	while(1) {
		i2c_reg_read(&i2c0_ps, read_reg_addr, rec_data_count);
		// Accelerometer data
		raw_accel_x = (s16)((rec_msg_data_buf[0] << 8) | rec_msg_data_buf[1]);
		raw_accel_y = (s16)((rec_msg_data_buf[2] << 8) | rec_msg_data_buf[3]);
		raw_accel_z = (s16)((rec_msg_data_buf[4] << 8) | rec_msg_data_buf[5]);

		accel_x = (s16)raw_accel_x;
		accel_y = (s16)raw_accel_y;
		accel_z = (s16)raw_accel_z;

//		if (raw_accel_x > 32767) {
//			accel_x = raw_accel_x - 32768;
//		}
//		if (raw_accel_x > 32767) {
//			accel_x = raw_accel_x - 32768;
//		}
//		if (raw_accel_x > 32767) {
//			accel_x = raw_accel_x - 32768;
//		}
		// ignore next two bytes (5 and 6): temperature data?
		// Gyroscope data
		raw_gyro_x = ((rec_msg_data_buf[8] << 8) | rec_msg_data_buf[9]);
		raw_gyro_y = ((rec_msg_data_buf[10] << 8) | rec_msg_data_buf[11]);
		raw_gyro_z = ((rec_msg_data_buf[12] << 8) | rec_msg_data_buf[13]);

		gyro_x = (s16)raw_gyro_x;
		gyro_y = (s16)raw_gyro_y;
		gyro_z = (s16)raw_gyro_z;

//		xil_printf("raw accx: %5d raw accy: %5d raw accz: %5d ", raw_accel_x, raw_accel_y, raw_accel_z);
//		xil_printf("raw gyrox: %5d raw gyroy: %5d raw gyroz: %5d\r", raw_gyro_x, raw_gyro_y, raw_gyro_z);
		xil_printf("%d,%d,%d,%d,%d,%d,\r\n", accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z);
		usleep(500000);
	}
    cleanup_platform();
    return EXIT_SUCCESS;
}

/**
 * I2C write transaction from master to slave.
 * @param	InstPtr:
 * @param	MsgBufPtr: pointer to send message buffer
 * @param	count: number of bytes of data including reg. addr.
 */
void i2c_reg_write(XIicPs *InstPtr, u8 *MsgBufPtr, s32 count) {
	s32 status = XIicPs_MasterSendPolled(InstPtr, MsgBufPtr, count, (u16)IMU_I2C_ADDR);
	if(status != XST_SUCCESS) {
		xil_printf("I2C write failure!\r\n");
	}
}

/**
 * I2C read transaction from master to slave.
 * @param	InstPtr:
 * @param	MsgBufPtr: pointer to send message buffer
 * @param	count: number of bytes of data including reg. addr.
 *
 * @return	Returns pointer to receive message buffer.
 */
void i2c_reg_read(XIicPs *InstPtr, u8 reg_addr, s32 count) {
	s32 write_status = XIicPs_MasterSendPolled(InstPtr, &reg_addr, (s32)1, (u16)IMU_I2C_ADDR);
	if(write_status != XST_SUCCESS) {
		xil_printf("I2C write-to-initiate-read failure!\r\n");
	}

	s32 read_status = XIicPs_MasterRecvPolled(InstPtr, rec_msg_data_buf, count, (u16)IMU_I2C_ADDR);
	if(read_status != XST_SUCCESS) {
		xil_printf("I2C read failure!\r\n");
	}
	//return rec_msg_data_buf;
}

/**
 * I2C MPU-9250 IMU start-up sequence, refer to given MPU-9250.pdf
 * and http://www.invensense.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf
 */
void i2c_startup(XIicPs *i2c0_ps) {
	u8 send_msg_data_buf[2];
	s32 send_data_count = 2;
	// reset i2c
	XIicPs_Reset(i2c0_ps);
	xil_printf("Finished reset\r\n");

	// Write 0x80 to device register address 0x6B:
	// Power Management 1, H_RESET: reset the internal registers to restore default settings
	send_msg_data_buf[0] = 0x6B; // register address
	send_msg_data_buf[1] = 0x80; // data
	i2c_reg_write(i2c0_ps, send_msg_data_buf, send_data_count);
	xil_printf("Finished initial write\r\n");

	// sleep for 5 ms
	usleep(5000);

	// Write 0x30 to device register address 0x6B:
	// Power Management 1
	// Auto select best clock source-- PLL if ready, else internal oscillator.
	send_msg_data_buf[0] = 0x6B; // register address
	send_msg_data_buf[1] = 0x03; // data
	i2c_reg_write(i2c0_ps, send_msg_data_buf, send_data_count);

	// Write 0x03 to device register address 0x1A:
	// Configuration: Gyroscope bandwidth 41 Hz, delay: 5.9 ms, Fs (kHz): 1
	send_msg_data_buf[0] = 0x1A; // register address
	send_msg_data_buf[1] = 0x03; // data
	i2c_reg_write(i2c0_ps, send_msg_data_buf, send_data_count);

	// Write 0x18 to device register address 0x1B:
	// Gyroscope Configuration: Gyroscope full scale select: +2000 dps
	send_msg_data_buf[0] = 0x1B; // register address
	send_msg_data_buf[1] = 0x18; // data
	i2c_reg_write(i2c0_ps, send_msg_data_buf, send_data_count);

	// Write 0x10 to device register address 0x1C:
	// Accelerometer Configuration: accel full scale select: +/- 8g
	send_msg_data_buf[0] = 0x1C; // register address
	send_msg_data_buf[1] = 0x10; // data
	i2c_reg_write(i2c0_ps, send_msg_data_buf, send_data_count);
}
