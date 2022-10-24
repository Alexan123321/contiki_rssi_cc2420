#include "rssi_scanner.h"

PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process);
PROCESS_THREAD(main_process, ev, data) {
  PROCESS_BEGIN();
  LOG_INFO("Starting main... \n");
  //Measure RSSIs of all channels: 
  measure_rssis();
  //Print the channel information obtained: 
  print_channel_information();
  //Calculate the best channel:
  find_best_channel();
  //Print best channel information:
  print_best_channel_information();
  //Set channel to the best: 
  set_best_channel();
  LOG_INFO("Ending main... \n");
  PROCESS_END();
}