#include <ThingSpeak.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>
/*
  #define a = 1;
  #define b = 2;
  #define c = 4;
  #define d = 8;
  #define e = 16;
  #define f = 32;
  #define g = 64;
  #define dp = 128;
*/
#define nulla  0b00111111
#define egy    0b00000110
#define ketto  0b01011011
#define harom  0b01001111
#define negy   0b01100110
#define ot     0b01101101
#define hat    0b01111101
#define het    0b00000111
#define nyolc  0b01111111
#define kilenc 0b01101111
#define pont   0b10000000
#define vesszo 0b01000000

#define elso 1
#define masodik 2
#define harmadik 4
#define negyedik 8
#define otodik 16
#define hatodik 32
#define hetedik 64
#define nyolcadik 128

// NTP Servers:
static const char ntpServerName[] = "hu.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = 1;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

unsigned long counterChannelNumber = 211724;            // Channel ID
const char * myCounterReadAPIKey = "YFTPSFODDBITXOEU"; // Read API Key
const int FieldNumber1 = 1;
const int FieldNumber2 = 2;
int statusCode = 0;

WiFiClient  client;

WiFiUDP Udp;
Ticker hofok;
Ticker kijelzes;
Ticker hofok2;
Ticker set_pwm;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

union adat {
  byte karakter[4];
  unsigned long kimenet;
};

adat ora;

float temp = 0.0;
int pwm = 0;
boolean disp=0;
boolean    set_temp_disp=0;
int homerseklet=0;

void setup() {
  // put your setup code here, to run once:
  ThingSpeak.begin(client);
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D7, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect();

  Udp.begin(localPort);

  setSyncProvider(getNtpTime);
  setSyncInterval(300); //Sets time synchronisation interval (secs)

  pinMode(D1, OUTPUT);
  analogWrite(D1, 0);


  hofok.attach(300, get_temp);
  kijelzes.attach(5, invert_disp);
  //set_pwm.attach_ms(100, strobe);
  get_temp();
}

time_t prevDisplay = 0; // when the digital clock was displayed





void loop() {

  if (timeStatus() != timeNotSet) {

    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();  //Send time and date to Serial
    }
  } else {
    getNtpTime();
  }
if (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    pwm = Serial.parseInt();
    strobe();
}
  
  if (disp) {
    send_disp();
  } else {
    temp_disp();
  }
}

void invert_disp(){
  disp=!disp;
}

void strobe() {
  analogWrite(D1, pwm++);
  Serial.print("PWM: ");
  Serial.println(pwm);
}


void get_temp() {
  temp = ThingSpeak.readFloatField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  Serial.print("Getting temp: "); Serial.print(temp);
  homerseklet = int(temp * 100);
  if (homerseklet==0) get_temp();
}

void temp_disp(void) {
  //Serial.println(temp);
  digitalWrite(D7, LOW);
    switch (homerseklet / 1000)
    {
      case 0:
        shiftOut(D5, D6, MSBFIRST, nulla);
        break;
      case 1:
        shiftOut(D5, D6, MSBFIRST, egy);
        break;
      case 2:
        shiftOut(D5, D6, MSBFIRST, ketto);
        break;
      case 3:
        shiftOut(D5, D6, MSBFIRST, harom);
        break;
    }
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, hetedik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
    digitalWrite(D7, LOW);
    switch ((homerseklet % 1000) / 100)
    {
      case 0:
        shiftOut(D5, D6, MSBFIRST, (nulla + 128));
        break;
      case 1:
        shiftOut(D5, D6, MSBFIRST, (egy + 128));
        break;
      case 2:
        shiftOut(D5, D6, MSBFIRST, (ketto + 128));
        break;
      case 3:
        shiftOut(D5, D6, MSBFIRST, (harom + 128));
        break;
      case 4:
        shiftOut(D5, D6, MSBFIRST, (negy + 128));
        break;
      case 5:
        shiftOut(D5, D6, MSBFIRST, (ot + 128));
        break;
      case 6:
        shiftOut(D5, D6, MSBFIRST, (hat + 128));
        break;
      case 7:
        shiftOut(D5, D6, MSBFIRST, (het + 128));
        break;
      case 8:
        shiftOut(D5, D6, MSBFIRST, (nyolc + 128));
        break;
      case 9:
        shiftOut(D5, D6, MSBFIRST, (kilenc + 128));
        break;
    }
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, hatodik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
    digitalWrite(D7, LOW);
    switch ((homerseklet % 100) / 10)
    {
      case 0:
        shiftOut(D5, D6, MSBFIRST, nulla);
        break;
      case 1:
        shiftOut(D5, D6, MSBFIRST, egy);
        break;
      case 2:
        shiftOut(D5, D6, MSBFIRST, ketto);
        break;
      case 3:
        shiftOut(D5, D6, MSBFIRST, harom);
        break;
      case 4:
        shiftOut(D5, D6, MSBFIRST, negy);
        break;
      case 5:
        shiftOut(D5, D6, MSBFIRST, ot);
        break;
      case 6:
        shiftOut(D5, D6, MSBFIRST, hat);
        break;
      case 7:
        shiftOut(D5, D6, MSBFIRST, het);
        break;
      case 8:
        shiftOut(D5, D6, MSBFIRST, nyolc);
        break;
      case 9:
        shiftOut(D5, D6, MSBFIRST, kilenc);
        break;
    }
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, otodik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
    digitalWrite(D7, LOW);

    switch (homerseklet % 10)
    {
      case 0:
        shiftOut(D5, D6, MSBFIRST, nulla);
        break;
      case 1:
        shiftOut(D5, D6, MSBFIRST, egy);
        break;
      case 2:
        shiftOut(D5, D6, MSBFIRST, ketto);
        break;
      case 3:
        shiftOut(D5, D6, MSBFIRST, harom);
        break;
      case 4:
        shiftOut(D5, D6, MSBFIRST, negy);
        break;
      case 5:
        shiftOut(D5, D6, MSBFIRST, ot);
        break;
      case 6:
        shiftOut(D5, D6, MSBFIRST, hat);
        break;
      case 7:
        shiftOut(D5, D6, MSBFIRST, het);
        break;
      case 8:
        shiftOut(D5, D6, MSBFIRST, nyolc);
        break;
      case 9:
        shiftOut(D5, D6, MSBFIRST, kilenc);
        break;
    }
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, negyedik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
    digitalWrite(D7, LOW);

    shiftOut(D5, D6, MSBFIRST, 0b01100011);
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, harmadik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
    digitalWrite(D7, LOW);


    shiftOut(D5, D6, MSBFIRST, 0b00111001);
    shiftOut(D5, D6, MSBFIRST, 0b00000000);
    shiftOut(D5, D6, MSBFIRST, masodik);
    digitalWrite(D7, HIGH);
    delayMicroseconds(1000);
}


