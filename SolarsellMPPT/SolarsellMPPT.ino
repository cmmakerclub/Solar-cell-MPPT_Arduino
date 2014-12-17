/*Sola sell MPPT
*by BANHAN
*
*/

const int senAmpInPin = A0;  // Analog input pin that the potentiometer is attached to
const int senVoltInPin = A1;  // Analog input pin
const int analogOutPin = 11; // Analog output pin control mosfet

float valueAmp = 0.0;        // value read from the pot
float valueVolt = 0.0;        // value read from the pot
float watt = 0.0; //Power = V*A

byte outputValue = 0;        // value output to the PWM (analog out)

float deltaVolt = 0.0;
float deltaAmp = 0.0;
float deltaWatt = 0.0;

unsigned long timeMer = 0;//millis();

float preWatt = 0.0;
float nowWatt = 1.0;

int valuePwm = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  //เช็คเดลต้าV Amp 
  deltaVolt = getVolt();
  deltaAmp = getAmp();
  //เริ่ม MPPT
  if(deltaVolt > 0){
     deltaWatt = getWatt();
     //เก็บค่าwatt 
     if(timeMer % 2 == 0){
         nowWatt = deltaWatt;//ตอนนี้
     }else{
         preWatt = deltaWatt;//ก่อนหน้า
     }
     //ถ้าwattตอนนี้มากกว่าให้เพิ่ม%PWM
     if(nowWatt > preWatt){
         outputValue++;
     }else{//ถ้าwattก่อนหน้ามากกว่าให้ลด%PWM
         outputValue--;
     }
  }else{//ถ้าแรงดันเป็น0แต่มีกระแสใหลแสดงว่ามีการซ๊อตต้องลด%pwm
      if(deltaAmp >0){
        outputValue--;
      }
  }
  //สั่งPWMทำงาน
  analogWrite(analogOutPin,outputValue);
  //debug
  valuePwm = map(outputValue, 0, 255, 0,100);
  // print the results to the serial monitor:
  Serial.print("Volt = " );                       
  Serial.print(deltaVolt);      
  Serial.print("\t Amp = ");      
  Serial.print(deltaAmp); 
Serial.print("\t Watt = ");      
  Serial.print(deltaWatt);  
  Serial.print("\t % PWM = ");      
  Serial.println(valuePwm); 
}

public float getWatt(){
  // read the analog in value:
  valueAmp = analogRead(senAmpInPin);
  valueVolt = analogRead(senVoltInPin); 
 // แปลงเป็น V และ Amp
 //เช็นเชอร์อ่านกระแสได้สูงสุด 5A และ 1A จะอ่านเป็นดิจิตอลได้ 204.6
 valueAmp = valueAmp / 204.6;
  //เช็นเชอร์จะอ่านค่าVได้21V ดังนั้นค่าดิจิตอล 1V = 1023/21 = 48.71
  valueVolt = valueVolt / 48.71;
  //หาค่าWจากสูตร W=VxI
  watt = valueAmp*valueVolt;
  return watt;
}

public float getVolt(){
  // read the analog in value:
  //valueAmp = analogRead(senAmpInPin);
  valueVolt = analogRead(senVoltInPin); 
  //เช็นเชอร์จะอ่านค่าVได้21V ดังนั้นค่าดิจิตอล 1V = 1023/21 = 48.71
  valueVolt = valueVolt / 48.71;
  
  return valueVolt;
  
}

public float getAmp(){
  // read the analog in value:
  valueAmp = analogRead(senAmpInPin);
  //valueVolt = analogRead(senVoltInPin); 
  //เช็นเชอร์อ่านกระแสได้สูงสุด 5A และ 1A จะอ่านเป็นดิจิตอลได้ 204.6
 valueAmp = valueAmp / 204.6;
 
 return valueAmp;
}

