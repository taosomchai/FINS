#include "FINS.h"

//EthernetServer FinsServer(FINS_PORT);
EthernetUDP FinsServer;

FINS::FINS()
{
}

void FINS::begin(uint8_t sDNA, uint8_t sDA1, uint8_t sDA2)
{
	DNA = sDNA;
	DA1 = sDA1;
	DA2 = sDA2;
	FinsServer.begin(FINS_UDP_PORT);
}

/*
int FINS::Run()
{
  Runs = 1 + Runs * (Runs < 9999);
	
  //****************** Read from socket ****************
  //int dataCnt = FinsServer.available();
  int packetSize = FinsServer.parsePacket();
  if(packetSize)
  {
    Reads = 1 + Reads * (Reads < 9999);
    int i = 0;
    while(FinsServer.available())
    {
      ByteArray[i] = FinsServer.read();
      i++;
    }
	//int j;
	//for(j=0;j<i;j++) {
	//	Serial.print(ByteArray[j], HEX);
	//	Serial.print(",");
	//}

	//Set FC
	FC = 0;
    if(i<14)			//ILLEGAL RESPONSE LENGTH CHECK
        FC = -1;		//FINS length error
    else {  
        if((ByteArray[3]!=DNA)||(ByteArray[4]!=DA1)||(ByteArray[5]!=DA2)) 
            FC = -2;	//illegal source address error
        else {
			if((ByteArray[10]==0x01)&&(ByteArray[11]==0x01))
				FC = 1;	//Memory area read
			if((ByteArray[10]==0x01)&&(ByteArray[11]==0x02))
				FC = 2;	//Memory area write
		}
	}
    if(!Active)
    {
      Active = true;
      PreviousActivityTime = millis();
    }
  }
  if(millis() > (PreviousActivityTime + 60000))
  {
    if(Active)
    {
      Active = false;
    }
  }

  int Start, WordDataLength, ByteDataLength, MessageLength;
  //****************** Read Registers ******************
  if(FC == 1)
  {
	if(ByteArray[12]==0x82) 
	{
		Start = word(ByteArray[13], ByteArray[14]);
		WordDataLength = word(ByteArray[16],ByteArray[17]);
		ByteDataLength = WordDataLength * 2;
		//Message Response
		ByteArray[0] |= 0x40;
		ByteArray[3]  = ByteArray[6];
		ByteArray[4]  = ByteArray[7];
		ByteArray[5]  = ByteArray[8];
		ByteArray[6]  = DNA;
		ByteArray[7]  = DA1;
		ByteArray[8]  = DA2;
		//SID , MRC, SRC
		ByteArray[12] = 0x00;
		ByteArray[13] = 0x00;
		for(int i = 0; i < WordDataLength; i++)
		{
			ByteArray[14 + (i * 2)] = highByte(DM[Start + i]);
			ByteArray[15 + (i * 2)] =  lowByte(DM[Start + i]);
		}
		MessageLength = ByteDataLength + 14;
		//client.write(ByteArray, MessageLength);
		FinsServer.beginPacket(FinsServer.remoteIP(), FinsServer.remotePort());    //send command packet
		FinsServer.write(ByteArray, MessageLength); 
		FinsServer.endPacket();

		Writes = 1 + Writes * (Writes < 9999);
		FC = 0;
	}
  }
  //****************** Write Multiple Registers ******************
  if(FC == 2)
  {
	if(ByteArray[12]==0x82) 
	{
		Start = word(ByteArray[13],ByteArray[14]);
		WordDataLength = word(ByteArray[16],ByteArray[17]);
		ByteDataLength = WordDataLength * 2;
		for(int i = 0; i < WordDataLength; i++)
		{
			DM[Start + i] =  word(ByteArray[ 18 + (i * 2)],ByteArray[19 + (i * 2)]);
		}
		//Message Response
		ByteArray[0] |= 0x40;
		ByteArray[3]  = ByteArray[6];
		ByteArray[4]  = ByteArray[7];
		ByteArray[5]  = ByteArray[8];
		ByteArray[6]  = DNA;
		ByteArray[7]  = DA1;
		ByteArray[8]  = DA2;
		//SID , MRC, SRC
		ByteArray[12] = 0x00;
		ByteArray[13] = 0x00;
		MessageLength = 14;
		//client.write(ByteArray, MessageLength);
		FinsServer.beginPacket(FinsServer.remoteIP(), FinsServer.remotePort());    //send command packet
		FinsServer.write(ByteArray, MessageLength); 
		FinsServer.endPacket();

		Writes = 1 + Writes * (Writes < 9999);
		FC = 0;
	}
  }
  //DM[20] = Runs;
  //DM[21] = Reads;
  //DM[22] = Writes;
  return FC;
}
*/

