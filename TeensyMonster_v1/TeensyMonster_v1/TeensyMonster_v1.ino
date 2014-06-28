// Teensy Monster V1
// teensymonster.cc
// Code v1.0
// last updated April, 2014
// license __ http://opensource.org/licenses/MIT


//libraries
#include <Encoder.h> //download from http://www.pjrc.com/teensy/td_libs_Encoder.html


// ------------------------------------------------------------------------------------------------------------------------------------
// EDIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

  //....................................................
  //DEBUG_______________________________________________ 
  //enable if you want to test your output in the serial monitor
  boolean enableDebug = 1;  // 1 for enable, 0 for disable. **Remember to disable debug when running MIDI**. 
  
  //....................................................
  //CHANNEL_____________________________________________
  int channelNumber = 1; //each controller should have a unique channel number between 1 and 10
  
  //....................................................
  //SHIFT_______________________________________________
  //shift buttons offer dual functionality to your pushbuttons and encoders
  //if using a shift button enter the pin number here, else put 0
  int shiftPin = 0; 
  
  //......................................................
  //PUSHBUTTON____________________________________________ 
  //pushbuttons require one digital pin
  //enter '1' for a pin which a pushbutton is connected 
  //enter '0' for a pin which a pushbutton is not connected 
  //pins with '7' are those which are (normally) Teensy analog in, but they can be used as pushbuttons if needed
  //pins with '8' are those which are encoders and should not be used as pushbuttons unless necessary
  //pins with '9' are used for other purposes and can NEVER be used 
  //do NOT include the SHIFT button here
  int toReadPushButton[46] = {  
              //Pin number are written below
  8,8,8,8,0,  //0-4
  0,9,0,0,0,  //5-9
  0,0,0,0,0,  //10-14
  0,0,0,8,8,  //15-19
  9,9,9,9,0,  //20-24
  0,0,0,1,1,  //25-29
  0,0,0,0,0,  //30-34
  0,9,9,7,7,  //35-39
  7,7,7,7,7,  //40-44
  9           //45  
  }; 
  //pushbutton mode
  //there are a few different modes in which you may wish for your pushbutton to behave
  //'1' - standard mode, when pushbutton is engaged note is turned on, when pushbutton is released, note is turned off
  //'2' - on mode, note is only turned on with each click
  //'3' - off mode, note is only turned off with each click
  //'4' - toggle mode, note is switched between on and off with each click
  //'7','8','9' are treated the same as the pushbutton section (above)
  int pushbuttonMode[92] = {  
              //Pin number are written below
  8,8,8,8,1,  //0-4
  1,9,1,1,1,  //5-9
  1,1,1,1,1,  //10-14
  1,1,1,8,8,  //15-19
  9,9,9,9,1,  //20-24
  1,1,1,1,1,  //25-29
  1,1,1,1,1,  //30-34
  1,9,9,7,7,  //35-37
  7,7,7,7,7,  //40-44
  9,          //45
              //When shift button is held, the following pushbuttons are enabled (these are NOT pin numbers)
  8,8,8,8,0,  //46-50 SHIFT
  1,9,1,1,1,  //51-55 SHIFT
  1,1,1,1,1,  //56-60 SHIFT
  1,1,1,8,8,  //61-65 SHIFT
  9,9,9,9,1,  //66-70 SHIFT
  1,1,1,1,1,  //71-75 SHIFT
  1,1,1,1,1,  //76-80 SHIFT
  1,9,9,7,7,  //81-85 SHIFT
  7,7,7,7,7,  //86-90
  9           //91
  }; 
  
  //...................................................
  //DEBOUNCE___________________________________________
  //debounce is a measurement of the time in which a pushbutton is unresponsive after it is pressed
  //this is important to prevent unwanted double clicks 
  int pbBounce = 150; //150 millisecond debounce duration - you may want to change this value depending on the mechanics of your pushbuttons
  
  //..................................................
  //LED_______________________________________________ 
  //LEDs require one digital pin
  //'1' for pins which have LEDs hooked up to them, else '0'
  //you CANNOT hook LEDs and pushbuttons up to the same pins
  //note that pins 14,15,16,24,25,26 have PWM ability - this enables you to adjust the brightness of the LED
  //14,15,16,24,25,26 also already have resistors attached to them, all othe LEDs will require resistors
  //'7','8','9' are treated the same as the pushbutton section
  int toDisplayLED[46] = {  
              //Pin number are written below
  8,8,8,8,0,  //0-4
  0,9,0,0,0,  //5-9
  0,0,0,0,1,  //10-14
  1,0,0,8,8,  //15-19
  9,9,9,9,0,  //20-24
  0,0,0,0,0,  //25-29
  0,0,0,0,0,  //30-34
  0,9,9,7,7,  //35-39
  7,7,7,7,7,  //40-44
  9           //45  
  }; 
  //adjust the brightness of the LEDs connected to pins 14,15,15,24,25,26
  //255 is max brightness, 0 is zero brightness, 127 is half brightness
  int LEDbrightness[6] = {
                //Pin number are written below
  255,255,255,  //14-16
  255,255,255   //24-26
  };
  
  //....................................................
  //ROTARY ENCODER______________________________________ 
  //encoders require two digital pins
  //encoders can be read in two modes: best performance, good performance 
  //for best performance, two digital iterrupt pins are required
  //for good performance, one digitial interrupt pin and one regular digital pin are required
  //you can read three encoders in the best perfomance mode
  //you can read six encoders in the good performance mode
  //Teensy interrupt (INT) pins are 0,1,2,3,18,19
  //{0,5} - example of good performance read (0 is an interrupt pin, and 5 is a regular digital pin)
  //note that interrupt pin MUST come first - {5,0} would not work
  //{18,19} - example of best performance read (both 18 and 19 are digital interrupt pins)
  //**ONLY BEST PERFORMANCE MODE IS CURRENTLY WORKING WITH THIS RELEASE (ERROR IN ENCODER LIBRARY)**
  //enter the pin number if in use, else '99'
  //encoders have dual functionality enabled by the shift button
  //if you are using an i2c module, you CANNOT use encoder pins 0 or 1 
  int encoderPins[6][2] = {
  {0,1}, //encoder 1 
  {2,3}, //encoder 2
  {99,99}, //encoder 3
  {99,99}, //encoder 4 **NOT USABLE**
  {99,99}, //encoder 5 **NOT USABLE**
  {99,99}  //encoder 6 **NOT USABLE**
  }; 
  
  //.........................................................
  //ANALOG IN _______________________________________________ 
  //analog inputs require three pins: power, ground, and input
  //there are two ways to do analog inputs - the multiplexer or direct Teensy 
  //MULTIPLEXER ______
  //CD4067BE - http://www.ti.com/lit/ds/symlink/cd4067b.pdf
  //this mux uses pin 6 as inhibit, and 20,21,22,23 for parallel connection, and teensy ananlog 7 for the reading
  //'1' for multiplexer analog inputs you want to read, else enter '0'
  int toReadAnalogMux[16] = { 
           //IC pin number are written below 
  1,1,0,0, //0-3 
  0,0,0,0, //4-7
  0,0,0,0, //8-11
  0,0,0,0  //12-15
  }; 
  //TEENSY ______
  //directly from Teensy analog pins
  //analog inputs require three pins: power, ground, and input
  //enter '1' for analog inputs you want to read, else enter '0'
  int toReadAnalogTeensy[7] = { 
           //Pin number are written below
  0,0,0,0, //38,39,40,41
  0,0,0    //42,43,44
  };
  //THRESHOLD
  //you can increase this if you have some jitter problems on your analog devices 
  //it is best to solve your jitter problems according to the Instructable
  int analogThreshold = 3; 

