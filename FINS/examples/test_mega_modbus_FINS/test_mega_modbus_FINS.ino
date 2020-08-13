#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "Mudbus.h"
#include "SD.h"
#include "FINS.h"

Mudbus Mb;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT

FINS OmronFins;

//EthernetServer server(84);
//EthernetClient client;
//String readString;
//const int MAX_LEN = 10;

// The circuit:
// * SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
const int chipSelect = 4;

unsigned int main_loop_counter;
unsigned int cnt;

void setup()
{
  uint8_t mac[]     = { 0x90, 0xA2, 0xDA, 0x00, 0x51, 0x06 };
  uint8_t ip[]      = { 192, 168, 1, 8 };
  uint8_t gateway[] = { 192, 168, 1, 1 };
  uint8_t subnet[]  = { 255, 255, 255, 0 };
  Ethernet.begin(mac, ip, gateway, subnet);
  //Avoid pins 4,10,11,12,13 when using ethernet shield
  delay(5000);
  Serial.begin(115200);
  pinMode(7, INPUT);
  pinMode(8, OUTPUT); 

   // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  File dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

   //server.begin();
   //pinMode(9, OUTPUT); //pin selected to control
   
   pinMode(2, INPUT);
   pinMode(3, INPUT);
   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT);

   OmronFins.begin(0x00, 0x08, 0x00);
} 

void loop()
{
  //Event_EthernetClient();
  Mb.Run();
 
  //Analog inputs 0-1023
  Mb.R[0] = analogRead(A0); //pin A0 to Mb.R[0]
  Mb.R[1] = analogRead(A1);
  Mb.R[2] = analogRead(A2);
  Mb.R[3] = analogRead(A3);
  Mb.R[4] = analogRead(A4);
  Mb.R[5] = analogRead(A5);
  //Analog outputs 0-255
  analogWrite(6, Mb.R[6]); //pin ~6 from Mb.R[6]
  //Digital inputs
  Mb.C[7] = digitalRead(7); //pin 7 to Mb.C[7]
  //Digital outputs
  digitalWrite(8, Mb.C[8]); //pin 8 from Mb.C[8]
  main_loop_counter++;
  Mb.R[9] = main_loop_counter;
  //Inputs();
  //Outputs();

  OmronFins.Run(&Mb.R[0]);

/*
  //map table
  OmronFins.DM[0] = Mb.R[0];
  OmronFins.DM[1] = Mb.R[1];
  OmronFins.DM[2] = Mb.R[2];
  OmronFins.DM[3] = Mb.R[3];
  OmronFins.DM[4] = Mb.R[4];
  OmronFins.DM[5] = Mb.R[5];
  OmronFins.DM[6] = Mb.R[6];
  OmronFins.DM[7] = Mb.R[7];
  OmronFins.DM[8] = Mb.R[8];
  OmronFins.DM[9] = Mb.R[9];
  OmronFins.DM[10] = Mb.R[10];
  OmronFins.DM[11] = Mb.R[11];
  Mb.R[20] = OmronFins.DM[20];
  Mb.R[21] = OmronFins.DM[21];
  Mb.R[22] = OmronFins.DM[22];
  Mb.R[23] = OmronFins.DM[23];
  Mb.R[24] = OmronFins.DM[24];
  Mb.R[25] = OmronFins.DM[25];
  Mb.R[26] = OmronFins.DM[26];
  Mb.R[27] = OmronFins.DM[27];
  Mb.R[28] = OmronFins.DM[28];
  Mb.R[29] = OmronFins.DM[29];
  Mb.R[30] = OmronFins.DM[30];
  Mb.R[31] = OmronFins.DM[31];
*/ 
     
}

/*
 
void Event_EthernetClient(void)
{
    // Create a client connection
  client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString);

          //now output HTML data header

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>HTML form GET example</H1>");

          client.println("<FORM ACTION=\"http://192.168.1.8:84\" method=get >");

          client.println("Pin 9 \"on\" or \"off\": <INPUT TYPE=TEXT NAME=\"LED\" VALUE=\"\" SIZE=\"25\" MAXLENGTH=\"50\"><BR>");

          client.println("<INPUT TYPE=SUBMIT NAME=\"submit\" VALUE=\"Change Pin 9!\">");

          client.println("</FORM>");

          client.println("<BR>");

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
          //stopping client
          client.stop();

          /////////////////////
          if(readString.indexOf("on") >0)//checks for on
          {
            digitalWrite(9, HIGH);    // set pin 4 high
            Serial.println("Led On");
          }
          if(readString.indexOf("off") >0)//checks for off
          {
            digitalWrite(9, LOW);    // set pin 4 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";

        }
      }
    }
  }
}
*/

