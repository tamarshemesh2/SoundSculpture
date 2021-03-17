#include<Wire.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

const float filter_from = 3000;
const int filterLen = 31;
const int signalLen = 100;

int active;
int active_count;
int obj_count;
int time_from_last_obj;

const int MPU6050_addr = 0x68;
int16_t AccX; 

float FILT[filterLen]= {-8.000e-04, -2.200e-03, -4.100e-03, -5.300e-03, -3.900e-03,  1.600e-03,
  1.130e-02,  2.220e-02,  2.850e-02,  2.300e-02,  2.000e-04, -4.040e-02,
 -9.310e-02, -1.461e-01, -1.855e-01,  7.999e-01, -1.855e-01, -1.461e-01,
 -9.310e-02, -4.040e-02,  2.000e-04,  2.300e-02,  2.850e-02,  2.220e-02, 1.130e-02,
 1.600e-03, -3.900e-03, -5.300e-03, -4.100e-03, -2.200e-03, -8.000e-04};
float Input[signalLen] = {0};

float conv = 0;
float val = 0.0;

int i = 0;

#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
#define CHANNEL A0
const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 2048; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void accelerometerReader(){
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr, 14, true);
  AccX = Wire.read()<<8 | Wire.read();
}

void convolution(){
  conv = 0;
  for (int j = 0; j < filterLen; j++){
    int index = (i - j + signalLen) % signalLen;
    conv += Input[index] * FILT[j];
  }
}

void runningAverage() {
  val = 0.5 * abs(conv) + 0.5 * val;
}

void reactToGestures(){
  time_from_last_obj++;
  
  if (val > filter_from && active == 0){
    if (active == 1){
      Serial.print("bake:1,act:0");
    }
    else {
      Serial.print("bake:0,act:0");
    }
    active_count = 0;
    active = 1;
  }
  
  else if (active == 1 && active_count < 5) {
      Serial.print("bake:1,act:0");
      active_count++;
  }
  else if (active == 1 && active_count >= 5 && active_count <= 30) {
      Serial.print("bake:0,act:1");
      active_count++;
      if (active_count == 30) {
        obj_count++;
        time_from_last_obj = 0;
      }
  }
  else {
      Serial.print("bake:0,act:0");
      active_count = 0;
      active = 0;
  }
  Serial.print(",filt:");
  Serial.println(val);
  Serial.print(",obj_count:");
  Serial.print(obj_count);
  Serial.print(",time:");
  Serial.println(time_from_last_obj);
}

void setup()
{
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  
  Wire.begin();
  Wire.setClock(400000);
  mpu.initialize();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  Serial.begin(115200);
  while(!Serial);

  obj_count = 1;
  time_from_last_obj = 0;
}

void loop()
{
  // exelometer:
  accelerometerReader();
  Input[i] = (float) AccX;
  convolution();
  runningAverage();
  i= (i + 1) % signalLen;
  
  // mic:
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */

  Serial.print("norm:");
  Serial.println(AccX);
  Serial.print(" ");
  reactToGestures();
  Serial.print(" ");
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY); /* print magnitudes */
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  Serial.println(x, 6); //Print out what frequency is the most dominant.


  if (AccX == 0) {
    mpu.initialize();
  }
  
  delay(50); /* Repeat after delay */
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 3; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
//    Serial.print(i - 3);
//    Serial.print(" ");
//    Serial.print(abscissa, 6);
//    if(scaleType==SCL_FREQUENCY){
//      Serial.print("Hz");
//    }
//    Serial.print(" ");
    Serial.print(vData[i], 4);
    Serial.print(" ");
  }
  Serial.println();
}