// END EDIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
// ------------------------------------------------------------------------------------------------------------------------------------




// VARIABLES AND FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

//PUSHBUTTONS_______________________________________________
long timeHit[92]; //46*2 = 92 
boolean buttonState[92]; //stored state: if the button was last turned on or off
int shiftChange;

//ENCODER_______________________________________________
Encoder *encoders[6];
boolean toReadEncoder[6];
long encPosition[12];
long tempEncPosition;

//ANALOG IN_______________________________________________
int s0 = 20; //control pin A
int s1 = 21; //control pin B
int s2 = 22; //control pin C
int s3 = 23; //control pin D
int SIG_pin = 45; //analog read pin 
int INH_pin = 6; //analog read pin 
int analogInsPrev[16]; //array to hold previously read analog values - set all to zero for now
int tempAnalogInMux = 0; //array to hold previously read analog values 
int tempAnalogInMap = 0;
int controlPin[] = {s0,s1,s2,s3}; //set contol pins in array
//control array 
int muxChannel[16][4]={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};
//function to read mux
int readMux(int channel){  
  //loop through the four control pins
  for(int i = 0; i < 4; i ++){ 
    //turn on/off the appropriate control pins according to what channel we are trying to read 
    digitalWrite(controlPin[i], muxChannel[channel][i]); 
  }
  //read the value of the pin
  int val = analogRead(SIG_pin); 
  //return the value
  return val; 
}
int analogPinsTeensy[7] = {38,39,40,41,42,43,44};
int analogInsPrevTeensy[7]; //array to hold previously read analog values 
int tempAnalogInTeensy = 0; 
int tempAnalogInMapTeensy = 0;







