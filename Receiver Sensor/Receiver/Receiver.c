/*
 * Receiver.c
 *
 * Created: 06/02/2013 09:34:21
 */ 


#include <avr/io.h>


#include "app.h"
#include "leds.h"
#include "radio.h"
#include "serial.h"
#include "button.h"
#include "util/delay.h"

#include <stdio.h>

//
// Application constants
//
#define MAX_LEN 4
//
// Application global variables
//
unsigned char array[MAX_LEN][MAX_LEN];
char line[80];
int id;
int nodeId;

void application_start(int nodeId){

	// initialize the devices

	serial_init(9600);
	leds_init();
	button_init();
	radio_init(nodeId);
	radio_start();
	timer_start(0,1,1);

	// start the application

	id = nodeId;
	printf("Receiver on node: %d\n\r", id);
	printf("(Button toggles LED 3)\n\r");
}

bool pressed=false;

void application_button_pressed()
{
	leds_on(3);
	pressed=true;
}

void application_button_released()
{
	leds_off(3);
	pressed=false;
}

void application_timer_tick(int timer){	
}



void application_radio_rx_msg(int dst, int src, int len, unsigned char *data)
{
	int i;

	leds_on(1);

	if (data[0]!=nodeId)
	{
		for(i=0; i<4;i++)
		{
			array[data[0]][i]=data[i];
		}
	}
	
leds_off(1);

}


void application_radio_tx_done(){
}

