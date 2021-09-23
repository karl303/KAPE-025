
#define CS_OUT_PIN    13
#define DATA_OUT_PIN  12
#define CLK_OUT_PIN   11

//int DacCode = 23;   // 23/255 * 3.3 V = 0.297 V
                    // 0.297 V * 2 V/V = 0.595 V
                    
int DacCode = 138;  // 138/255 * 3.3 V = 1.785 V
                    // 1.785 V * 2 V/V = 3.57 V
                    
int TempSensorPin = A0;

float TempReadingVoltage = 0.0;
float TempReadingDegC = 0.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(CS_OUT_PIN, OUTPUT);
  pinMode(DATA_OUT_PIN, OUTPUT);
  pinMode(CLK_OUT_PIN, OUTPUT);

  int k = 0;
  
  digitalWrite(CS_OUT_PIN, LOW);
   
  for(k = 0; k < 24; k++)
  {
    if (k > 15)
    {
      digitalWrite(DATA_OUT_PIN, (DacCode >> (23 - k)) & 0x1);
    }
    else
    {
      digitalWrite(DATA_OUT_PIN, LOW);  // Set all bits low (keep DC/DC converter off)
    }
    
    digitalWrite(CLK_OUT_PIN, LOW);
    digitalWrite(CLK_OUT_PIN, HIGH);
    digitalWrite(CLK_OUT_PIN, LOW);
  }
  digitalWrite(CS_OUT_PIN, HIGH);

  Serial.begin(9600);
  while(!Serial)
  {
    ;
  }
  Serial.println("KAPE-025 driver launching.");
}

void loop() {
  int k = 0;
  int tempReadingRaw = 0;
  int n = 0;
  
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

  digitalWrite(CS_OUT_PIN, LOW);
  //digitalWrite(DATA_OUT_PIN, HIGH);
  for(k = 0; k < 24; k++)
  {
    if(k == 1)
    {
      digitalWrite(DATA_OUT_PIN, HIGH);
    }
    else if(k > 15)
    {
      digitalWrite(DATA_OUT_PIN, (DacCode >> (23 - k)) & 0x1);
    }
    else if((k == 15) || (k == 14))
    {
      digitalWrite(DATA_OUT_PIN, HIGH);
    }
    else if((k == 13) || (k == 12))
    {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    else
    {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    digitalWrite(CLK_OUT_PIN, LOW);
    digitalWrite(CLK_OUT_PIN, HIGH);
    digitalWrite(CLK_OUT_PIN, LOW);
  }
  digitalWrite(CS_OUT_PIN, HIGH);
  
  delay(2500);                       // wait for a second

  for(n = 0; n < 4; n++)
  {
    tempReadingRaw = tempReadingRaw + analogRead(TempSensorPin);
    Serial.print(tempReadingRaw, DEC);
    Serial.print(",");
  }
  Serial.println("");
  TempReadingVoltage = ((float)tempReadingRaw/(float)4)/((float)1024)*(float)3.3;
  TempReadingDegC = (TempReadingVoltage - 0.744) / 0.0119;
  Serial.print(TempReadingVoltage, DEC);
  Serial.print(", ");
  Serial.println(TempReadingDegC, DEC);
  
  delay(2500);

  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  digitalWrite(CS_OUT_PIN, LOW);
  digitalWrite(DATA_OUT_PIN, LOW);
  for(k = 0; k < 24; k++)
  {
    if(k == 1)
    {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    else if(k > 15)
    {
      digitalWrite(DATA_OUT_PIN, (DacCode >> (23 - k)) & 0x1);
    }
    else if((k == 15) || (k == 14))
    {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    else if((k == 13) || (k == 12))
    {
      digitalWrite(DATA_OUT_PIN, HIGH);
    }
    else
    {
      digitalWrite(DATA_OUT_PIN, LOW);
    }
    digitalWrite(CLK_OUT_PIN, LOW);
    digitalWrite(CLK_OUT_PIN, HIGH);
    digitalWrite(CLK_OUT_PIN, LOW);
  }
  digitalWrite(CS_OUT_PIN, HIGH);
  
  delay(2500);                       // wait for a second
  tempReadingRaw = 0;
  for(n = 0; n < 4; n++)
  {
    tempReadingRaw = tempReadingRaw + analogRead(TempSensorPin);
    Serial.print(tempReadingRaw, DEC);
    Serial.print(",");
  }
  Serial.println("");
  TempReadingVoltage = ((float)tempReadingRaw/(float)4)/((float)1024)*(float)3.3;
  TempReadingDegC = (TempReadingVoltage - 0.744) / 0.0119;
  Serial.print(TempReadingVoltage, DEC);
  Serial.print(", ");
  Serial.println(TempReadingDegC, DEC);
  
  delay(2500);
}
