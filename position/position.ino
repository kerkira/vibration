
// NOTE THIS IS NOWHERE NEAR DONE --- needs a LOT of work 



// Program to determine seismometer position using numerical integration
// January 04, 2017
// Adapted from NewWashFeedbackArduino and numerical integrator by Charlie
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
double t;
double arg;
int AnalogDACSetting;
double f;
double fc = 1; // high pass filter time constant
double aveConst = 0;
double xPosition = 0;
double increment = 0;
double filteredX = 0;
double runningMean = 0;
double timestep = 0;
double meanSubtracted;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();  //SPI.begin must be called here on Due only

  timestep = loopTime*1e-6;
  
  // initialize board
  adcAcc = extendedADCShield.analogReadConfigNext( 0, SINGLE_ENDED, BIPOLAR, RANGE10V);
  adcVel = extendedADCShield.analogReadConfigNext( 1, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcAcc);

  // not sure why this is here -- ask Charlie
  while(Serial.available() > 0){
    SerialChar = Serial.read();
  }
  aveConst = exp(-2 * PI * fc * timestep);
  
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

  timestamp();
  t = loopCounter*timestep; // time in seconds
  Serial.print(t,4);
  Serial.print("\t");


  // Euler method - numerical integrator
  increment = adcVel * timestep;
  xPosition += increment;

  //running filter
  filteredX = filteredX*aveConst + increment;

  //continuous mean subtraction
  runningMean = runningMean * aveConst + (1-aveConst) * xPosition;
  meanSubtracted = xPosition - runningMean;

  //To do: 
  //   implement low-pass filter at ~100hz
  //   compute proportional output
  

  // Proportional output
  



  // read off the acceleration and velocity of the seismometer
  adcAcc = extendedADCShield.analogReadConfigNext( 1, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcAcc);
  Serial.print("\t");
  adcVel = extendedADCShield.analogReadConfigNext( 0, SINGLE_ENDED, BIPOLAR, RANGE10V);
  Serial.print(adcVel);
  
  
  // write values to the piezos
  arg = 5*sin(150*2*PI*t);
  AnalogDACSetting = (int) ( arg/5.0 * 32768.0 + 32768);
  analog.write( 0, AnalogDACSetting  );
  analog.write( 1, AnalogDACSetting );
  analog.write( 2, AnalogDACSetting );

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
