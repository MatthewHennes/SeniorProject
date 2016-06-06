// Top resistor: 248.4 k
// Bottom resistor: 4.68 k
// Req: 253.08 k

#include <SPI.h>

const int ADC_VAL_BUFF_SIZE = 50;
const int NUM_BATTERIES = 18;

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

const int DEMUX2_ENABLE = 11; //B7
const int DEMUX2_A = 19; //F6
const int DEMUX2_B = 20; //F5
const int DEMUX2_C = 21; //F4

const int DEMUX3_ENABLE = 4; //D4
const int DEMUX3_A = 1; //D3
const int DEMUX3_B = 0; //D2
const int DEMUX3_C = 2; //D1

// Setup the various components
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
  
  // Initialize Demux 2
  pinMode(DEMUX2_ENABLE, OUTPUT);
  digitalWrite(DEMUX2_ENABLE, LOW);
  pinMode(DEMUX2_A, OUTPUT);
  digitalWrite(DEMUX2_A, LOW);
  pinMode(DEMUX2_B, OUTPUT);
  digitalWrite(DEMUX2_B, LOW);
  pinMode(DEMUX2_C, OUTPUT);
  digitalWrite(DEMUX2_C, LOW);
  
  // Initialize Demux 3
  pinMode(DEMUX3_ENABLE, OUTPUT);
  digitalWrite(DEMUX3_ENABLE, LOW);
  pinMode(DEMUX3_A, OUTPUT);
  digitalWrite(DEMUX3_A, LOW);
  pinMode(DEMUX3_B, OUTPUT);
  digitalWrite(DEMUX3_B, LOW);
  pinMode(DEMUX3_C, OUTPUT);
  digitalWrite(DEMUX3_C, LOW);
}

// Take measuremnts and output results
void loop()
{
  unsigned int ADCResults[NUM_BATTERIES];
  char value[ADC_VAL_BUFF_SIZE];
  char battery[ADC_VAL_BUFF_SIZE];
  char raw[ADC_VAL_BUFF_SIZE];
  float voltage = 0;
  
  // Take measurements
  for (int i = 0; i < NUM_BATTERIES; i++)
  {
    ADCResults[i] = measureBattery(i);
    /*
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
    */
    delay(100);
  }
  
  // Output results
  for (int i = 0; i < NUM_BATTERIES; i++)
  {
    if (i == 0)
      voltage = ADCResults[i] * V_MULTIPLIER;
    else
      voltage = (ADCResults[i] - ADCResults[i - 1]) *
       V_MULTIPLIER; 
    Serial.write("Voltage ");
    String(i).toCharArray(battery, ADC_VAL_BUFF_SIZE);
    Serial.write(battery);
    Serial.write(": ");
    String(voltage).toCharArray(value, ADC_VAL_BUFF_SIZE);
    Serial.write(value);
    Serial.write(" V\n");
  }
  Serial.write("\n");
  
  delay(1000);
}

// Take an ADC sample, and read it from SPI
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
  
  return result + 11; // compensate for offset
}

// Measure the specified battery's voltage
unsigned int measureBattery(int battery)
{
  unsigned int ADC_Value;
  
  digitalWrite(DEMUX1_ENABLE, LOW);
  digitalWrite(DEMUX1_A, LOW);
  digitalWrite(DEMUX1_B, LOW);
  digitalWrite(DEMUX1_C, LOW);
  digitalWrite(DEMUX2_ENABLE, LOW);
  digitalWrite(DEMUX2_A, LOW);
  digitalWrite(DEMUX2_B, LOW);
  digitalWrite(DEMUX2_C, LOW);
  digitalWrite(DEMUX3_ENABLE, LOW);
  digitalWrite(DEMUX3_A, LOW);
  digitalWrite(DEMUX3_B, LOW);
  digitalWrite(DEMUX3_C, LOW);
  
  switch(battery)
  {
    case 0:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      //delay(10000);
      break;
      
    case 1:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_A, HIGH);
      //delay(10000);
      break;
      
    case 2:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_B, HIGH);
      //delay(10000);
      break;
      
    case 3:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_B, HIGH);
      digitalWrite(DEMUX1_A, HIGH);
      //delay(10000);
      break;
      
    case 4:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_C, HIGH);
      //delay(10000);
      break;
      
    case 5:
      digitalWrite(DEMUX1_ENABLE, HIGH);
      digitalWrite(DEMUX1_C, HIGH);
      digitalWrite(DEMUX1_A, HIGH);
      //delay(10000);
      break;
      
    case 6:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      //delay(10000);
      break;
      
    case 7:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      digitalWrite(DEMUX2_A, HIGH);
      //delay(10000);
      break;
      
    case 8:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      digitalWrite(DEMUX2_B, HIGH);
      //delay(10000);
      break;
      
    case 9:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      digitalWrite(DEMUX2_B, HIGH);
      digitalWrite(DEMUX2_A, HIGH);
      //delay(10000);
      break;
      
    case 10:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      digitalWrite(DEMUX2_C, HIGH);
      //delay(10000);
      break;
      
    case 11:
      digitalWrite(DEMUX2_ENABLE, HIGH);
      digitalWrite(DEMUX2_C, HIGH);
      digitalWrite(DEMUX2_A, HIGH);
      //delay(10000);
      break;
      
    case 12:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      //delay(10000);
      break;
      
    case 13:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      digitalWrite(DEMUX3_A, HIGH);
      //delay(10000);
      break;
      
    case 14:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      digitalWrite(DEMUX3_B, HIGH);
      //delay(10000);
      break;
      
    case 15:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      digitalWrite(DEMUX3_B, HIGH);
      digitalWrite(DEMUX3_A, HIGH);
      //delay(10000);
      break;
      
    case 16:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      digitalWrite(DEMUX3_C, HIGH);
      //delay(10000);
      break;
      
    case 17:
      digitalWrite(DEMUX3_ENABLE, HIGH);
      digitalWrite(DEMUX3_C, HIGH);
      digitalWrite(DEMUX3_A, HIGH);
      //delay(10000);
      break;
    
    default:
      digitalWrite(DEMUX1_ENABLE, LOW);
      digitalWrite(DEMUX1_A, LOW);
      digitalWrite(DEMUX1_B, LOW);
      digitalWrite(DEMUX1_C, LOW);
      digitalWrite(DEMUX2_ENABLE, LOW);
      digitalWrite(DEMUX2_A, LOW);
      digitalWrite(DEMUX2_B, LOW);
      digitalWrite(DEMUX2_C, LOW);
      digitalWrite(DEMUX3_ENABLE, LOW);
      digitalWrite(DEMUX3_A, LOW);
      digitalWrite(DEMUX3_B, LOW);
      digitalWrite(DEMUX3_C, LOW);
      break;
  }
  
  delay(100);
  
  ADC_Value = takeADCSample();
  
  digitalWrite(DEMUX1_ENABLE, LOW);
  digitalWrite(DEMUX1_A, LOW);
  digitalWrite(DEMUX1_B, LOW);
  digitalWrite(DEMUX1_C, LOW);
  digitalWrite(DEMUX2_ENABLE, LOW);
  digitalWrite(DEMUX2_A, LOW);
  digitalWrite(DEMUX2_B, LOW);
  digitalWrite(DEMUX2_C, LOW);
  digitalWrite(DEMUX3_ENABLE, LOW);
  digitalWrite(DEMUX3_A, LOW);
  digitalWrite(DEMUX3_B, LOW);
  digitalWrite(DEMUX3_C, LOW);
  
  return ADC_Value;
}
