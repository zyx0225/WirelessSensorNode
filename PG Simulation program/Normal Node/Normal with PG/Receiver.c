
/*
 * Receiver.c
 *
 * 
 */ 


#include <avr/io.h>


#include "app.h"
#include "leds.h"
#include "radio.h"
#include "serial.h"
#include "button.h"
#include "util/delay.h"

#include <stdio.h>
#include <stdlib.h>
//
// Application constants
//
#define MAX_LEN 4
#define MAX_DELAY 1000
//
// Application global variables
//
extern unsigned char buffer[MAX_LEN][MAX_LEN];
extern unsigned char CF[32];
extern double PG[2][3];
char line[80];
int id;





void application_radio_rx_msg(int dst, int src, int len, unsigned char *data)
{
	int i,j,k;
	unsigned char send[36];
	float lambda[2];
	float P_lambda[2];
	float CF_f;
	for (i=0; i<36;i++)
	{
		send[i]='0';
	}

	leds_on(1);
	
	for (i=0; i<36;i++)
	{
		printf(" %c",data[i]);
	}
	printf(" \n\r");
	printf(" \n\r");
	data[3]=data[3]+1;
	k=data[0]-'0';
	
	if (buffer[k][0]=='0')
	{		
		for(i=0; i<4;i++)
		{
			buffer[k][i]=data[i];
		}
	}
	else
	{
		if (buffer[k][3]<=data[3]){}
		else
		{
			buffer[k][3]=data[3];			
		}
	}                               //update the hop count
	
	for (i=0; i<4;i++)
	{
		send[i]=buffer[k][i];
	}
	for (i=4; i<36;i++)
	{
		send[i]=data[i];
		CF[i-4]=data[i];
	}		
	sscanf(CF,"%4.4f",CF_f);
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
	
	//for(i=0; i<32;i++)
	//{
	//	printf(" %c",CF[i]);
	//}
	//printf(" \n\r");
	
	srandom(NODE_ID);
	double delay=(MAX_DELAY*random())/RANDOM_MAX;
	void _delay_ms(delay);

	radio_send(send,36,0xFFFF); //*buffer,len,dst??
	
	for (i=0;i<2;i++)
	{
		for (j=0;j<3;j++)
		{
			for (k=2;k<3;k++)
			{
				lambda[k-2]=((float) (sqrt((buffer[k][1]-'0'-i)*(buffer[k][1]-'0'-i)+(buffer[k][2]-'0'-j)*(buffer[k][2]-'0'-j))))/CF_f; //CF from char to double
				P_lambda[k-2]=(float) (pow(lambda[k-2],(buffer[k][3]-'0'-1)))*(pow(2.71828,0-lambda[k-2]));
			}
			PG[i][j]=P_lambda[0]*P_lambda[1];
		}
	}
	
	for(i=0; i<2;i++)
	{
		for(j=0; j<3;j++)
		{
			printf(" %4.5f",PG[i][j]);
		}
		printf(" \n\r");
	}
	printf(" \n\r");

float max=PG[0][0];
int maxposition[2]={0,0};
for (i=0;i<2;i++)
{
	for (j=0;j<3;j++)
	{
		if(PG[i][j]>max)
		{
			max=PG[i][j];
			maxposition[0]=i;
			maxposition[1]=j;
		}
	}
}
printf("the location of this node is: x=%d y=%d", maxposition[0], maxposition[1]);
printf(" \n\r");
	
leds_off(1);

}


