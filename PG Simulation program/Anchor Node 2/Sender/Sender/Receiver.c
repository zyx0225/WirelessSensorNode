
/*
 * Receiver.c
 */ 


#include <avr/io.h>


#include "app.h"
#include "leds.h"
#include "radio.h"
#include "serial.h"
#include "button.h"
#include "util/delay.h"
#include <math.h>

#include <stdio.h>

//
// Application constants
//
#define MAX_LEN 4
//
// Application global variables
//
extern unsigned char buffer[MAX_LEN][MAX_LEN];
extern unsigned char send[36];
char line[80];
int id;





void application_radio_rx_msg(int dst, int src, int len, unsigned char *data)
{
	int i,j,k;
	float CF=0; //correction factor
	float l;
	unsigned char *b;
	unsigned char a[32];
	
	for (i=0;i<32;i++)
	{
		a[i]='0';
	}
	
	for (i=0;i<36;i++)
	{
		printf(" %c",data[i]);
	}
	printf(" \n\r");
		
	leds_on(1);
	k=data[0]-'0';
	
	data[3]=data[3]+1;

	if (data[0]=='NODE_ID') {}
	else
	{
		buffer[k][0]=data[0];
		buffer[k][1]=data[1];
		buffer[k][2]=data[2];
		if (buffer[k][3]>data[3])
		{
			buffer[k][3]=data[3];
		}
		l=(float) (buffer[NODE_ID][1]-buffer[k][1])*(buffer[NODE_ID][1]-buffer[k][1])+(buffer[NODE_ID][2]-buffer[k][2])*(buffer[NODE_ID][2]-buffer[k][2]);	
		printf(" l=%f",l);
		printf(" \n\r");	
		CF=(float) (sqrt(l))/(buffer[k][3]-'0');
		printf("cf=%f",CF);
		printf(" \n\r");
		snprintf(a,sizeof(a),"%4.4f",CF);
		//    double to char 
	}
			
	for (i=0; i<4;i++)
	{
		send[i]=buffer[2][i];
	}
	for (i=4; i<36;i++)
	{
		send[i]=a[i-4];
	}
	 
	//printf(" %f",CF);
	//printf(" \n\r");
	
	for(i=0; i<4;i++)
	{
		for(j=0; j<4;j++)
		{
			printf(" %c",buffer[i][j]);	
		}		
		printf(" \n\r");
	}
	printf(" \n\r");
	printf(" \n\r");
	
leds_off(1);

}