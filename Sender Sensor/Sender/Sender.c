/*
 * Sender.c
 *
 * Created: 06/02/2013 09:21:32
 */ 


#include <avr/io.h>


#include "app.h"
#include "leds.h"
#include "radio.h"
#include "serial.h"
#include "button.h"

#include <stdio.h>

int id;
int sequence_number=0;
int inuse;
int flag=1;
unsigned char power=15;

#define MAX_LEN 128

unsigned char buffer[MAX_LEN];

void application_start(int nodeId){

	// initialise the devices

	serial_init(9600);
	leds_init();
	button_init();
	radio_init(nodeId);
	radio_start();

	// start the application

	id = nodeId;
	printf("Multi-tx transmitter on node: %d: ", id);
	timer_start(0,1,1);
}

bool pressed=false;

void application_button_pressed()
{
	leds_on(2);
	pressed=true;
}

void application_button_released()
{
	leds_off(2);
	pressed=false;
}

void application_timer_tick(int timer){

	if (timer==0) {

		// send a packet periodically
		if(flag){
			leds_off(1);
			flag=0;
		}else{
			leds_on(1);
			flag=1;
		}

		sequence_number++;

		buffer[0] = 0x40;
		buffer[1] = sequence_number;

		if (pressed)
		buffer[2] = 0x01;
		else
		buffer[2] = 0x00;

		buffer[3] = power;

		//radio_set_power(power);

		if (power==0)
		power=15;
		else
		power = power - 1;

		inuse = 1;
		radio_send(buffer,4,0xFFFF);
		
		float i=1.0;
		float j=5.0;
		float k=j/i;
		float l=4.0;
		printf("k=%d l=%d l=%lf l=%f",k);

	}

}


char line[80];

void application_radio_rx_msg(int dst, int src, int len, unsigned char *data)
{
}


void application_radio_tx_done(){
	inuse = 0;
}

