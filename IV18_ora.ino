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

union adat {
  byte karakter[4];
  unsigned long kimenet;
};

adat ora;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D7, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
}

void loop() {

  //digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, egy);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, elso);
  digitalWrite(D7, HIGH);
  delay(1);
  digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, harom);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, masodik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, pont);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, harmadik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, ot);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, negyedik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, kilenc);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, otodik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, pont);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, hatodik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, kilenc);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, hetedik);
  digitalWrite(D7, HIGH);
  delay(1);
    digitalWrite(D7, LOW);
  shiftOut(D5, D6, MSBFIRST, egy);
  shiftOut(D5, D6, MSBFIRST, 0b00000000);
  shiftOut(D5, D6, MSBFIRST, nyolcadik);
  digitalWrite(D7, HIGH);
  delay(1);


  // put your main code here, to run repeatedly:
  /* while (Serial.available() > 0) {
     ora.karakter[0] = Serial.read();
     switch (ora.karakter[0])
     {
       case '0':
         shiftOut(D5, D6, LSBFIRST, nulla);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '1':
         shiftOut(D5, D6, LSBFIRST, egy);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '2':
         shiftOut(D5, D6, LSBFIRST, ketto);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '3':
         shiftOut(D5, D6, LSBFIRST, harom);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '4':
         shiftOut(D5, D6, LSBFIRST, negy);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '5':
         shiftOut(D5, D6, LSBFIRST, ot);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '6':
         shiftOut(D5, D6, LSBFIRST, hat);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '7':
         shiftOut(D5, D6, LSBFIRST, het);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '8':
         shiftOut(D5, D6, LSBFIRST, nyolc);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case '9':
         shiftOut(D5, D6, LSBFIRST, kilenc);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
       case 'd':
         shiftOut(D5, D6, LSBFIRST, 128);
         digitalWrite(D7, LOW);
         delayMicroseconds(10);
         digitalWrite(D7, HIGH);
         break;
     }
    }
  */
}
