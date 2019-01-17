/*
   * Wire - I2C Scanner
   * Copyright (C) 2018 Rexenor Chin 2019-01-17
   * Sketch provides simple command via serial interface
   * ============================================
   Command list
   set sda=PIN
   set scl=PIN
   set rst=PIN
   reset
   test
   * ============================================
   Default setting
   The WeMos D1 Mini I2C bus uses pins:
   D1 = SCL
   D2 = SDA
   test function based on http://playground.arduino.cc/Main/I2cScanner
*/
#include <Wire.h>
 int sclPin = D1;
 int sdaPin = D2;
 int rstPin = 0;
void setup()
{
  Serial.begin(115200);
  Serial.println("I2C Scanner");
  serialcommand();
}
void serialcommand(){
  Serial.println("Title:I2C Scanner\nVersion:5\n \nCommand list\n\nset sda=PIN\nset scl=PIN\nset rst=PIN\nreset\ntest");
}
void loop() {
  //while (!Serial.available());
  String data="";
  while (Serial.available()) {
    data=Serial.readString(); 
    //data+=String(c,DEC);
  }
  if(data!=""){
    String Fset="set ";
    String Freset="reset\n";
    String Ftest="test\n";
    //Serial.println(String(data));
    if(data.indexOf(Fset)>= 0){
      Serial.println("Setting Pin: SDA, SCL, Reset");
      String temp;
      //checking SDA
      if(data.indexOf("sda=")>= 0){
        temp = midString(data,"sda=","\n");
        if(temp!="-1"){
          Serial.print("SDA @pin:");
          sdaPin=temp.toInt();
          Serial.println(sdaPin);
          set();
        }
      }
      //checking SCL
      if(data.indexOf("scl=")>= 0){
        temp = midString(data,"scl=","\n");
        if(temp!="-1"){
          Serial.print("SCL @pin:");
          sclPin=temp.toInt();
          Serial.println(sclPin);
          set();
        }
      }
      //checking Reset
      if(data.indexOf("rst=")>= 0){
        temp = midString(data,"rst=","\n");
        if(temp!="-1"){
          Serial.print("SDA @pin:");
          rstPin=temp.toInt();
          Serial.println(rstPin);
          set();
        }
      }
      
    }
    if(data.indexOf(Freset)>= 0){
      Serial.println("Resetting RFID module");
      reset();
    }
    if(data.indexOf(Ftest)>= 0){
      Serial.println("Testing RFID module");
      TestI2C();
    }

  }
  data="";
  Serial.flush();
}
void set(){
  Wire.begin(sdaPin, sclPin);
  pinMode(rstPin, OUTPUT);
  digitalWrite(rstPin, 1);
}
void reset(){
  digitalWrite(rstPin, 0);
  delay(500);
  digitalWrite(rstPin, 1);
}

void TestI2C()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");
      Serial.println(address);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("Done.\n");
  }

  delay(2000);
}
String midString(String str, String start, String finish){
  int locStart = str.indexOf(start);
  if (locStart==-1) return "";
  locStart += start.length();
  int locFinish = str.indexOf(finish, locStart);
  if (locFinish==-1) return "";
  return str.substring(locStart, locFinish);
}
