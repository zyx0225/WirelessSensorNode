#include <stdlib.h>
#include "rfa1_os.h"

void application_start(int nodeId);
void application_timer_tick(int timer);
void application_radio_rx_msg(int dst, int src, int len, unsigned char *data);
void application_radio_tx_done();
void application_button_pressed();
void application_button_released();