// ======================================================================================
// SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
void setup(){ 

  //DEBUG _______________________________________________
  if(enableDebug==1){
    Serial.begin(9600);//open serail port @ debug speed
    Serial.flush();
    Serial.println();
    Serial.println("___DEBUG ENABLED___");
    Serial.println();
  }else{
    Serial.begin(31250);//open serail port @ midi speed
  }
  
  //CHECK FOR CONFLICTING INPUTS_______________________________________________
  if(enableDebug){
    Serial.println("~~~~~~~~~~~~~~ CHECKING CONFLICTS ~~~~~~~~~~~~~~");
    
    //pushbutton vs encoder
    //LED vs encoder
    //pushbutton vs LED
    //teensy analog vs pushbutton
    //teensy analog vs LED
    //teensy analog vs encoder
    
    //pushbutton vs encoder
    for(int p=0;p<46;p++){ //loop through all pushbuttons
      for(int e=0;e<6;e++){ //loop through all encoders
        if( (encoderPins[e][0]==p && toReadPushButton[p]==1) || (encoderPins[e][1]==p && toReadPushButton[p]==1) ){
          Serial.println("ERROR - pin ["+(String)p+"] enabled in both pushbutton and encoder");  
        }
      }
    }
    //LED vs encoder
    for(int d=0;d<46;d++){ //loop through all LEDs
      for(int e=0;e<6;e++){ //loop through all encoders
        if( (encoderPins[e][0]==d && toDisplayLED[d]==1) || (encoderPins[e][1]==d && toDisplayLED[d]==1) ){
          Serial.println("ERROR - pin ["+(String)d+"] enabled in both encoder and LED");  
        } 
      }
    } 
    //pushbutton vs LED
    for(int i=0;i<46;i++){ //loop through all pins
      if(toReadPushButton[i]==1 && toDisplayLED[i]==1){
        Serial.println("ERROR - pin ["+(String)i+"] enabled in both pushbutton and LED");  
      }
    }
    //teensy analog vs pushbutton
    for(int a=0;a<7;a++){ //loop through all analog, a+38 is pin #
      if(toReadPushButton[a+38]==1  && toReadAnalogTeensy[a]==1){
        Serial.println("ERROR - pin ["+(String)(a+38)+"] enabled in both pushbutton and analog teensy");
      }
    }
    //teensy analog vs LED
    for(int a=0;a<7;a++){ //loop through all analog, a+38 is pin #
      if(toDisplayLED[a+38]==1  && toReadAnalogTeensy[a]==1){
        Serial.println("ERROR - pin ["+(String)(a+38)+"] enabled in both LED and analog teensy");
      }
    }
    //teensy analog vs encoder
    for(int a=0;a<7;a++){ //loop through all analog, a+38 is pin #
      for(int e=0;e<6;e++){ //loop through all encoders
        if( (encoderPins[e][0]==a+38 && toReadAnalogTeensy[a]==1) || (encoderPins[e][1]==a+38 && toReadAnalogTeensy[a]==1) ){
          Serial.println("ERROR - pin ["+(String)(a+38)+"] enabled in both encoder and analog teensy");  
        } 
      }
    }
  }
  
  if(enableDebug){
    Serial.println("~~~~~~~~~~~~~~ PIN CONFIG ~~~~~~~~~~~~~~");  
  }
  
  //SHIFT - pin config _______________________________________________
  //we need enable the shift pin as an INPUT as well as turn on the pullup resistor 
  if(shiftPin!=0){
    pinMode(shiftPin,INPUT_PULLUP); //shift button
    if(enableDebug){  
      Serial.println("SHIFT button is enabled on pin ["+(String)shiftPin+"]"); 
    }
  }
  
  //PUSHBUTTON - pin config _______________________________________________
  //we need enable each pushbutton pin as an INPUT as well as turn on the pullup resistor 
  for(int i=0;i<46;i++){
    if(toReadPushButton[i]==1){
      pinMode(i,INPUT_PULLUP); //pushbutton pullup
      if(enableDebug){
        Serial.println("Pushbutton on pin ["+(String)i+"] is enabled with pushbutton mode ["+(String)pushbuttonMode[i]+"]");  
      }
    }  
  }
  
  //LED - pin config _______________________________________________
  //we need enable each LED pin as an OUTPUT
  for(int i=0;i<46;i++){
    if(toDisplayLED[i]==1){
      pinMode(i,OUTPUT); //pushbutton pullup
      if(enableDebug){
        Serial.println("LED on pin ["+(String)i+"] is enabled"); 
      }
    }  
  }   

  //ENCODER - pin config _______________________________________________
  for(int i=0;i<6;i++){
    if(encoderPins[i][0]!=99 && encoderPins[i][1]!=99){
      encoders[i] = new Encoder(encoderPins[i][0],encoderPins[i][1]);
      toReadEncoder[i] = true;
      if(enableDebug){
        Serial.println("Encoder on pins ["+(String)encoderPins[i][0]+","+(String)encoderPins[i][1]+"] is enabled"); 
      }
    }
    else{
      toReadEncoder[i] = false;  
    }
  }
  
  //ANALOG IN - pin config _______________________________________________
  for(int i=0;i<16;i++){
    if(toReadAnalogMux[i]==1 && enableDebug==1){
      Serial.println("Analog in on multiplexer pin ["+(String)i+"] is enabled");   
    }
  }
  for(int i=0;i<7;i++){
    if(toReadAnalogTeensy[i]==1 && enableDebug){
      int p = i+38;
      Serial.println("Analog in on teensy pin ["+(String)p+"] is enabled");   
    }
  }
  //set analog in reading
  pinMode(SIG_pin,INPUT);
  pinMode(INH_pin,OUTPUT);
  digitalWrite(INH_pin,LOW); //can we do this on led 6?
  //set our control pins to output
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT); 
  //turn all control pins off (for now)
  digitalWrite(s0,LOW);
  digitalWrite(s1,LOW);
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  
  //DEBUG
  if(enableDebug){
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("PROGRAM WILL START LOOPING IN THREE SECONDS...");
    delay(3000); //wait three seconds
  }
}



