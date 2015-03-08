/*
 * Sender.c
 *
 * Created: 06/02/2013 09:21:32
 *  Author: sb
 */ 


#include <avr/io.h>


#include "app.h"
#include "leds.h"
#include "radio.h"
#include "serial.h"
#include "button.h"
#include "util/delay.h"

#include <stdio.h>

int id;
int sequence_number=0;
int inuse;
int flag=1;
unsigned char power=15;

#define MAX_DELAY 1000
#define MAX_LEN 4

unsigned char buffer[MAX_LEN][MAX_LEN]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
unsigned char send[36];
unsigned char CF[32];
double PG[2][3]={0.,};

void application_start(int nodeId){

	// initialize the devices

	serial_init(9600);
	leds_init();
	button_init();
	radio_init(nodeId);
	radio_start();
	srandom(nodeId);
	radio_set_power(power);
	

	double delay=(MAX_DELAY*random())/RANDOM_MAX;
	void _delay_ms(delay);
	
	for (int i=0;i<32;i++)
	{
		CF[i]='0';
	}
		

	// start the application

	id = nodeId;
	printf("Multi-tx transmitter on node: %d: ", id);
	
	timer_start(0,3,1);        //I'm not sure t_id? 1s, periodic
	
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

		// send the node information when time=0
		if(flag){
			leds_off(1);
			flag=0;
		}else{
			leds_on(1);
			flag=1;
		}


	}

}


char line[80];


void application_radio_tx_done(){
	inuse = 0;
}

