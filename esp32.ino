#include<ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#define soil_pin A0
char auth[] = "PzjDY7RAYwfGFzrOqw9viL7-87qaReQm";
char ssid[] = "localhost";
char pass[] = "nanikore";
SoftwareSerial UART (D7,D8);
int pinValueV0;
int pinValueV1;
int pinValueV2;
int pinValueV3;
int pinValueV4;
//============================================
int soil_value;
//''''''''''unicaode siram manual'''''''''''''
int unicode_manualMode = 999;
int unicode_autoMode = 998;
int unicode_manualNaik = 100;
int unicode_manualTurun = 200;
int unicode_manualMaju = 300;
int unicode_manualMundur = 400;
//''''''''''''`````````````````````````````

void manual_mode();
//void read_data();
void setup()
{
  Serial.begin(9600);
  UART.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(soil_pin,INPUT);
  StaticJsonDocument<200> doc;
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;
  serializeJsonPretty(doc, Serial);
}

 BLYNK_WRITE(V0)
{
  pinValueV0 = param.asInt();
  Serial.print("Nilai pin Virtual V0 :");
  Serial.println(pinValueV0);
 /* if(pinValueV0 == 0) //diletakkan disini agar hanya berjalan 1scantime
  {
    UART.println(unicode_manualMode);
    Serial.println("modemanual aktif");
  }
   if(pinValueV0 == 1) //diletakkan disini agar hanya berjalan 1scantime
  {
    UART.println(unicode_autoMode);
    Serial.println("modemanual mati");
  }*/
}
 BLYNK_WRITE(V1)
{
  pinValueV1 = param.asInt();
  Serial.print("Nilai pin Virtual V1 :");
  Serial.println(pinValueV1);
}
BLYNK_WRITE(V2)
{
  pinValueV2 = param.asInt();
  Serial.print("Nilai pin Virtual V2 :");
  Serial.println(pinValueV2);
}BLYNK_WRITE(V3)
{
  pinValueV3 = param.asInt();
  Serial.print("Nilai pin Virtual V3 :");
  Serial.println(pinValueV3);
}
BLYNK_WRITE(V4)
{
  pinValueV4 = param.asInt();
  Serial.print("Nilai pin Virtual V4 :");
  Serial.println(pinValueV4);
}
BLYNK_READ(V5)
{
  soil_value = analogRead(soil_pin);
  Blynk.virtualWrite(V5,soil_value);}
void loop()
{
  if(pinValueV0 == 0) //diletakkan disini agar hanya berjalan 1scantime
  {
    UART.println(unicode_manualMode);
    Serial.println("modemanual aktif");
    manual_mode();
  }
   if(pinValueV0 == 1) //diletakkan disini agar hanya berjalan 1scantime
  {
    UART.println(unicode_autoMode);
    Serial.println("modemanual mati");
  }
  
  //if(pinValueV0 == 0)
  //{
   //manual_mode();
   //delay(1000);
  //}
  //read_data();
  Blynk.run();
  
}
void manual_mode()
{
  
  if((pinValueV0 == 0)&&(pinValueV1 == 1))
  {
    Serial.println("mode naik");
    UART.println(int(unicode_manualNaik));
  }
  if((pinValueV0 == 0)&&(pinValueV2 == 1))
  {
    Serial.println("mode turun");
    UART.println(int(unicode_manualTurun));
  }
  if((pinValueV0 == 0)&&(pinValueV3 == 1))
  {
    Serial.println("mode maju");
    UART.println(int(unicode_manualMaju));
  }
  if((pinValueV0 == 0)&&(pinValueV4 == 1))
  {
    Serial.println("mode mundur");
    UART.println(int(unicode_manualMundur));
  }
}
/*void read_data()
{
  soil_value = UART.parseInt();
  Serial.print("incom data : ");
  Serial.print(soil_value);
  Serial.println(" ");
  if(pinValueV0 == 0)
  {
  delay(1);
  }
  else
  {
  delay(5000);    
  }

  
}*/
