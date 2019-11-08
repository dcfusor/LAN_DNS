
// warning - some of this was very obscure and not in the normal dox

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//@@@ below are hardcoded!
#define WAP "dougswap"
#define WPASS "fusordoug"
#define DNSPORT 53831
#define DATAPORT 42042
// note leading space below
#define CAPS F(" bp rhi ti li ss")

WiFiUDP tport;  // port for tellem stuff
WiFiUDP dport;  // for regular comm
char packetBuffer[255]; // to handle anything coming in that port

IPAddress myip; // my ip as assigned by dhcp
IPAddress bcip; // broadcast addr for this network
char sendstring[80]; // tellem send string

unsigned long milliz = 0;
unsigned long minm = 0; // milliseconds in minute, melts in your mouth
bool debug = true;

////////////////////////////////////////////
void build_host_info()
{ // name is built someplace I can't find to change (issue on web, no one can)
 unsigned char mac[6]; // my mac, I hope
 String mymsg; // name, space, myip for tellem
 WiFi.macAddress(mac); // get our mac address to build our hostname with   
 myip = WiFi.localIP(); // class and union of dword and 4 bytes
 bcip = myip; // copy net address but
 bcip[3] = 255; // make it broadcast address
 mymsg = myip.toString();
 mymsg += " ESP_";
 for (int i=3;i<6;i++) // we just want last 3 of mac
 {
   mymsg += String(mac[i],HEX);
 }
 mymsg += CAPS;  // capabilities for this unit
 mymsg.toCharArray(sendstring,80);
}
////////////////////////////////////////////
void tellem()
{ // broadcast our name and IP on the tellme port
  tport.beginPacket(bcip,DNSPORT);
  tport.write(sendstring);
  tport.endPacket(); 
}
////////////////////////////////////////////
void ditch_incoming()
{
  int packetSize = tport.parsePacket();
  if (packetSize) {
    int len = tport.read(packetBuffer, 254);
    if (debug)
    {
    if (len > 0) packetBuffer[len] = 0;
    Serial.println(packetBuffer);
    }  
  }
}
////////////////////////////////////////////

void setup() {
//  wifi_station_set_hostname( "test11" ); // no effect on hostname, which seems to be:
//ESP_88BF4E (the numbers are the end of the mac address)
  Serial.begin(115200);
  WiFi.begin(WAP, WPASS);
  while (WiFi.status() != WL_CONNECTED) { // wait for dhcp
   delay(500);
   Serial.print(F("."));
  }
  tport.begin(DNSPORT);
  dport.begin(DATAPORT);
  build_host_info();
}
////////////////////////////////////////////

void loop() {
  yield(); // let background run some 
  minm = millis() - milliz;
  if (minm >= 60000)
  {
   tellem(); 
   milliz = millis();
   minm = 0; 
  }
 
 if (minm) // in oher words, we are not at top of minute 
 {
   if (0 == minm%1000) ditch_incoming(); // in case it matters?
 }

} // end of loop
