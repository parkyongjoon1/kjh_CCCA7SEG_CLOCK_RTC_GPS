void buttonChanged(int state){
  if(state==LOW) {
    changemode();
  }
  //Serial.println("Changed: " + String(state));
}

void changemode()
{
  switch (display_mode) {
    case OFF:
        display_mode = NORMAL;
      break;
    case NORMAL:
        display_mode = MMDD;
        buttonTime = makeTime(tm);
      break;
    case MMDD:
        //display_mode = SEC;
        display_mode = NORMAL;
      break;
    case SEC:
        display_mode = OFF;
      break;
    case START:
        display_mode = NORMAL;
      break;
  } // End of switch
}