int FINS::Run(uint16_t *data)
{
  Runs = 1 + Runs * (Runs < 9999);
	
  //****************** Read from socket ****************
  //int dataCnt = FinsServer.available();
  int packetSize = FinsServer.parsePacket();
  if(packetSize)
  {
    Reads = 1 + Reads * (Reads < 9999);
    int i = 0;
    while(FinsServer.available())
    {
      ByteArray[i] = FinsServer.read();
      i++;
    }
	//int j;
	//for(j=0;j<i;j++) {
	//	Serial.print(ByteArray[j], HEX);
	//	Serial.print(",");
	//}

	//Set FC
	FC = 0;
    if(i<14)			//ILLEGAL RESPONSE LENGTH CHECK
        FC = -1;		//FINS length error
    else {  
        if((ByteArray[3]!=DNA)||(ByteArray[4]!=DA1)||(ByteArray[5]!=DA2)) 
            FC = -2;	//illegal source address error
        else {
			if((ByteArray[10]==0x01)&&(ByteArray[11]==0x01))
				FC = 1;	//Memory area read
			if((ByteArray[10]==0x01)&&(ByteArray[11]==0x02))
				FC = 2;	//Memory area write
		}
	}
    if(!Active)
    {
      Active = true;
      PreviousActivityTime = millis();
    }
  }
  if(millis() > (PreviousActivityTime + 60000))
  {
    if(Active)
    {
      Active = false;
    }
  }

  int Start, WordDataLength, ByteDataLength, MessageLength;
  //****************** Read Registers ******************
  if(FC == 1)
  {
	if(ByteArray[12]==0x82) 
	{
		Start = word(ByteArray[13], ByteArray[14]);
		WordDataLength = word(ByteArray[16],ByteArray[17]);
		ByteDataLength = WordDataLength * 2;
		//Message Response
		ByteArray[0] |= 0x40;
		ByteArray[3]  = ByteArray[6];
		ByteArray[4]  = ByteArray[7];
		ByteArray[5]  = ByteArray[8];
		ByteArray[6]  = DNA;
		ByteArray[7]  = DA1;
		ByteArray[8]  = DA2;
		//SID , MRC, SRC
		ByteArray[12] = 0x00;
		ByteArray[13] = 0x00;
		for(int i = 0; i < WordDataLength; i++)
		{
			ByteArray[14 + (i * 2)] = highByte(data[Start + i]);
			ByteArray[15 + (i * 2)] =  lowByte(data[Start + i]);
		}
		MessageLength = ByteDataLength + 14;
		//client.write(ByteArray, MessageLength);
		FinsServer.beginPacket(FinsServer.remoteIP(), FinsServer.remotePort());    //send command packet
		FinsServer.write(ByteArray, MessageLength); 
		FinsServer.endPacket();

		Writes = 1 + Writes * (Writes < 9999);
		FC = 0;
	}
  }
  //****************** Write Multiple Registers ******************
  if(FC == 2)
  {
	if(ByteArray[12]==0x82) 
	{
		Start = word(ByteArray[13],ByteArray[14]);
		WordDataLength = word(ByteArray[16],ByteArray[17]);
		ByteDataLength = WordDataLength * 2;
		for(int i = 0; i < WordDataLength; i++)
		{
			data[Start + i] =  word(ByteArray[ 18 + (i * 2)],ByteArray[19 + (i * 2)]);
		}
		//Message Response
		ByteArray[0] |= 0x40;
		ByteArray[3]  = ByteArray[6];
		ByteArray[4]  = ByteArray[7];
		ByteArray[5]  = ByteArray[8];
		ByteArray[6]  = DNA;
		ByteArray[7]  = DA1;
		ByteArray[8]  = DA2;
		//SID , MRC, SRC
		ByteArray[12] = 0x00;
		ByteArray[13] = 0x00;
		MessageLength = 14;
		//client.write(ByteArray, MessageLength);
		FinsServer.beginPacket(FinsServer.remoteIP(), FinsServer.remotePort());    //send command packet
		FinsServer.write(ByteArray, MessageLength); 
		FinsServer.endPacket();

		Writes = 1 + Writes * (Writes < 9999);
		FC = 0;
	}
  }
  //DM[20] = Runs;
  //DM[21] = Reads;
  //DM[22] = Writes;
  return FC;
}
