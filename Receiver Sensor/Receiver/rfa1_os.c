//
// Need to modify radio.c and timer.c to support this
//
// radio.c
//    - modify init() and send() to leave radio in RX state
//    - add radio_tx_done() - set a flag in send(), and clear in tx_done()
//    - add radio_rxed()
//    - modify radio_recv() as radio already in RX state on entry
// timer.c
//    - add ticked()
//    - add ticked flag - set in ISR and clear in ticked()
//
#include <avr/io.h>

#include "leds.h"
#include "rfa1_os.h"
#include "serial.h"
#include "radio.h"
#include "button.h"
#include "hw_timer.h"
#include "app.h"

#define True 1
#define False 0

typedef struct _os_timer {
	unsigned char active;
	unsigned char id;
	unsigned char timedOut;
	unsigned char periodic;
	unsigned char period;
	unsigned char remaining;
} os_timer;

//void os_timer_start(os_timer *t, unsigned char t_id,
//	unsigned char t_secs, unsigned char t_isPeriodic);
//void os_timer_tick(unsigned char t_id);
//void os_timer_init();
//int os_timer_count();
//unsigned char os_timer_expired(int i);
//unsigned char timer_ticked();

#define BUFFER_LEN 128
unsigned char buffer[BUFFER_LEN];

os_timer t0, t1, t2, t3;

#define TIMERS 4
os_timer* os_timer_list[TIMERS]={&t0,&t1,&t2,&t3};

void os_timer_start(os_timer *t, unsigned char t_id,
					unsigned char t_secs, unsigned char t_isPeriodic)
{
	if (t_id<TIMERS) {
   	   t->id = t_id;
	   t->timedOut = False;
	   t->periodic = t_isPeriodic;
	   t->period = t_secs;
	   t->remaining = t_secs;
	   t->active = True;
       os_timer_list[t_id] = t;
   }
}

void timer_start(unsigned char tid, unsigned char t_secs, unsigned char t_isPeriodic)
{
	if (tid<4) {
		os_timer_start(os_timer_list[tid], tid, t_secs, t_isPeriodic);
	}
}

void os_timer_tick(unsigned char t_id)
{
   if (t_id<TIMERS) {
	   if (os_timer_list[t_id]!=0)
	      if (os_timer_list[t_id]->active)
   	         if (os_timer_list[t_id]->remaining>0) {
				 os_timer_list[t_id]->remaining--;
     	         if (os_timer_list[t_id]->remaining==0) {
					 os_timer_list[t_id]->timedOut = True;
	                 if (os_timer_list[t_id]->periodic)
					    os_timer_list[t_id]->remaining = os_timer_list[t_id]->period;
		         }
			 }
   }
}

void os_timer_init()
{
   int i;
   for (i=0; i<TIMERS; i++)
      os_timer_list[i] -> active = false;
   hw_timer_init();
}

int os_timer_count(){
	return TIMERS;
}

unsigned char os_timer_expired(int i)
{
  if (os_timer_list[i]->timedOut) {
     os_timer_list[i]->timedOut = False;
     return 1;
  }
  else
     return 0;
}

unsigned long os_tick=0;
unsigned char timer_ticked()
{
  if ((hw_timer_now_us()) > os_tick) {
          os_tick = hw_timer_now_us() + 10000;
          return 1;
  }
  return 0;
}


int main() {

   int i;
   bool pressed=false;

   //
   // Startup platform and timers
   //

   //platform_init();
   os_timer_init();


   //
   // Call application 'main'
   //

   application_start(NODE_ID);

   //
   // Event handling loop
   //
   for (;;) {

      // process transmit done

	  if (radio_tx_done()) {
		  application_radio_tx_done();
	  }

      // process received messages

	  if (radio_rxed()) {
		  unsigned short dst, src;
		  unsigned char valid, energy, lqi;
		  unsigned char len = radio_recv(buffer,BUFFER_LEN,&dst,&src,&valid,&energy,&lqi);
		  application_radio_rx_msg(dst,src,len,buffer);
	  }

      if (timer_ticked()) {
         for (i=0; i<os_timer_count(); i++)
  		    os_timer_tick(i);
	  }

      for (i=0; i<os_timer_count(); i++) {
		  if (os_timer_expired(i)) {
		     application_timer_tick(i);
		     break; // so that we get a chance to run application_radio_tx_done() before next send() request
		 }
	  }

	  if (button_pressed() && (!pressed)) {
	  	pressed = true;
		application_button_pressed();
	  }
	  else if (button_released() && (pressed)) {
	  	pressed = false;
		application_button_released();
	  }


   }

}

