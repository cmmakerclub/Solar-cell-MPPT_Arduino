/*Sola sell MPPT
*by BANHAN
*
*/

const int senAmpInPin = A0;  // Analog input pin that the potentiometer is attached to
const int senVoltInPin = A1;  // Analog input pin
const int analogOutPin = 9; // Analog output pin control mosfet
const int analogOutPin2 = 10; // Analog output pin control mosfet

float valueAmp = 0;        // value read from the pot
float valueVolt = 0;        // value read from the pot
float watt = 0; //Power = V*A

byte outputValue = 255;        // value output to the PWM (analog out)

float deltaVolt = 0;
float deltaAmp = 0;
float deltaWatt = 0;

unsigned long timeMer = 0;//millis();

float preWatt = 0;
float nowWatt = 1;

float preWatt1mS = 0;
float nowWatt1mS = 1;

long count = 0;

int valuePwm = 0;

float gianP = 1;
float gianM = 1;
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
  analogWrite(analogOutPin2,255);
  //timeMer = millis();
  //check V Amp 
  deltaVolt = getVolt();
  deltaAmp = getAmp();
  //start MPPT
  if(deltaVolt > 6){
     deltaWatt = getWatt();
     //get preious watt 
     
        
     
         preWatt = deltaWatt;//
         delayMicroseconds(33);
        //delay(1);
         //get now watt
         deltaWatt = getWatt();
          nowWatt = deltaWatt;//
           /*if(count == 0){
                preWatt1mS = preWatt;
                Serial.println(preWatt1mS);
           }
          count++;
          if(timeMer%5000000 == 0){
              Serial.println("____________________timeMer%500000 == 0______");
             nowWatt1mS = nowWatt;
            reset_millis();
            if(nowWatt1mS < preWatt1mS){
              outputValue = outputValue*0.5;
              count = 0;
              Serial.println("____________nowWatt1mS < preWatt1mS______________");
            } 
          }*/
          
    /* //decalr gian
     if(deltaVolt > solarVolt*0.825){
         gianP = 1;
         gianM = 1;
     }else{
         gianP = 1;
         gianM = 1;
     }*/
     //Pk > Pk-1
     if(nowWatt < preWatt ){
       //gianP = 1.1;
         //gianM = 1;
       
         outputValue+=gianP;
         if(outputValue >=254){
            outputValue = 255;  
         }
     }else //if(nowWatt > preWatt  )
     {//Pk-1 >Pk
         //gianP =1;
         //gianM = 1.1;
         //
           outputValue-=gianM;//
           if(outputValue < 1){
            outputValue = 1;  
         }
         
     }
  }else{//Short
      //if(deltaAmp >0){
        if(255/1.2 <= outputValue){
            outputValue = 255;  
         }else{
        outputValue = outputValue*1.2;
        Serial.println("666666666666666666666666666666666666666666666666666666666");
         }
      //}
      
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
  //เช็นเชอร์จะอ่านค่าVมากสุดได้26.25V 
  valueVolt = (valueVolt /38.9);//
  
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

void reset_millis()
{
 extern volatile unsigned long timer0_millis, timer0_overflow_count;
 timer0_millis = timer0_overflow_count = 0;
}

