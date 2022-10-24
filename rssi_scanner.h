//Define libraries:
#include "contiki.h"
#include "CC2420.h"
#include "sys/log.h"
#include <limits.h>

//Define logging: 
#define LOG_MODULE "RSSI Log"
#define LOG_LEVEL LOG_LEVEL_INFO

//Define channel constants:
#define MAX_CHANNEL 26
#define MIN_CHANNEL 11
#define NUMBER_OF_CHANNELS (MAX_CHANNEL - MIN_CHANNEL + 1)

//Define number of measurements to be taken for the average:
#define NUMBER_OF_RSSI_MEASUREMENTS 100

//Define the best channel globally:
int best_channel = 0;
int best_rssi = 0;
int rssi[NUMBER_OF_CHANNELS] = {};

//Measures the RSSI on a given channel: 
int measure_rssi(int channel) {
    //Turn on the channel listener:
    cc2420_on();
    //Set the channel on the listener:
    cc2420_set_channel(channel);
    int rssi_sum = 0;
    //Measure average RSSI:
    for(int i = 0; i < NUMBER_OF_RSSI_MEASUREMENTS; i++) {
        rssi_sum += cc2420_rssi();
    }
    //Calculate the average RSSI on the channel:
    rssi_sum /= NUMBER_OF_RSSI_MEASUREMENTS;
    //Turn off the channel listener:
    cc2420_off();
    //Return the rssi:
    return rssi_sum;
}

//Measures the RSSI on all channels:
void measure_rssis() {
    LOG_INFO("Measuring RSSIs... \n");
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        rssi[i] = measure_rssi(i + MIN_CHANNEL);
    }
}

//Selects the best channel
void find_best_channel() {
    LOG_INFO("Finding the best channel... \n");
    //Define channel variable: 
    best_channel = 0;
    //And lowest RSSI:
    best_rssi = rssi[0]; 
    //Find the lowest RSSI and corresponding channel:
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        if(rssi[i] < best_rssi) {
            best_rssi = rssi[i];
            best_channel = i + MIN_CHANNEL;
        }
    }
}

//Prints channel information: 
void print_channel_information() {
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++) {
        LOG_INFO("Channel: %d, RSSI: %d\n", i + MIN_CHANNEL, rssi[i]);
    }
}

//Prints best channel information: 
void print_best_channel_information() {
    LOG_INFO("Best channel: %d, Best RSSI: %d\n", best_channel, best_rssi);
}

//Set best channel:
void set_best_channel() {
    LOG_INFO("Setting current channel to the best... \n");
    cc2420_set_channel(best_channel);
}
