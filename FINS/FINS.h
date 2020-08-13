
#include "Arduino.h"

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#ifndef FINS_h
#define FINS_h

#define FINS_N_DM 125		//Max 16 bit registers for DM is 125
#define FINS_PORT 9600
#define FINS_UDP_PORT 9600


class FINS
{
public:
  FINS();
  void begin(uint8_t sDNA, uint8_t sDA1, uint8_t sDA2);
//  int Run();
  int Run(uint16_t *data);
//  int  DM[FINS_N_DM];
  bool Active;    
  unsigned long PreviousActivityTime;
  int Runs, Reads, Writes;
private: 
  uint8_t ByteArray[260];
  uint8_t FC;
  uint8_t DNA;	// Destination network address: (hex)00, local network
  uint8_t DA1;	// Destination node address: (hex)01, local PLC node selecter switch
  uint8_t DA2;	// Destination unit address: (hex)00, PLC       unit selecter switch
};

#endif