// ======================================================================================
// LOOPS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
void loop(){
  
  //SHIFT loop _______________________________________________
  if(shiftPin!=0){
    if(digitalRead(shiftPin)==LOW){ //check if shift button was engaged
      shiftChange = 46;  //if enganged, the offset is 46
    }else{
      shiftChange = 0;  
    }
  }
  
  //PUSHBUTTONS loop _______________________________________________
  boolean tempDigitalRead;
  for(int i=0;i<46;i++){ //loop through all 46 digital pins
    if(i!=shiftPin){ //ensure this is not the shift pin
      int j = i + shiftChange; //add the shift change (+46)
      if(toReadPushButton[i]==1){ //check if this a pin with a pushbutton hooked up to it
        tempDigitalRead = digitalRead(i);
        if(pushbuttonMode[j]==1 && tempDigitalRead!=buttonState[j]){ //___NORMAL MODE (1)
          delay(2); //just a delay for noise to ensure push button was actually hit
          if(digitalRead(i)==tempDigitalRead){ //check if pushbutton is still the same
            if(tempDigitalRead==LOW){ //button pressed, turn note on
              midiSend('p',1,j); //call note on/off function
            }else{ //button released
              midiSend('p',0,j);
            }
            buttonState[j] = tempDigitalRead; //update the state (on or off)           
          }
        }else{ //___ALL OTHER MODES (2,3,4) 
          if(digitalRead(i)==LOW && (millis()-timeHit[j])>pbBounce){ //check bounce time  
            if(pushbuttonMode[j]==2){ //mode 2 - only note on
              midiSend('p',1,j); 
            }else if(pushbuttonMode[j]==3){ //mode 3 - only note off
              midiSend('p',0,j);          
            }else{ //mode 4 - toggle
              if(buttonState[j]==1){ //on->off
                midiSend('p',0,j);
                buttonState[j]=0;  
              }else{ //off->on
                midiSend('p',1,j);
                buttonState[j]=1;  
              }
            }
            timeHit[j] = millis();
          }
        }   
      }
    }
  }
  
  //ENCODER loop _______________________________________________
  for(int i=0;i<6;i++){ //loop through all encoders
    if(toReadEncoder[i]==true){ //check if we should read this encoder
      tempEncPosition = encoders[i]->read(); //get encoder position
      if(shiftChange==46){ //shift button is engaged
        i=i+6; //encoders 6-11
      }else{
        //encoders 0-5  
      }
      if(tempEncPosition > encPosition[i]){ //this position is greater than the last
          midiSend('e',1,i); //send message
          encPosition[i] = tempEncPosition; //update position          
      }else if(tempEncPosition < encPosition[i]){ //this position is less than the last
          midiSend('e',0,i); //send message
          encPosition[i] = tempEncPosition; //update position          
      }else{
        //do nothing  
      }
    }
  } 
 
  //ANALOG IN MUX loop _______________________________________________
  for(int i=0;i<16;i++){ //loop through 16 mux channels
    if(toReadAnalogMux[i]==1){ //we read this mux channel analog input
      tempAnalogInMux = readMux(i); //ready valued using readMux function
      if(abs(analogInsPrev[i]-tempAnalogInMux)>analogThreshold){ //ensure value changed more than our threshold
        tempAnalogInMap = map(tempAnalogInMux,0,1023,0,127); //remap value between 0 and 127
        midiSend('a',tempAnalogInMap,i); //send message
        analogInsPrev[i]=tempAnalogInMux; //update current value
      }
    }    
  } 
  
  //ANALOG IN TEENSY loop _______________________________________________
  for(int i=0;i<7;i++){ //loop through the 6 analog teensy channels
    if(toReadAnalogTeensy[i]==1){ //we read this analog input
      tempAnalogInTeensy = analogRead(analogPinsTeensy[i]);
      if(abs(analogInsPrevTeensy[i]-tempAnalogInTeensy)>analogThreshold){ //ensure value changed more than our threshold
        tempAnalogInMapTeensy = map(tempAnalogInTeensy,0,1023,0,127); //remap value between 0 and 127
        midiSend('a',tempAnalogInMapTeensy,i+16); //send message
        analogInsPrevTeensy[i]=tempAnalogInTeensy; //update current value
      }
    }    
  }
    
  //LED loop _______________________________________________
  //you have to write your own LED code -- everyone will expect their LEDs to behave in a different way
  //SAMPLE 1
  //here is a sample of how push-button on pin 4 would turn on the LED connected to pin 14
  //note that pin 14 is a PWM pin, thus we can adjust the brightness from 0-255
//  if(buttonState[2]==0){ //push-button on pin 4 is on
//    analogWrite(14,127); //turn on the LED, half brightness
//  }else{ //push-button on pin 4 is off
//    analogWrite(14,0); //turn off the LED  
//  }
  //SAMPLE 2
  //here is a sample of how push-button on pin 12 would turn on the LED connected to pin 35
  //note that pin 35 is a regular digital pin and can only turn on and off
//  if(buttonState[12]==0){ //push-button on pin 4 is on
//    digitalWrite(35,HIGH); //turn on LED
//  }else{ //push-button on pin 4 is off
//    digitalWrite(35,LOW); //turn off LED  
//  }
  if(buttonState[28]==0){ 
    analogWrite(14,255); 
  }else{ 
    analogWrite(14,0); 
  }
  if(buttonState[29]==0){ 
    analogWrite(15,255); 
  }else{ 
    analogWrite(15,0); 
  }

}






