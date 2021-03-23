// MSFduino based on MSFTime samples program 
// Jarkman, 01/2011 
// http://www.jarkman.co.uk/catalog/robots/msftime.htm 
// Prerequisites: 
// An MSF time receiver, wired to analogue pin 0, like this: http://www.pvelectronics.co.uk/index.php?main_page=product_info&cPath=9&products_id=2 
// Time library: http://www.arduino.cc/playground/Code/Time 
// Library compatible with Arduino IDE 0023
#include <Time.h> // from http://www.arduino.cc/playground/Code/Time
#include "MSFTime.h"
#include <LiquidCrystal.h>
MSFTime MSF;// = MSFTime();
 time_t prevDisplay = 0; // when the digital clock was displayed
 byte prevStatus = 255;
 time_t msfTimeSync();
 LiquidCrystal lcd(12, 13, 5, 4, 3, 2);
 void setup()
 {
 lcd.begin(16, 2);
MSF.init( 255 ); // LED pin for status - pass 13 for the built-in LED on the Arduino, or 255 for no led at all
 // For reasons I do not understand, you cannot use this when running with USE_AVR_INTERRUPTS
lcd.clear();
delay(2000);
lcd.setCursor(4,0);
lcd.println("MSFduino");
delay(5000);
lcd.setCursor(1,1);
lcd.println("by Alex, g7kse");
delay(5000);
lcd.clear();
 setSyncProvider(msfTimeSync); // tell the Time library to ask for a new time value
}
void loop()
 {
 byte currStatus = MSF.getStatus();
if(currStatus != prevStatus || currStatus & MSF_STATUS_FIX)
 {
 if( currStatus != prevStatus )
 {
 if( currStatus & MSF_STATUS_CARRIER)
 lcd.println("Got carrier ");
 if( (currStatus & MSF_STATUS_WAITING))
 lcd.println("Waiting for sync");
 if( (currStatus & MSF_STATUS_READING))
 lcd.println("Reading fix");
 prevStatus = currStatus;
 }
now();
if( timeStatus()!= timeNotSet )
 {
 if( now() != prevDisplay) //update the display only if the time has changed
 {
 prevDisplay = now();
 digitalClockDisplay();
 }
 }
 }
 }
 void printDigits(int digits){
 // utility function for digital clock display: prints preceding colon and leading 0
 lcd.print(":");
 if(digits < 10)
 lcd.print('0');
 lcd.print(digits);
 }
 void digitalClockDisplay(){
 // digital clock display of the time
 lcd.print(hour());
 printDigits(minute());
 printDigits(second());
 lcd.print(" ");
 lcd.print(day());
 lcd.print(" ");
 lcd.print(month());
 lcd.print(" ");
 lcd.print(year());
 lcd.println();
 }
 time_t msfTimeSync() // called periodically by Time library to syncronise itself
 {
 return MSF.getTime();
 }
 /***************************************************************************************/
