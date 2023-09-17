#include <SPI.h>
#include <Stepper.h>
#include<Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define stepp 32

#define IN1  8
#define IN2  7
#define IN3  6
#define IN4  4

int  x = 0;

Stepper steps(stepp, IN4, IN2, IN3, IN1);

const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

Servo channel_1;
Servo channel_2;
Servo channel_3;
Servo channel_4;
Servo channel_5;
Servo channel_6;
Servo channel_7;

int ch1_value = 0;
int ch2_value = 0;
int ch3_value = 0;
int ch4_value = 0;
int ch5_value = 0;
int ch6_value = 0;
int ch7_value = 0;

struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Received_data received_data;

void reset_the_Data() 
{
  
  received_data.ch1 = 0;
  received_data.ch2 = 127;
  received_data.ch3 = 127;
  received_data.ch4 = 127;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;
}


void setup()
{
  Wire.begin(); 
  channel_1.attach(2);
  channel_2.attach(3);
  channel_3.attach(4);
  channel_4.attach(5);
  channel_5.attach(6);
  channel_6.attach(7);
  channel_7.attach(8);
  
  reset_the_Data();

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  radio.startListening();

}


unsigned long lastRecvTime = 0;

void receive_the_data()
{
  while ( radio.available() ) {
  radio.read(&received_data, sizeof(Received_data));
  lastRecvTime = millis(); 
}
}

void loop()
{

  receive_the_data();
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    
    reset_the_Data();
    
  } 

  
  ch1_value = map(received_data.ch1,0,255,1000,2000);
  ch2_value = map(received_data.ch2,0,255,1000,2000);
  // ch3_value = map(received_data.ch3,0,255,1000,2000);
  ch4_value = map(received_data.ch4,0,255,1000,2000);
  // ch5_value = map(received_data.ch5,0,1,1000,2000);
  // ch6_value = map(received_data.ch6,0,1,1000,2000);
  // ch7_value = map(received_data.ch7,0,255,1000,2000);

  channel_1.writeMicroseconds(ch1_value);  
  channel_2.writeMicroseconds(ch2_value);  
  // channel_3.writeMicroseconds(ch3_value);  
  channel_4.writeMicroseconds(ch4_value);  
  // channel_5.writeMicroseconds(ch5_value);  
  // channel_6.writeMicroseconds(ch6_value);  
  // channel_7.writeMicroseconds(ch7_value);  

  ch3_value = map(received_data.ch3,0,255,0,1);
  ch5_value = map(received_data.ch5,0,1,0,1);
  ch6_value = map(received_data.ch6,0,1,0,1);
  ch7_value = map(received_data.ch7,0,255,0,1);

  digitalWrite(IN1, ch3_value);
  digitalWrite(IN2, ch5_value);
  digitalWrite(IN3, ch6_value);
  digitalWrite(IN4, ch7_value);

  if(ch3_value || ch5_value || ch6_value || ch7_value) {
    steps.step(1);
  }
}