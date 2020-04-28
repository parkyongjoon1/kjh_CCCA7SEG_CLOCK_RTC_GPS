void display_init()
{
  pinMode(Anode0, OUTPUT);
  pinMode(Anode1, OUTPUT);
  pinMode(Anode2, OUTPUT);
  pinMode(Anode3, OUTPUT);
  
  pinMode(CathodA, OUTPUT);
  pinMode(CathodB, OUTPUT);
  pinMode(CathodC, OUTPUT);
  pinMode(CathodD, OUTPUT);
  pinMode(CathodE, OUTPUT);
  pinMode(CathodF, OUTPUT);
  pinMode(CathodG, OUTPUT);
  pinMode(CathodDp, OUTPUT);
  pinMode(CathodLED, OUTPUT);
  
  analogWrite(Anode0, SEGLOW);
  analogWrite(Anode1, SEGLOW);
  analogWrite(Anode2, SEGLOW);
  analogWrite(Anode3, SEGLOW);
  digitalWrite(CathodA, SEGOFF);
  digitalWrite(CathodB, SEGOFF);
  digitalWrite(CathodC, SEGOFF);
  digitalWrite(CathodD, SEGOFF);
  digitalWrite(CathodE, SEGOFF);
  digitalWrite(CathodF, SEGOFF);
  digitalWrite(CathodG, SEGOFF);
  digitalWrite(CathodDp, SEGOFF);
  digitalWrite(CathodLED, SEGOFF);
};

void display_loop(byte v0, byte v1, byte v2, byte v3, boolean l1, boolean l2, boolean l3, boolean l4) 
{
  byte v;
  boolean l;
  analogWrite(Anode0, SEGLOW);
  analogWrite(Anode1, SEGLOW);
  analogWrite(Anode2, SEGLOW);
  analogWrite(Anode3, SEGLOW);
  
  digitalWrite(CathodA,SEGOFF);
  digitalWrite(CathodB,SEGOFF);
  digitalWrite(CathodC,SEGOFF);
  digitalWrite(CathodD,SEGOFF);
  digitalWrite(CathodE,SEGOFF);
  digitalWrite(CathodF,SEGOFF);
  digitalWrite(CathodG,SEGOFF);
  digitalWrite(CathodDp,SEGOFF);
  digitalWrite(CathodLED, SEGOFF);

  switch(fcnt % 4) {
    case 0:
      v=v0;
      l=l1;
      break;
    case 1:
      v=v1;
      l=l2;
      break;
    case 2:
      v=v2;
      l=l3;
      break;
    case 3:
      v=v3;
      l=l4;
      break;
  }
  
  if(v & 0x01) digitalWrite(CathodA,SEGON);
  if(v & 0x02) digitalWrite(CathodB,SEGON);
  if(v & 0x04) digitalWrite(CathodC,SEGON);
  if(v & 0x08) digitalWrite(CathodD,SEGON);
  if(v & 0x10) digitalWrite(CathodE,SEGON);
  if(v & 0x20) digitalWrite(CathodF,SEGON);
  if(v & 0x40) digitalWrite(CathodG,SEGON);
  if(v & 0x80) digitalWrite(CathodDp,SEGON);
  if(l) digitalWrite(CathodLED, SEGON);
  
  switch(fcnt % 4) {
    case 0:
      analogWrite(Anode0, brightness);
      break;
    case 1:
      analogWrite(Anode1, brightness);
      break;
    case 2:
      analogWrite(Anode2, brightness);
      break;
    case 3:
      analogWrite(Anode3, brightness);
      break;
  }
};
