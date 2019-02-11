/*
 * cyclone_checker.c
 * Create an 8x8 checkerboard using cyclone colors.
 *
 *  Created on: Feb 1, 2019
 *      Author: Brian Bradford, Jared Danner, and Leo Freier
 */


#include <stdio.h>
#include <xparameters.h>
#include "platform.h"
#include "xvtc.h"
#include "xaxivdma.h"
#include "xil_cache.h"


u16 test_image[480][640];

int main() {

	XVtc Vtc;
    XVtc_Config *VtcCfgPtr;

    init_platform();
    int i, j, k;
    // Enable VTC module: Using high-level functions provided by Vendor
    VtcCfgPtr = XVtc_LookupConfig(XPAR_AXIVDMA_0_DEVICE_ID);
    XVtc_CfgInitialize(&Vtc, VtcCfgPtr, VtcCfgPtr->BaseAddress);
    XVtc_EnableGenerator(&Vtc);
    // Initialize Test image for VDMA transfer to VGA monitor
    for(i = 0; i < 8; i ++){ //number of vertical checkers
    	for(j = 0; j < 60; j ++){ //vertical checker pixels
    		for(k = 0; k < 640; k ++){ //horizontal pixels
    			//condition: switching checker colors
    			if((((k / 80) % 2) == 0)){
    				if(i % 2 == 0){
    					test_image[(i * 60) + j][k] = 0x41C0;//cardinal pixel
    				}
    				else{
        				test_image[(i * 60) + j][k] = 0x4BE0;//gold pixel
    				}
    			}
    			else{
    				if(i % 2 == 0){
        				test_image[(i * 60) + j][k] = 0x4BE0;//gold pixel
    				}
    				else{
    					test_image[(i * 60) + j][k] = 0x41C0;//cardinal pixel
    				}
    			}
    		}
    	}
    }
    for(i = 0; i < 480; i ++){
    	for(j = 0; j < 640; j ++){
    		if(i <= 2 || j <= 2 || i >= 477 || j >= 637){
    			test_image[i][j] = 0x0000;
    		}
    	}
    }

    Xil_DCacheFlush();

    // Set up VDMA config registers
    // Simple function abstraction by Vendor for writing VDMA registers
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_CR_OFFSET,  0x03);  // Read Channel: VDMA MM2S Circular Mode and Start bits set, VDMA MM2S Control
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_HI_FRMBUF_OFFSET, 0x00);  // Read Channel: VDMA MM2S Reg_Index
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_START_ADDR_OFFSET, test_image);  // Read Channel: VDMA MM2S Frame buffer Start Addr 1
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_STRD_FRMDLY_OFFSET, 0x0500);  // Read Channel: VDMA MM2S FRM_Delay, and Stride
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_HSIZE_OFFSET, 0x0500);  // Read Channel: VDMA MM2S HSIZE
    XAxiVdma_WriteReg(XPAR_AXIVDMA_0_BASEADDR, XAXIVDMA_MM2S_ADDR_OFFSET + XAXIVDMA_VSIZE_OFFSET, 0x01E0);  // Read Channel: VDMA MM2S VSIZE  (Note: Also Starts VDMA transaction)

    while(1){}
    cleanup_platform();
    return 0;
}