// ........................................................................................................
// COMMUNICATION FUNCTIONS ________________________________________________________________________________
// http://www.pjrc.com/teensy/td_midi.html

//debug out
void serialDebugOut(String cType, int cNum, String cVal){
  Serial.print(cType);
  Serial.print(" ");
  Serial.print(cNum);
  Serial.print(": ");
  Serial.println(cVal);    
}

//function to send MIDI 
void midiSend(char type, int val, int pin){ 
  String clickState;
  switch (type){
    
  case 'p': //--------------- PUSHBUTTON   
    if(enableDebug){
      if(val==1){
        clickState = "click on";  
      }else{
        clickState = "click off";  
      }
      serialDebugOut("Pushbutton",pin,clickState);  
    }
    else{
      if(val==1){
        //usbMIDI.sendNoteOn(pin,127,channelNumber); //!!!
      }else{
        //usbMIDI.sendNoteOff(pin,127,channelNumber); //!!!
      }
    }
  break;
    
  case 'e': //--------------- ENCODER
    if(enableDebug){
      if(val==1){
        clickState = "forward";  
      }else{
        clickState = "reverse";  
      }
      serialDebugOut("Encoder",pin,clickState);  
    }
    else{
      if(val==1){
        //fuzzy
        //usbMIDI.sendNoteOn(pin+46,127,channelNumber); //!!! //we have 46 used digitals, thus must add 46
        //ableton
        //usbMIDI.sendControlChange(pin+23,68,channelNumber); //!!! //we have 23 used analogs, thus must add 23
        //other
        //usbMIDI.sendControlChange(pin+23,65,channelNumber); //!!! //we have 23 used analogs, thus must add 23
        
      }
      else{
        //fuzzy
        //usbMIDI.sendNoteOff(pin+46,127,channelNumber); //!!! //we have 46 used digitals, thus must add 46
        //ableton
        //usbMIDI.sendControlChange(pin+23,60,channelNumber); //!!! //we have 23 used analogs, thus must add 23
        //other
        //usbMIDI.sendControlChange(pin+23,63,channelNumber); //!!! //we have 23 used analogs, thus must add 23
      }
    }
  break;
 
  case 'a': //--------------- ANALOG   
    if(enableDebug){
      if(pin>15){
        serialDebugOut("Analog Teensy",pin,val);   
      }else{
        serialDebugOut("Analog mux",pin,val);  
      }
    }else{
      //usbMIDI.sendControlChange(pin,val,channelNumber); //!!! 
    }
  break;
    
  }
}
