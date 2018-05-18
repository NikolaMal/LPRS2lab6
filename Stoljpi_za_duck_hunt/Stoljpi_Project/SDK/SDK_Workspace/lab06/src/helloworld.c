/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdbool.h>
#include "platform.h"
#include "xparameters.h"
#include "xintc.h"
#include "vga_periph_mem.h"
#define GPIO_BASEADDR 0x7DE00000

XIntc Intc;

void interrupt_handler(void * baseaddr_p) {
	xil_printf("\n\rtimer timeout.");
}

void print(char *str);
bool stoljpi(){
	u32 output;
	    	//postavljanje o_pwr na 0 (citanje okidaca)

	    	bool o_pwr = 0;
	    	output = ((u32)o_pwr << 2);
	    	Xil_Out32(XPAR_MY_PERIPHERAL_0_BASEADDR, output);

	   		u32 input = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	   		bool in_btn_0 = input & 0x4;
	   		bool i_sig;

	   		//zastita za rafal
	   		while(in_btn_0 == 0x0){
	   			output = ((u32)in_btn_0 <<1);
	   			Xil_Out32(XPAR_MY_PERIPHERAL_0_BASEADDR, output);
	   			input = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	   			in_btn_0 = input & 0x4;

	   		}
	   		//stoji u while-u dok se ne pritisne okidac
	   		while(in_btn_0 == 0x1){
	   			output = ((u32)in_btn_0 << 1);
	   			Xil_Out32(XPAR_MY_PERIPHERAL_0_BASEADDR, output);
	   			input = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	   			in_btn_0 = input & 0x4;
	   		}
	   		//postavljanje o_pwr na 1 (citanje sa senzora)
	   		o_pwr = 1;

	   		output = ((u32)o_pwr << 2);

	   		Xil_Out32(XPAR_MY_PERIPHERAL_0_BASEADDR, output);
	   		int i;
	   		for(i = 0; i < 1000000;i++);
	   		input = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	   		i_sig = input & 0x4;



	   		if(!i_sig){
	   			printf("belo\n");
	   		}

	   		output = (((u32)i_sig << 1) | ((u32)o_pwr));
	   		Xil_Out32(XPAR_MY_PERIPHERAL_0_BASEADDR, output);


	   		for(i=0;i<5000000;i++);

	   		return i_sig;
}
int main()
{
	bool sig;
	int score = 0;
	char s [6] = "SCORE  ";
    init_platform();
    /*XStatus Status;

    Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);

    Status = XIntc_Connect (&Intc, XPAR_INTC_0_DEVICE_ID,
                                (XInterruptHandler) interrupt_handler,
                                (void *)0);

    Status = XIntc_Start(&Intc, XIN_REAL_MODE);
    XIntc_Enable  (&Intc, XPAR_INTC_0_DEVICE_ID);

    microblaze_enable_interrupts();*/

    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x1);// show frame      2
    //VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xFFFFFF);// foreground 4
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x000000);// background color 5
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000);// frame color      6
    //VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 4*7, 480);
    //VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 4*8, 1);





    while(1){
    	set_cursor(700*26);
    	s[6] = score + 48;
    	clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    	clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    	draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR+0x4);
    	print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR,s,7);

    	sig = stoljpi();
    	if(!sig){
    		score++;
    	}


    }

    return 0;
}
