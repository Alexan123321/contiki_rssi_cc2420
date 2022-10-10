//Import standard libraries: 
#include <stdio.h>

//Import Contiki-related libraries:
//Used for overall Contiki-functionality
#include "contiki.h"
#include "net/rime/rime.h"
//Used to switch the MAC-layer off and use the radio
#include "net/netstack.h"

#include "dev/leds.h"
#include "cc2420.h"
#include "cc2420_const.h"
#include "dev/spi.h"

/*---------------------------------------------------------------------------*/
/* This assumes that the CC2420 is always on and "stable" */
static void
set_frq(int c)
{
  int f;
  /* We can read even other channels with CC2420! */
  /* Fc = 2048 + FSCTRL  ;  Fc = 2405 + 5(k-11) MHz, k=11,12, ... , 26 */
  f = c + 352; /* Start from 2400 MHz to 2485 MHz, */

  /* Write the new channel value */
  CC2420_SPI_ENABLE();
  SPI_WRITE_FAST(CC2420_FSCTRL);
  SPI_WRITE_FAST((uint8_t)(f >> 8));
  SPI_WRITE_FAST((uint8_t)(f & 0xff));
  SPI_WAITFORTx_ENDED();
  SPI_WRITE(0);

  /* Send the strobe */
  SPI_WRITE(CC2420_SRXON);
  CC2420_SPI_DISABLE();
}
static void
do_rssi(void)
{
  int channel;
  printf("RSSI:");
  for(channel = 11; channel <= 26; ++channel) {
    set_frq(channel);
    printf("%d ", cc2420_rssi() + 100);
  }
  printf("\n");
}
/*---------------------------------------------------------------------------*/
PROCESS(scanner_process, "RSSI Scanner");
AUTOSTART_PROCESSES(&scanner_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(scanner_process, ev, data)
{
  PROCESS_BEGIN();
  /* switch mac layer off, and turn radio on */
  NETSTACK_MAC.off(0);
  cc2420_on();

  while(1) {
    do_rssi();
    PROCESS_PAUSE();
  }

  PROCESS_END();
}
