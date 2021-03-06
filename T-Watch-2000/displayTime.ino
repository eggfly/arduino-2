// The basic Time Display GUI
// if you are just updating the colon and seconds, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker
//
// This version displays the battery level, and the inactivity counter to help
// you see how this is working. Touching the screen resets the inactivity counter. 

#include <time.h>

byte xcolon = 0; // location of the colon
byte xss = 0;    // location of the seconds digits

void displayTime(boolean fullUpdate) {

  byte xpos = 40; // Stating position for the display
  byte ypos = 50; //90
  
  // Font 7 is a 7-seg display but only contains
  // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
  const int font = 4;
  const uint8_t hhDigitSize = 2;
  const uint8_t mmDigitSize = 2;
  const uint8_t ssDigitSize = 1;
  const uint8_t ssDigitVOffset = 19; //No. of pixels to position seconds digits lower than hh:mm
  const uint16_t colour = TFT_RED; //0xFBE0=orange
  
  // Get the current data
  RTC_Date tnow = ttgo->rtc->getDateTime();

  hh = tnow.hour;
  mm = tnow.minute;
  ss = tnow.second;
  dday = tnow.day;
  mmonth = tnow.month;
  yyear = tnow.year;

  ttgo->tft->setTextSize(hhDigitSize);

  if (fullUpdate) {
    //Show battery level
    ttgo->tft->setTextColor(TFT_BLACK, TFT_WHITE); 
    ttgo->tft->setCursor( 100, 2);
    ttgo->tft->print("Batt: ");
    if (batteryLevel < 10) ttgo->tft->print(0);
    ttgo->tft->print(batteryLevel);
    ttgo->tft->print("% ");

    //Display time
    //ttgo->tft->setTextColor(0x39C4, TFT_BLACK); // Set desired color
    //ttgo->tft->drawString("88:88", xpos, ypos, font);
    ttgo->tft->setTextColor(colour, TFT_BLACK); 
    if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, font);
    xpos += ttgo->tft->drawNumber(hh, xpos, ypos, font);
    xcolon = xpos + 3;
    ttgo->tft->setTextSize(mmDigitSize);
    xpos += ttgo->tft->drawChar(':', xcolon, ypos, font);
    xpos += 6;
    if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, font);
    xpos += ttgo->tft->drawNumber(mm, xpos, ypos, font);
    xpos += 6;
    xss = xpos;
    ttgo->tft->setTextSize(ssDigitSize);
    if (ss < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos+ssDigitVOffset, font);
    ttgo->tft->drawNumber(ss, xpos, ypos+ssDigitVOffset, font);
    ttgo->tft->setTextSize(hhDigitSize);
  }
  else {
    ttgo->tft->setTextColor(colour, TFT_BLACK);
    ttgo->tft->setTextSize(ssDigitSize);
    xpos = xss;
    if (ss < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos+ssDigitVOffset, font);
    ttgo->tft->drawNumber(ss, xpos, ypos+ssDigitVOffset, font);
    ttgo->tft->setTextSize(hhDigitSize);
    
    if (ss % 2) { // Toggle the colon every second
      ttgo->tft->setTextColor(0x39C4, TFT_BLACK);
      ttgo->tft->drawChar(':', xcolon, ypos, font);
      ttgo->tft->setTextColor(colour, TFT_BLACK);
    } else {
      ttgo->tft->drawChar(':', xcolon, ypos, font);
    }
  }

  //Display date
  ttgo->tft->setTextSize(3);
  ttgo->tft->setCursor( 40, 210);
  ttgo->tft->print(dday);
  ttgo->tft->print("/");
  ttgo->tft->print(mmonth);
  ttgo->tft->print("/");
  ttgo->tft->print(yyear);

  //Show active time
  ttgo->tft->setTextSize(2);
  ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
  ttgo->tft->setCursor( 5, 170);
  ttgo->tft->print("Inactive time: ");
  int activeTime = (millis()-lastActiveTime)/1000;
  if (activeTime < 10) ttgo->tft->print(0);
  ttgo->tft->print(activeTime);

}
