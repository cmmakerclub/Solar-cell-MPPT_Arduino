/*Sola sell MPPT
*by BANHAN
*
*/

const int senAmpInPin = A0;  // Analog input pin that the potentiometer is attached to
const int senVoltInPin = A1;  // Analog input pin
const int analogOutPin = 9; // Analog output pin control mosfet

float valueAmp = 0;        // value read from the pot
float valueVolt = 0;        // value read from the pot
float watt = 0; //Power = V*A

byte outputValue = 5;        // value output to the PWM (analog out)

float deltaVolt = 0;
float deltaAmp = 0;
float deltaWatt = 0;

unsigned long timeMer = 0;//millis();

float preWatt = 0;
float nowWatt = 1;

int valuePwm = 0;

float gianP = 0;
float gianM = 0;
float solarVolt = 17.5;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  //
  //---------------------------------------------- Set PWM frequency for D9 & D10 ------------------------------
 
TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
//  TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
//TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
}

void loop() {
  timeMer = millis();
  //check V Amp 
  deltaVolt = getVolt();
  deltaAmp = getAmp();
  //start MPPT
  if(deltaVolt > 5.9){
     deltaWatt = getWatt();
     //get preious watt 
     
        
     
         preWatt = deltaWatt;//
         delayMicroseconds(1);
         //get now watt
         deltaWatt = getWatt();
          nowWatt = deltaWatt;//
     //decalr gian
     if(deltaVolt > solarVolt*0.825){
         gianP = 1.2;
         gianM = 1;
     }else{
         gianP = 1;
         gianM = 1.05;
     }
     //Pk > Pk-1
     if(nowWatt >= preWatt ){
       //gianP = 1.1;
         //gianM = 1;
       
         outputValue+=gianP;
         if(outputValue >=254){
            outputValue = 254;  
         }
     }else if(preWatt > nowWatt ){//Pk-1 >Pk
         //gianP =1;
         //gianM = 1.1;
         //
           outputValue-=gianM;//
           if(outputValue <= 1){
            outputValue = 1;  
         }
         
     }
  }else{//Short
      if(deltaAmp >0){
        outputValue = outputValue*0.9;
        Serial.println("666666666666666666666666666666666666666666666666666666666");
      }
  }
  //run PWM
  //outputValue = 0;
  analogWrite(analogOutPin,outputValue);
 // analogWrite(analogOutPin,10);
  //debug
  
  valuePwm = map(outputValue, 0, 255, 0,100);
  // print the results to the serial monitor:
  if(timeMer%2 == 0){
  Serial.print("Volt = " );                       
  Serial.print(deltaVolt);      
  Serial.print("V\t Amp = ");      
  Serial.print(deltaAmp); 
Serial.print("mA\t Watt = ");      
  Serial.print(deltaWatt);  
  Serial.print("W\t PWM = ");      
  Serial.print(valuePwm);
 Serial.println("%");  
  }
 
}

 float getWatt(){
  // read the analog in value:
  valueAmp = analogRead(senAmpInPin);
  valueVolt = analogRead(senVoltInPin); 
 //Current
 valueAmp = (valueAmp/204.6);
  //เช็นเชอร์จะอ่านค่าVมากสุดได้21V 
  valueVolt = (valueVolt /204.6)*5.5;
  //หาค่าWจากสูตร W=VxI
  watt = valueAmp*valueVolt;
  return watt;
}

 float getVolt(){
  // read the analog in value:
  //valueAmp = analogRead(senAmpInPin);
  valueVolt = analogRead(senVoltInPin); 
  //เช็นเชอร์จะอ่านค่าVมากสุดได้21V 
  valueVolt = (valueVolt /204.6)*5.5;
  
  return valueVolt;
  
}

 float getAmp(){
  // read the analog in value:
  valueAmp = analogRead(senAmpInPin);
  //valueVolt = analogRead(senVoltInPin); 
  //เช็นเชอร์อ่านกระแสได้สูงสุด 5A และ 1A จะอ่านเป็นดิจิตอลได้ 204.6
 valueAmp = (valueAmp/204.6);
 
 return valueAmp*1000;
}

