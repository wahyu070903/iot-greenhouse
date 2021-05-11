#include<SoftwareSerial.h>
#include<ArduinoJson.h>
SoftwareSerial UART (12,13);
//stepper lifting
#define PUL_LIFT 4
#define DIR_LIFT 3
#define EN_LIFT 2
//=======================
#define PUL_TRACTION 7
#define DIR_TRACTION 6
#define EN_TRACTION 5
//#define start_limit 8
//#define end_limit 9
#define LDR_pin1  A0
#define LDR_pin2  A1
//======motor1&2======
#define enA 8
#define enB 9
#define inp1 22
#define inp2 23
#define inp3 24
#define inp4 25
#define relay1 26
#define relay2 27
int ary[3] = {0,0,0};
int ary2[3] = {0,0,0};
//====================
int waktu_siram = 10;//60detik 1 menit
int batas_siap = 8;//50 detik
int ValLDR1 = 0;
int ValLDR2 = 0;
int captured_data;
int Valmode,Valanalog1,Valanalog2,Valanalog3,Valanalog4,Valsemprot,Vallampu;
int count = 0 ; 
int difu1 = 0;
int difu2 = 0;
int difumanual1 = 0;
int difumanual2 = 0;
int motormanualcount1 = 0;
int motormanualcount2 = 0;
int motorcount1 = 0;
int motorcount2 = 0;
int countsebelumsiram = 0;
int sudahcount = 0;
//---------program bagian------------
void naik(int x);
void turun(int x);
void maju(int x);
void mundur(int x);
void siram();
void analysis_Data();
void pengunci();
void sensor_cahaya();
void motor1CW();
void motor1CCW();
void motor2CW();
void motor2CCW();
void countersiram();
//---------------------------
void setup()
{
  Serial.begin(9600);
  UART.begin(9600);
  pinMode(PUL_LIFT,OUTPUT);
  pinMode(DIR_LIFT,OUTPUT);
  pinMode(EN_LIFT,OUTPUT);
  //..........................
  pinMode(PUL_TRACTION,OUTPUT);
  pinMode(DIR_TRACTION,OUTPUT);
  pinMode(EN_TRACTION,OUTPUT);
  digitalWrite(EN_LIFT,LOW);
  digitalWrite(EN_TRACTION,LOW);
  //=============================
  //=============================
  //pinMode(start_limit,INPUT);
  //pinMode(end_limit,INPUT);
  //==============================
  pinMode(LDR_pin1,INPUT);
  pinMode(LDR_pin2,INPUT);
  //================================
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(inp1,OUTPUT);
  pinMode(inp2,OUTPUT);
  pinMode(inp3,OUTPUT);
  pinMode(inp4,OUTPUT);

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
}
void loop()
{
  select_Mode();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(UART);
  if (root == JsonObject::invalid())
    return;
  root.prettyPrintTo(Serial);
  Valmode = root["mode"];
  Valanalog1 = root["analog1"];
  Valanalog2 = root["analog2"];
  Valanalog3 = root["motor1"];
  Valanalog4 = root["motor2"];
  Valsemprot = root["semprot"];
  Vallampu   = root["lampu"];
//  select_Mode();
  
}
void read_Data()
{
  
}
void select_Mode()
{
  if(Valmode == 0)
  {
    analysis_Data();
    count = 0; // reset data pada counter
  }
  else if(Valmode == 1)
  {
    count++;
    //delay(500);
    Serial.print("count : ");
    Serial.print(count);
    Serial.println(" ");
    //sensor_cahaya();
    if(count >= waktu_siram)
    {
      siram();
      count = 0;
    }
  }
  
}


