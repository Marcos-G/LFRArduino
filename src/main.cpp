#include <Arduino.h>


void setearTimer50htz(){
  noInterrupts(); // disable all interrupts
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 1250; // compare match register 16MHz/256/50Hz (20016micros)
  TCCR3B |= (1 << WGM32); // CTC mode
  TCCR3B |= (1 << CS32 ); // 256 prescaler
  TIMSK3 |= (1 << OCIE3A); // enable timer compare interrupt
  interrupts();
}
void setearTimer20htz(){
  noInterrupts(); // disable all interrupts
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4 = 0;
  OCR4A = 3125; // compare match register 16MHz/256/20Hz (20016micros)
  TCCR4B |= (1 << WGM42); // CTC mode
  TCCR4B |= (1 << CS42 ); // 256 prescaler
  TIMSK4 |= (1 << OCIE4A); // enable timer compare interrupt
  interrupts();
}
long tiempo=0;

float kp=0.0;float ki=0.0;float kd=0.0;
float ep=0.0;float ei=0.0;float ed=0.0;
float pos=6.0;
float preve=0.0;
int min=1024;
int max=0;
float setpoint=6.0;
int s[]={0,0,0,0,0,0};
float posSen[]={1.2,3.1,4.7,6.4,8.0,10.0};
void updateK(){
  if (Serial1.available()>0) {
    String input=Serial1.readStringUntil('\n');
    char k=input[0];
    float val=input.substring(1).toFloat();
    if(k=='P'){
      kp=val;
    }
    else if(k=='I'){
      ki=val;
    }
    else if(k=='D'){
      kd=val;
    }
  }
}
void dataAleatoria(){
  float prom=0;
  float sum=0;
  for(int a=0;a<6;a++){
    s[a]=(analogRead(a)-min)*255.0/(max*4.0);
    //s[a]=(analogRead(a))/4;
    prom+=s[a]*posSen[a];
    sum+=s[a];
  }
  pos=prom/sum;
  float e=pos-setpoint;
  ep=e*kp;
  ed=(e-preve)*kd;
  preve=e;

}
void sendState(){
  String msg="{";

  msg+=(F("\"KP\":"));
  msg+=(kp);
  msg+=(F(",\"KI\":"));
  msg+=(ki);
  msg+=(F(",\"KD\":"));
  msg+=(kd);
  msg+=(",");
  msg+=(F("\"POS\":"));
  msg+=(pos);
  msg+=(F(",\"EP\":"));
  msg+=(ep);
  msg+=(F(",\"EI\":"));
  msg+=(ei);
  msg+=(F(",\"ED\":"));
  msg+=(ed);
  msg+=(F(",\"S0\":"));
  msg+=(s[0]);
  msg+=(F(",\"S1\":"));
  msg+=(s[1]);
  msg+=(F(",\"S2\":"));
  msg+=(s[2]);
  msg+=(F(",\"S3\":"));
  msg+=(s[3]);
  msg+=(F(",\"S4\":"));
  msg+=(s[4]);
  msg+=(F(",\"S5\":"));
  msg+=(s[5]);/*
  msg+=(F(",\"S6\":"));
  msg+=(s[6]);
  msg+=(F(",\"S7\":"));
  msg+=(s[7]);
  msg+=(F(",\"S8\":"));
  msg+=(s[8]);
  msg+=(F(",\"S9\":"));
  msg+=(s[9]);
  msg+=(F(",\"S10\":"));
  msg+=(s[10]);*/
  msg+=("}\n");
  Serial1.print(msg);
}
ISR(TIMER3_COMPA_vect) // timer compare interrupt service routine
{
  dataAleatoria();
}
ISR(TIMER4_COMPA_vect) // timer compare interrupt service routine
{
  sendState();
}
void setup() {
Serial1.begin(115200);
Serial.begin(115200);
setearTimer50htz();
setearTimer20htz();
}
void loop() {
  if(millis()<10000){
    for(int a=0;a<6;a++){
      int sen=analogRead(a)/4;
      if(sen>max){
        max=sen;
      }
      if(sen<min){
        min=sen;
      }
    }
  }
updateK();
//dataAleatoria();

}
