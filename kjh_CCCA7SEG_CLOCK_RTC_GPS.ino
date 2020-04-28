/**********************************************************
   Title : DIGITAL CLOCK using RTC with FM Receiver
   Create Date : 2017.12.15.
   Modify Date : 2018.05.29.
   Author : Park Yongjoon
 **********************************************************/

// Common Cathod 일 경우 CA_SEG, Common Anode 일 경우 CC_SEG 정의 하시오.
#define CC_SEG
//개별세크먼트인경우 SEGTYPE_EACH 정의, 별도 콜론
// 0.56인치인 WHITE경우 SEGTYPE_056 정의, 왼쪽에서 세번째,네번째 도트가 콜론임. (CC_SEG)
// 0.56인치인 RED경우 SEGTYPE_SMA41056 정의, 왼쪽에서 두번째 자리 도트가 콜론임 (CC_SEG)
// 0.80인치인경우 SEGTYPE_080 정의 (CC_SEG)
// 큰개별세크먼트(벽시계)인 경우 SEGTYPE_BIGEACH 정의, 별도 콜론(CA_SEG)
#define SEGTYPE_SMA41056

//GPS일경우 3초, WIFI일경우 3600초로 설정하면 좋음.
#define SYNKDISPLAYTIME 3600

//가장 어두울 때의 값 0~255
#define SEGMINVALUE 10
//가장 밝을 때의 값 0~255
#define SEGHIGHVALUE 255


#include <Arduino.h>
#include <MsTimer2.h>
#include <DS1307RTC.h>
#include <TimeLib.h>
#include <ButtonDebounce.h>
#include <TinyGPS++.h>
#include "LED.h"

#ifdef CC_SEG
#define SEGLOW 255
#define SEGMIN (255-SEGMINVALUE)
#define SEGHIGH (255-SEGHIGHVALUE)
#define SEGON LOW
#define SEGOFF HIGH
#endif

#ifdef CA_SEG
#define SEGLOW 0
#define SEGMIN SEGMINVALUE
#define SEGHIGH SEGHIGHVALUE
#define SEGON HIGH
#define SEGOFF LOW
#endif

ButtonDebounce button(A1, 250 * 5);

unsigned int fcnt = 0, pfcnt = 0;
tmElements_t tm, gm;
time_t rtcTime, gpsTime, buttonTime;
TinyGPSPlus gps;
boolean wifiset = false;
enum {START, OFF, NORMAL, MMDD, SEC} display_mode = START;
unsigned int brightness = SEGHIGH; //처음 밝게 시작
boolean gpsLock = false;
boolean RTCread = false;

void setup()
{
  button.setCallback(buttonChanged);
  display_init();
  //pinMode(2, OUTPUT);  //정시 출력을 사용할 때.

  Serial.begin(9600);

  //PWM주파수를 높여서 7세크먼트 깜빡임을 최소화 함.
  setPwmFrequency(10, 1); //timer1  31250/divide
  setPwmFrequency(9, 1);
  setPwmFrequency(5, 1); //timer 62500/divide
  setPwmFrequency(6, 1);

  MsTimer2::set(2, flash); //2ms Timer Interrupt발생시킴
  MsTimer2::start();

  RTC.read(tm);
  rtcTime = buttonTime = makeTime(tm);
  if (tm.Year < 48) RTC.set(1524790800); //2018년 이전이면 2018.04.27.01:00으로 셋
}

void loop() {
  unsigned short ai2;

  button.update();

  while (Serial.available()) {
    char c;
    c = Serial.read();

    if (gps.encode(c)) {
      if (gps.time.isValid()) {
        gm.Year = CalendarYrToTm(gps.date.year());
        gm.Month = gps.date.month();
        gm.Day = gps.date.day();
        gm.Hour = gps.time.hour();
        gm.Minute = gps.time.minute();
        gm.Second = gps.time.second();
        if (gps.time.hour() == 99) wifiset = false;
        else if (gps.time.hour() == 88) wifiset = true;
        else {
          if ((gm.Hour != 0) || (gm.Minute != 0) || (gm.Second != 0)) { //9:00로 초기화 되는 현상 막기 위함
            wifiset = false;
            //gpsTime = makeTime(gm) + 9 * 60 * 60; //이부분 있으면 콜론 비정상적 깜빡이는 버그 발생됨
            gpsLock = true;
          }
        }
      }
    }
  }

  ai2 = analogRead(2);

  //changemode();

  if (RTCread) {
    if (gpsLock) {
      rtcTime = gpsTime = makeTime(gm) + 9 * 60 * 60;
      RTC.set(gpsTime);
      gpsLock = false;
    }
    else {
      rtcTime = RTC.get();
      RTCread = false;
    }
  }

  if (pfcnt != fcnt && (fcnt % 5 == 0)) {
    if (ai2 < 600) ai2 = 600;
    if (ai2 > 900) ai2 = 900;
    ai2 = map(ai2, 600, 900, SEGHIGH, SEGMIN);
    if (brightness < ai2) brightness++;
    if (brightness > ai2) brightness--;
  }
  //digitalWrite(2, ((rtcTime%10==0) && (fcnt%2==0))?HIGH:LOW); //정시 출력할때
  pfcnt = fcnt;

  switch (display_mode) {
    case OFF:
      break;
    case NORMAL:
      break;
    case MMDD:
      if (rtcTime - buttonTime > 2) display_mode = NORMAL;
      break;
    case SEC:
      break;
    case START:
      //if (rtcTime - buttonTime > 10) display_mode = NORMAL;
      break;
  } // End of switch
}
