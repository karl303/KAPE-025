
#define CS_OUT_PIN    13
#define DATA_OUT_PIN  12
#define CLK_OUT_PIN   11

//int DacCode = 23;   // 23/255 * 3.3 V = 0.297 V
// 0.297 V * 2 V/V = 0.595 V

int DacCode = 138;  // 138/255 * 3.3 V = 1.785 V
// 1.785 V * 2 V/V = 3.57 V

int TempSensorPin = A0;

int IterationCount = 0;

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

  for (k = 0; k < 24; k++)
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
  while (!Serial)
  {
    ;
  }
  Serial.println("KAPE-025 driver launching.");
}

void loop() {
  int k = 0;
  int tempReadingRaw = 0;
  int tempReadingSum = 0;
  int n = 0;
  int i = 0;

  digitalWrite(CS_OUT_PIN, LOW);  // Drive chip select low

  // Loop through bits on the shift register
  for (k = 0; k < 24; k++)
  {
    if (k == 1) // DC/DC converter run bit
    {
      digitalWrite(DATA_OUT_PIN, HIGH);
    }
    else if (k > 15) // R-2R DAC section
    {
      digitalWrite(DATA_OUT_PIN, (DacCode >> (23 - k)) & 0x1);
    }
    else if (((k > 1) && (k < 8)) || ((k > 9) && (k < 16)))
    {
      if (IterationCount < 3)
      {
        if ((k == (15 - IterationCount * 2)) || (k == (14 - IterationCount * 2) ))
        {
          digitalWrite(DATA_OUT_PIN, HIGH);
        }
        else
        {
          digitalWrite(DATA_OUT_PIN, LOW);
        }
      }
      else
      {
        if ((k == (7 - (IterationCount - 3) * 2)) || (k == (6 - (IterationCount - 3) * 2) ))
        {
          digitalWrite(DATA_OUT_PIN, HIGH);
        }
        else
        {
          digitalWrite(DATA_OUT_PIN, LOW);
        }
      }
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

  //Serial.print("Iteration: ");
  //Serial.print(IterationCount);
  //Serial.print(": ");
  for (i = 0; i < 6; i++)
  {
    tempReadingRaw = 0;
    tempReadingSum = 0;

    for (n = 0; n < 4; n++)
    {
      switch (i)
      {
        case 0:
          tempReadingRaw = analogRead(A0);
          break;
        case 1:
          tempReadingRaw = analogRead(A1);
          break;
        case 2:
          tempReadingRaw = analogRead(A2);
          break;
        case 3:
          tempReadingRaw = analogRead(A3);
          break;
        case 4:
          tempReadingRaw = analogRead(A4);
          break;
        case 5:
          tempReadingRaw = analogRead(A5);
          break;
        default:
          tempReadingRaw = 0;
          break;
      }
      //tempReadingRaw = tempReadingRaw + analogRead(TempSensorPin);
      tempReadingSum = tempReadingSum + tempReadingRaw;
      //Serial.print(tempReadingSum, DEC);
      //Serial.print(",");
    }
    //Serial.println("");
    TempReadingVoltage = ((float)tempReadingSum / (float)4) / ((float)1024) * (float)3.3;
    TempReadingDegC = (TempReadingVoltage - 0.744) / 0.0119;
    //Serial.print(TempReadingVoltage, 2);
    //Serial.print(", ");
    Serial.print(TempReadingDegC, 2);
    if (i < 5)
    {
      Serial.print(",");
    }
    else
    {
      Serial.println("");
    }

  }
  delay(2500);

  IterationCount++;
  if (IterationCount == 6)
  {
    IterationCount = 0;
  }
}
