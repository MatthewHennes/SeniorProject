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

const int DEMUX1_ENABLE = 12; //D6
const int DEMUX1_A = 8; //B4
const int DEMUX1_B = 9; //B5
const int DEMUX1_C = 10; //B6

void setup()
{
  Serial.begin(9600);
  
  // start the SPI library:
  SPI.begin();
  
  // Initialize ADC
  pinMode(ADC_DISABLE, OUTPUT);
  digitalWrite(ADC_DISABLE, HIGH);
  
  // Initialize Demux 1
  pinMode(DEMUX1_ENABLE, OUTPUT);
  digitalWrite(DEMUX1_ENABLE, LOW);
  pinMode(DEMUX1_A, OUTPUT);
  digitalWrite(DEMUX1_A, LOW);
  pinMode(DEMUX1_B, OUTPUT);
  digitalWrite(DEMUX1_B, LOW);
  pinMode(DEMUX1_C, OUTPUT);
  digitalWrite(DEMUX1_B, LOW);
}

void loop()
{
  unsigned int ADCResult = 0;
  char value[ADC_VAL_BUFF_SIZE];
  char battery[ADC_VAL_BUFF_SIZE];
  char raw[ADC_VAL_BUFF_SIZE];
  float voltage = 0;
  
  for (int i = 0; i < 3; i++)
  {
    ADCResult = measureBattery(i);
    voltage = ADCResult * V_MULTIPLIER;
    Serial.write("Voltage ");
    String(i).toCharArray(battery, ADC_VAL_BUFF_SIZE);
    Serial.write(battery);
    Serial.write(": ");
    String(ADCResult).toCharArray(raw, ADC_VAL_BUFF_SIZE);
    Serial.write(raw);
    Serial.write(" = ");
    String(voltage).toCharArray(value, ADC_VAL_BUFF_SIZE);
    Serial.write(value);
    Serial.write(" V\n");
    delay(100);
  }
  Serial.write("\n");
  
  delay(1000);
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

unsigned int measureBattery(int battery)
{
  unsigned int ADC_Value;
  
  digitalWrite(DEMUX1_ENABLE, LOW);
  digitalWrite(DEMUX1_A, LOW);
  digitalWrite(DEMUX1_B, LOW);
  digitalWrite(DEMUX1_C, LOW);
  
  switch(battery)
  {
    case 0:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      break;
      
    case 1:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_A, HIGH);
      break;
      
    case 2:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_B, HIGH);
      break;
    
    default:
      digitalWrite(DEMUX1_ENABLE, LOW);
      digitalWrite(DEMUX1_A, LOW);
      digitalWrite(DEMUX1_B, LOW);
      digitalWrite(DEMUX1_C, LOW);
      break;
  }
  
  ADC_Value = takeADCSample();
  
  digitalWrite(DEMUX1_ENABLE, LOW);
  digitalWrite(DEMUX1_A, LOW);
  digitalWrite(DEMUX1_B, LOW);
  digitalWrite(DEMUX1_C, LOW);
  
  return ADC_Value;
}
