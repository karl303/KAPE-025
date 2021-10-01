void writeSipo()
{
  int i = 0;
  int U4 = 0;
  int U3 = 0;
  int U2 = 0;

  for(i = 3; i < 6; i++)
  {
    U4 = (U4 << 1) | ((HeaterControl >> i) & 0x1);
    U4 = (U4 << 1) | ((SensorEnable >> i) & 0x1);
  }
  U4 = (U4 << 1) | (DcDcRun & 0x1);
  U4 = (U4 << 1);

  for(i = 0; i < 3; i++)
  {
    U3 = (U3 << 1) | ((HeaterControl >> i) & 0x1);
    U3 = (U3 << 1) | ((SensorEnable >> i) & 0x1);
  }
  U3 = (U3 << 2);

  U2 = reverseBits(DacCode);


  digitalWrite(CS_OUT_PIN, LOW);

  for(i = 0; i < 8; i++)
  {
    writeBit(U4);
    U4 = U4 >> 1;
  }

  for(i = 0; i < 8; i++)
  {
    writeBit(U3);
    U3 = U3 >> 1;
  }

  for(i = 0; i < 8; i++)
  {
    writeBit(U2);
    U2 = U2 >> 1;
  }

  digitalWrite(CS_OUT_PIN, HIGH);
  
}

int reverseBits(int inputValue)
{
  int temp = inputValue;
  int reversed = 0;
  int i = 0;

  for(i = 0; i < 8; i++)
  {
    reversed = (reversed << 1) | (temp & 0x1);
    temp = temp >> 1;
  }
  
  return reversed;
}


void writeBit(int bitValue)
{
  digitalWrite(DATA_OUT_PIN, bitValue & 0x1);
  digitalWrite(CLK_OUT_PIN, HIGH);
  digitalWrite(CLK_OUT_PIN, LOW);
}
