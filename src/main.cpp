#include <Arduino.h>

void setup() {
Serial1.begin(115200);
Serial.begin(115200);
}
float kp=0.0;float ki=0.0;float kd=0.0;
float ep=0.0;float ei=0.0;float ed=0.0;
float pos=6.0;
int s[]={0,0,0,0,0,0,0,0,0,0,0,0};
void updateK(){
  if (Serial1.available()>0) {
    String input=Serial1.readStringUntil("\n");
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
  float preve=pos-6.00;
  pos=pos + float(random(0,20)-10)/10;
  if(pos>12 or pos<0)
  pos=6.00;
  float e=pos-6.00;
  ep=e*kp;
  ed=(e-preve)*kd;
  for(int i=0;i<12;i++){
    s[i]=random(0,255);
  }

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
  msg+=(s[5]);
  msg+=(F(",\"S6\":"));
  msg+=(s[6]);
  msg+=(F(",\"S7\":"));
  msg+=(s[7]);
  msg+=(F(",\"S8\":"));
  msg+=(s[8]);
  msg+=(F(",\"S9\":"));
  msg+=(s[9]);
  msg+=(F(",\"S10\":"));
  msg+=(s[10]);
  msg+=("}\n");
  Serial1.print(msg);
}
unsigned long time=0;
void loop() {
updateK();
dataAleatoria();
if(time<(millis()-50)){
  sendState();
time=millis();
}
}
