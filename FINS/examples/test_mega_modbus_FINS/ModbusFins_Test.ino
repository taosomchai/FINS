// File:      ModbusFins_Test.ino
// Software:  Somchai S.
// Board:     Arduino Mega


#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp.h>

#define ledPin 13

#include "Mudbus.h"
#include "SD.h"
#include "FINS.h"

Mudbus Mb;      // Modbus TCP
FINS OmronFins; // FINS Server at Port 9600

// The circuit:
// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
const int chipSelect = 4;

unsigned int main_loop_counter;
unsigned int cnt;
unsigned long new_cycle_time;
unsigned long old_cycle_time;
unsigned long cur_cycle_time;
unsigned long tm;
int           old_Triger, i;
int           SampleID;


void setup()
{
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x01, 0x18 };
  uint8_t ip[]      = { 192, 168, 1, 18 };
  uint8_t gateway[] = { 192, 168, 1, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };

  Serial.begin(115200);
  delay(5000);   //delay for ethernet W5100 is reset ready
  Ethernet.begin(mac, ip, gateway, subnet);
  OmronFins.begin(0x00, ip[3], 0x00); //node id = 18
} 


void loop()
{
  new_cycle_time = millis();
  if( new_cycle_time != old_cycle_time ){
    cur_cycle_time = new_cycle_time - old_cycle_time;
    old_cycle_time = new_cycle_time;    
  }
 
  Mb.Run();

  main_loop_counter++;
  Mb.R[10] = main_loop_counter;
  Mb.R[12] = cur_cycle_time & 0xFFFF;
  Mb.R[13] = cur_cycle_time>>16;


  OmronFins.Run(&Mb.R[0]);

}
