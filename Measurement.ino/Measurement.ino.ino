// Top resistor: 248.4 k
// Bottom resistor: 4.68 k
// Req: 253.08 k

#include <SPI.h>

const int ADC_VAL_BUFF_SIZE = 50;

const float V_REF = 5.06;
const float R_EQ = 253.08;
const float R_MEAS = 4.68;
const float STEP_SIZE = (2 * V_REF) / 8192;
const float R_MULTIPLIER = R_EQ / R_MEAS;
const float V_MULTIPLIER = R_MULTIPLIER * STEP_SIZE;

const int ADC_DISABLE = 6; //D7
const int

void setup()
{
  Serial.begin(9600);
  
  // start the SPI library:
  SPI.begin();
  
  // Initialize ADC
  pinMode(ADC_DISABLE, OUTPUT);
  digitalWrite(ADC_DISABLE, HIGH);
}

void loop()
{
  unsigned int ADCResult = 0;
  char value[ADC_VAL_BUFF_SIZE];
  float voltage = 0;
  
  delay(1000);
  ADCResult = takeADCSample();
  voltage = ADCResult * V_MULTIPLIER;
  Serial.write("Voltage: ");
  String(voltage).toCharArray(value, ADC_VAL_BUFF_SIZE);
  Serial.write(value);
  Serial.write("\n");
}

unsigned int takeADCSample()
{
  unsigned int result = 0;
  
  SPI.beginTransaction(SPISettings(1700000, MSBFIRST,
   SPI_MODE0));
  delay(100);
  digitalWrite(ADC_DISABLE, LOW);
  delay(100);
  result = SPI.transfer16(0);
  delay(100);
  SPI.endTransaction();
  digitalWrite(ADC_DISABLE, HIGH);
  
  return result;
}
