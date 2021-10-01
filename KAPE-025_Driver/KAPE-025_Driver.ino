
#define CS_OUT_PIN    13
#define DATA_OUT_PIN  12
#define CLK_OUT_PIN   11

//int DacCode = 23;   // 23/255 * 3.3 V = 0.297 V
// 0.297 V * 2 V/V = 0.595 V

int DacCode = 138;  // 138/255 * 3.3 V = 1.785 V
// 1.785 V * 2 V/V = 3.57 V

int DcDcRun = 0;
int SensorEnable = 0;
int HeaterControl = 0;

int TempSensorPin = A0;

int IterationCount = 0;

float TempReadingVoltage = 0.0;
float TempReadingDegC = 0.0;

void setup() {
  DcDcRun = 0;
  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(CS_OUT_PIN, OUTPUT);
  pinMode(DATA_OUT_PIN, OUTPUT);
  pinMode(CLK_OUT_PIN, OUTPUT);

  writeSipo();

  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  Serial.println("KAPE-025 driver launching.");

  DcDcRun = 1;
}

void loop() {

  if(IterationCount == 0)
  {
    HeaterControl = 1;
    DacCode = 138;
  }
  else
  {
    HeaterControl = HeaterControl << 1;
    DacCode = DacCode - 20;
  }

  writeSipo();

  
  delay(250);                       // wait for a second

  readPrintAnalog();
  
  delay(250);

  IterationCount++;
  if (IterationCount == 6)
  {
    IterationCount = 0;
  }
}

void readPrintAnalog()
{
  int tempReadingRaw = 0;
  int tempReadingSum = 0;
  int i = 0;
  int n = 0;

  SensorEnable = 1;
  for (i = 0; i < 6; i++)
  {
    writeSipo();
    delay(3);
    SensorEnable = SensorEnable << 1;
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
    
    TempReadingVoltage = ((float)tempReadingSum / (float)4) / ((float)1024) * (float)3.3;
    TempReadingDegC = (TempReadingVoltage - 0.744) / 0.0119;

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
  
  SensorEnable = 0;
  writeSipo();
}