void naik(int x)
{
  UART.end(); //menutup sementara serial untuk menghindari stepper tersandat akibat aliran data
  digitalWrite(EN_LIFT,LOW);
  digitalWrite(DIR_LIFT,HIGH);
  for(int a=0;a<x;a++)
  {
    digitalWrite(PUL_LIFT,HIGH);
    delayMicroseconds(500);
    digitalWrite(PUL_LIFT,LOW);
    delayMicroseconds(500);
  }
  digitalWrite(EN_LIFT,HIGH);
  UART.begin(9600);
}
void turun(int x)
{
  UART.end();
  digitalWrite(EN_LIFT,LOW);
  digitalWrite(DIR_LIFT,LOW);
  for(int a=0;a<x;a++)
  {
    digitalWrite(PUL_LIFT,HIGH);
    delayMicroseconds(500);
    digitalWrite(PUL_LIFT,LOW);
    delayMicroseconds(500);
  }
  digitalWrite(EN_LIFT,HIGH);
  UART.begin(9600);
} 
void maju(int x)
{
  UART.end();//menutup sementara serial untuk menghindari stepper tersandat akibat aliran data
  digitalWrite(EN_TRACTION,LOW);
  digitalWrite(DIR_TRACTION,HIGH);
  for(int a=0;a<=x;a++) //Resolusi 200x10 putaran
  {
    digitalWrite(PUL_TRACTION,255);
    delayMicroseconds(1000);
    digitalWrite(PUL_TRACTION,0);
    delayMicroseconds(1000);
  }
  digitalWrite(EN_TRACTION,HIGH);
  UART.begin(9600);//membuka kembali komunikasi serial
}
void mundur(int x)
{
  UART.end();
  digitalWrite(EN_TRACTION,LOW);
  digitalWrite(DIR_TRACTION,LOW);
  for(int a=0;a<=x;a++)//Resolusi 200x10 putaran
  {
    digitalWrite(PUL_TRACTION,255);
    delayMicroseconds(1000);
    digitalWrite(PUL_TRACTION,0);
    delayMicroseconds(1000); 
  }
  digitalWrite(EN_TRACTION,HIGH);
  UART.begin(9600);
}
void siram()
{
// if(countsebelumsiram == 0)
// {
 turun(4000);
 semprot(1);
 //}
// if(sudahcoun/t == 0)
// {
//  countersiram();
// }
// if(sudahcount == 1)
// {
 delay(4000);
 maju(3800);
 semprot(0);
 naik(4000);
 mundur(3800);
  //countsebelumsiram = 0;
  //sudahcount = 0;
// }
}
void analysis_Data()
{
  //=========penting slidder traction============
  ary[1] = ary[0];
  ary[0] = Valanalog1;
  ary[2] = ary[0] - ary[1];
  //=============================================
  //..........penting slidder lifting............
  ary2[1] = ary2[0];
  ary2[0] = Valanalog2;
  ary2[2] = ary2[0] - ary2[1];
  //..............................................
  if(ary[2] < 0)
  {
    mundur(abs(ary[2]));
  }

  if(ary[2] > 0)
  {
    maju(ary[2]);
  }
   if(ary2[2] < 0)
  {
    turun(abs(ary2[2]));
  }

  if(ary2[2] > 0)
  {
    naik(ary2[2]);
  }

 //.....................motor...................................
  if((Valanalog3 == 1)&&(difumanual1 == 0))
  {
    motormanualcount1++;
    motor1CCW(); //membuka penutup
    if(motormanualcount1 == 7)
    {
      digitalWrite(enA,0);
      motormanualcount1 = 0;
      difumanual1 = 1;
    }
  }
  if((Valanalog3 == 0)&&(difumanual1 == 1))
  {
    motormanualcount1++;
    motor1CW(); //menutup penutup
    if(motormanualcount1 == 4)
    {
      digitalWrite(enA,0);
      motormanualcount1 = 0;
      difumanual1 = 0;
    }
  }

  if((Valanalog4 == 1)&&(difumanual2 == 0))
  {
    motormanualcount2++;
    motor2CCW(); //membuka penutup
    if(motormanualcount2 == 6)
    {
      digitalWrite(enB,0);
      motormanualcount2 = 0;
      difumanual2 = 1;
    }
  }
  if((Valanalog4 == 0)&&(difumanual2 == 1))
  {
    motormanualcount2++;
    motor2CW(); //menutup penutup
    if(motormanualcount2 == 3)
    {
      digitalWrite(enB,0);
      motormanualcount2 = 0;
      difumanual2 = 0;
    }
  }
 semprot(Valsemprot);//menyemprot manual
 lampu(Vallampu);//menghidupkan lampu manual
//.............................................................................
}
void sensor_cahaya()
{
  ValLDR1 = analogRead(LDR_pin1);
  ValLDR2 = analogRead(LDR_pin2);
  Serial.print("LDR 1 Value = ");
  Serial.print(ValLDR1);
  Serial.println(" ");
  //=====================================================
  Serial.print("LDR 2 Value = ");
  Serial.print(ValLDR2);
  Serial.println(" ");
  digitalWrite(enA,0);
  digitalWrite(enB,0);
  //=======================================
  if((ValLDR1 > 300)&&(difu1==0)&&(count <= batas_siap))
  {
    motorcount1++;
    Serial.print("ini count motor 1 :");
    Serial.print(motorcount1);
    Serial.println(" ");
    motor1CW();
    if(motorcount1 == 3)
    {
      digitalWrite(enA,0);
      difu1 = 1;
      motorcount1 = 0;
    }
  }
  if((ValLDR1 < 300 )&&(difu1==1)&&(count <= batas_siap))
  {
    motorcount1++;
    Serial.print("ini count motor 1 :");
    Serial.print(motorcount1);
    Serial.println(" ");
    motor1CCW();
    if(motorcount1 == 3)
    {
      digitalWrite(enA,0);
      difu1 = 0;
      motorcount1 = 0;
    }
  }
  //**************************************************************************
  if((ValLDR2 > 300)&&(difu2==0)&&(count <= batas_siap))
  {
    motorcount2++;
    Serial.print("ini count motor 2 :");
    Serial.print(motorcount2);
    Serial.println(" ");
    motor2CW();
    if(motorcount2 == 3)
    {
      digitalWrite(enB,0);
      difu2 = 1;
      motorcount2 = 0;
    }
  }
  if((ValLDR2 < 300 )&&(difu2==1)&&(count <= batas_siap))
  {
    motorcount2++;
    Serial.print("ini count motor 2 :");
    Serial.print(motorcount2);
    Serial.println(" ");
    motor2CCW();
    if(motorcount2 == 3)
    {
      digitalWrite(enB,0);
      difu2 = 0;
      motorcount2 = 0;
    }
  }
  //***************************************************************************
}
void motor1CW()
{
  digitalWrite(enA,255);
  digitalWrite(inp1,HIGH);
  digitalWrite(inp2,LOW);
}
void motor1CCW()
{
  digitalWrite(enA,255);
  digitalWrite(inp1,LOW);
  digitalWrite(inp2,HIGH);
}
void motor2CW()
{
  digitalWrite(enB,255);
  digitalWrite(inp3,HIGH);
  digitalWrite(inp4,LOW);
}
void motor2CCW()
{
  digitalWrite(enB,255);
  digitalWrite(inp3,LOW);
  digitalWrite(inp4,HIGH);
}
void semprot(int x)
{
  digitalWrite(relay1,x);
}
void lampu(int x)
{
  digitalWrite(relay2,x);
}
void countersiram()
{
  countsebelumsiram++;
  if(countsebelumsiram == 2)
  {
    sudahcount = 1;
  }
}
