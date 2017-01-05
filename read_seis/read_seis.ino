




// Program to read seismometer output -- might eventually be adapted to a feedback loop
// Copied from NewWashFeedbackArduino by Charlie
// Kerkira J Stockton

#include <SPI.h>
#include <math.h>   //avr math library
#include <analogShield.h>
#include <DueTimer.h>
#include <ExtendedADCShield.h>
#include <SD.h>


//Initialize the ADC Shield with default pins and 16-bit ADC (LTC1859)
ExtendedADCShield extendedADCShield(16);

double adcAcc;
double adcVel;
unsigned long acquisitionTime;
unsigned long acquisitionTimeMicros;
char SerialChar;
unsigned long loopTime = 2000; //microseconds. This variable sets the loop's execution rate.
int loopCounter = 0;
double freq;
double arg;
int AnalogDACSetting;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //SPI.begin must be called here on Due only
  SPI.begin();
  

  adcAcc = extendedADCShield.analogReadConfigNext( 0, SINGLE_ENDED, BIPOLAR, RANGE10V);
  adcVel = extendedADCShield.analogReadConfigNext( 1, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcAcc);
  while(Serial.available() > 0){
    SerialChar = Serial.read();
  }

  //Absolute timer interrupt initialization
  Timer3.attachInterrupt(OneCycle);
  Timer3.start(loopTime);
}




void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Entered loop");
  while(1){
    //Everything normally in loop is in OneCycle(), as triggered by the timer.
  }
}



void OneCycle() {
  // Acquire Signals

  adcAcc = extendedADCShield.analogReadConfigNext( 1, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcAcc);
  Serial.print("\t");
  adcVel = extendedADCShield.analogReadConfigNext( 0, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcVel);
  
  timestamp();
  freq = loopCounter/500.;
  //Serial.print(freq);
  Serial.print("\t");
  
  arg = 5*sin(150*2*PI*freq);
  AnalogDACSetting = (int) ( arg/5.0 * 32768.0 + 32768);
  //analog.write( 0, AnalogDACSetting  );
  //analog.write( 1, AnalogDACSetting );
  //analog.write( 2, AnalogDACSetting );
  //Serial.print(arg);
  loopCounter++;

  Serial.print("\n");
  
}  



void timestamp(){
  acquisitionTime = millis();
  acquisitionTimeMicros = micros();
  
  //Compute time since last loop iteration, necessary for derivative computation.
  //loopMicroTime = micros();
  //loopTime = loopMicroTime - oldLoopMicroTime;
  //oldLoopMicroTime = loopMicroTime;
}
