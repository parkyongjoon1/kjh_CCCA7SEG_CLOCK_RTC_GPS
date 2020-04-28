void flash()
{
  byte tmonth, tday, thour, tminute, tsecond;
  byte digit10, digitDot;
  boolean pm = false;
  boolean syncFlash;
  boolean wifiFlash;

  //long m = millis()>>5; //for debug

  breakTime(rtcTime, tm);
  tmonth = tm.Month;
  tday = tm.Day;
  thour = tm.Hour;
  if (thour >= 12) pm = true;
  thour = thour % 12;
  if (thour == 0) thour = 12;
  tminute = tm.Minute;
  tsecond = tm.Second;
  syncFlash = (rtcTime - gpsTime <= SYNKDISPLAYTIME);
  wifiFlash = (tsecond % 2 == 0) && wifiset;
  switch (display_mode) {
    case OFF:
      if (tsecond % 3 == 0)  digitDot = LED_dot;
      else digitDot = LED_off;
      display_loop(LED_off, LED_off, LED_off, digitDot, false, false, false, false);
      //display_loop(LED[m/1000000%10], LED[m/100000%10], LED[m/10000%10], LED[m/1000%10], false, false, false, false);
      break;
    case NORMAL:
      digit10 = (thour < 10) ? LED_off : LED[thour / 10];
      digitDot = (syncFlash & (tsecond%2==0)) ? LED_dot : LED_off;
#ifdef SEGTYPE_080
      if (wifiFlash == false)  display_loop(digit10 | LED_dot, LED[thour % 10], LED[tminute / 10], LED[tminute % 10] | digitDot, false, false, false, false);
      else display_loop(digit10, LED[thour % 10], LED[tminute / 10], LED[tminute % 10] | digitDot, false, false, false, false);
#endif
#ifdef SEGTYPE_056
      if ((wifiFlash == true) || syncFlash)  display_loop(digit10, LED[thour % 10], LED[tminute / 10] | LED_dot, LED[tminute % 10] | LED_dot, false, false, false, false);
      else display_loop(digit10, LED[thour % 10], LED[tminute / 10], LED[tminute % 10], false, false, false, false);
#endif
#ifdef SEGTYPE_SMA41056
      if ((wifiFlash == true) || syncFlash )  display_loop(digit10, LED[thour % 10] | LED_dot, LED[tminute / 10], LED[tminute % 10], false, false, false, false);
      else display_loop(digit10, LED[thour % 10], LED[tminute / 10], LED[tminute % 10], false, false, false, false);
#endif
#ifdef SEGTYPE_EACH
      display_loop(digit10, LED[thour % 10], LED[tminute / 10], LED[tminute % 10] | digitDot, !pm, pm, !wifiFlash, !wifiFlash);
#endif  
#ifdef SEGTYPE_BIGEACH
      display_loop(digit10, LED[thour % 10], LED[tminute / 10], LED[tminute % 10] | digitDot, !wifiFlash, false, false, false);

      //for Debug
      //digitDot = (tsecond%2==0) ? LED_dot : LED_off;
      //display_loop( LED[tsecond % 10]| digitDot,  LED[tsecond % 10]| digitDot,  LED[tsecond % 10]| digitDot,  LED[tsecond % 10] | digitDot,  digitDot, false, false, false);
#endif
      break;
    case MMDD:
      display_loop(LED[tmonth / 10], LED[tmonth % 10], LED[tday / 10], LED[tday % 10], !pm, pm, false, !wifiFlash);
      break;
    case SEC:
#ifdef SEGTYPE_080
      if (wifiFlash == false) display_loop( LED[tminute / 10], LED[tminute % 10] | LED_dot, LED[tsecond / 10], LED[tsecond % 10], false, false, false, false);
      else display_loop( LED[tminute / 10], LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10], false, false, false, false);
#endif
#ifdef SEGTYPE_056
      if (wifiFlash == false) display_loop( LED[tminute / 10], LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10] | LED_dot, false, false, false, false);
      else display_loop( LED[tminute / 10], LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10], false, false, false, false);
      //display_loop( LED[gm.Second / 10], LED[gm.Second % 10], LED[tsecond / 10], LED[tsecond % 10] | LED_dot, false, false, false, false);
#endif
#ifdef SEGTYPE_SMA41056
      if ((wifiFlash == true) || syncFlash ) display_loop( LED[tminute / 10] | LED_dot, LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10], false, false, false, false);
      else display_loop( LED[tminute / 10], LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10], false, false, false, false);
#endif
#ifdef SEGTYPE_EACH
      display_loop( LED[tminute / 10], LED[tminute % 10], LED[tsecond / 10], LED[tsecond % 10], !pm, pm, !wifiFlash, false);
#endif
      break;
    case START:
      if(fcnt>2048) display_mode = NORMAL;
      byte d = (fcnt>>6) % 16;
      display_loop( startLED[d][0],  startLED[d][1],  startLED[d][2],  startLED[d][3], true, true, true, true);
      break;
  } // End of switch

  if (fcnt % 100 == 0) RTCread = true;
  fcnt++;
  if (fcnt >= 32000) fcnt = 0;
}