void send_disp() {

  digitalWrite(D7, LOW);
  //Óra tizesek
  switch (hour() / 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, nyolcadik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  digitalWrite(D7, LOW);
  //Óra egyesek
  switch (hour() % 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
    case 3:
      shiftOut(D5, D6, MSBFIRST, harom);
      break;
    case 4:
      shiftOut(D5, D6, MSBFIRST, negy);
      break;
    case 5:
      shiftOut(D5, D6, MSBFIRST, ot);
      break;
    case 6:
      shiftOut(D5, D6, MSBFIRST, hat);
      break;
    case 7:
      shiftOut(D5, D6, MSBFIRST, het);
      break;
    case 8:
      shiftOut(D5, D6, MSBFIRST, nyolc);
      break;
    case 9:
      shiftOut(D5, D6, MSBFIRST, kilenc);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, hetedik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  digitalWrite(D7, LOW);
  //Perc tizesek
  switch (minute() / 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
    case 3:
      shiftOut(D5, D6, MSBFIRST, harom);
      break;
    case 4:
      shiftOut(D5, D6, MSBFIRST, negy);
      break;
    case 5:
      shiftOut(D5, D6, MSBFIRST, ot);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, otodik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  digitalWrite(D7, LOW);
  //Perc egyesek
  switch (minute() % 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
    case 3:
      shiftOut(D5, D6, MSBFIRST, harom);
      break;
    case 4:
      shiftOut(D5, D6, MSBFIRST, negy);
      break;
    case 5:
      shiftOut(D5, D6, MSBFIRST, ot);
      break;
    case 6:
      shiftOut(D5, D6, MSBFIRST, hat);
      break;
    case 7:
      shiftOut(D5, D6, MSBFIRST, het);
      break;
    case 8:
      shiftOut(D5, D6, MSBFIRST, nyolc);
      break;
    case 9:
      shiftOut(D5, D6, MSBFIRST, kilenc);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, negyedik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  digitalWrite(D7, LOW);
  //Másodperc tizesek
  switch (second() / 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
    case 3:
      shiftOut(D5, D6, MSBFIRST, harom);
      break;
    case 4:
      shiftOut(D5, D6, MSBFIRST, negy);
      break;
    case 5:
      shiftOut(D5, D6, MSBFIRST, ot);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, masodik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  digitalWrite(D7, LOW);
  //Másodperc egyesek
  switch (second() % 10)
  {
    case 0:
      shiftOut(D5, D6, MSBFIRST, nulla);
      break;
    case 1:
      shiftOut(D5, D6, MSBFIRST, egy);
      break;
    case 2:
      shiftOut(D5, D6, MSBFIRST, ketto);
      break;
    case 3:
      shiftOut(D5, D6, MSBFIRST, harom);
      break;
    case 4:
      shiftOut(D5, D6, MSBFIRST, negy);
      break;
    case 5:
      shiftOut(D5, D6, MSBFIRST, ot);
      break;
    case 6:
      shiftOut(D5, D6, MSBFIRST, hat);
      break;
    case 7:
      shiftOut(D5, D6, MSBFIRST, het);
      break;
    case 8:
      shiftOut(D5, D6, MSBFIRST, nyolc);
      break;
    case 9:
      shiftOut(D5, D6, MSBFIRST, kilenc);
      break;
  }
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, elso);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
  //elválasztás
  digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, vesszo);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, hatodik + harmadik);
  digitalWrite(D7, HIGH);
  delayMicroseconds(2000);
}

void configModeCallback (WiFiManager * myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

void getTime(void)
{
  getNtpTime();
}

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress & address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
