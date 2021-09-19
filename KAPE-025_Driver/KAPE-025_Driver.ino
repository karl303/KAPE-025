
#define CS_OUT_PIN    13
#define DATA_OUT_PIN  12
#define CLK_OUT_PIN   11

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(CS_OUT_PIN, OUTPUT);
  pinMode(DATA_OUT_PIN, OUTPUT);
  pinMode(CLK_OUT_PIN, OUTPUT);
}

void loop() {
  int k = 0;
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
  
  delay(2000);                       // wait for a second
  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  digitalWrite(CS_OUT_PIN, LOW);
  digitalWrite(DATA_OUT_PIN, LOW);
  for(k = 0; k < 24; k++)
  {
    digitalWrite(CLK_OUT_PIN, LOW);
    digitalWrite(CLK_OUT_PIN, HIGH);
    digitalWrite(CLK_OUT_PIN, LOW);
  }
  digitalWrite(CS_OUT_PIN, HIGH);
  delay(2000);                       // wait for a second
}
