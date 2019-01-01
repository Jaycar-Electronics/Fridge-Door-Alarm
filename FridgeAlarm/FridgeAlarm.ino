/*
 * Adjustable Fridge Door Alarm - Jaycar Electronics
 * Author Dre West
 * 
 * This project has a buzzer module attached on pin 3
 * and a reed switch attached on A1.
 * 
 * The controller module buttons automatically go to A0.
 * 
 */

// include the library code:
#include <LiquidCrystal.h>

#define BUZZER 3 //buzzer is on pin 3.
#define LATCH A1 //latch is on A1
#define BUTTON A0 //buttons from shield are all connected on A0

//these are the analog readings of each button press.
//they should be the same for your unit aswel.
// if not, make a simple sketch to Serial.println(AnalogRead(A0)); to find new values
//we have divided these values by 10 to count for slight varience in the values  ie: (1023 instead of 1024)
#define NO_BUTTON 102
#define MENU_UP 10
#define MENU_DOWN 25
#define MENU_SELECT 64
#define MENU_LEFT 40 
#define MENU_RIGHT 0


LiquidCrystal lcd(8,9,4,5,6,7);

long timer = 0;
bool timerEnabled = false;
bool soundAlarm = false;
long deltaTime = 5 * 1000; //5 seconds

int button = NO_BUTTON;
bool menuOpen = false;

   
void setup() {
	lcd.begin(16, 2);
	pinMode(BUTTON,INPUT);
	pinMode(LATCH,INPUT_PULLUP);
	pinMode(BUZZER,OUTPUT);

 Serial.begin(9600);
 Serial.println("allok");

}


void loop() {
  lcd.clear();
  lcd.print("Fridge Alarm");
  lcd.setCursor(0,1);
	//each loop, wait for either the door latch or menu buttons.
  
  do
  {
    button = analogRead(BUTTON) / 10;
    
    lcd.setCursor(0,1);
    delay(100);
  	if(digitalRead(LATCH) == HIGH)//latch is open. 
  	{               
      lcd.print("            OPEN");
      
  		//have we got a timer? if not, start one
  		if(timerEnabled == false)
  		{
        Serial.println("timerEnabled");
  			timerEnabled = true;
  			timer = millis();
  		}
  		else if (millis() - timer > deltaTime) //we have a timer and
  		{ //if the elapsed time 
  			//is greater than what we specified
  			soundAlarm = true;
  		}

     //some debugging output
     Serial.print("latch opened at: ");
     Serial.print(timer,DEC);
     Serial.print("   current: ");
     Serial.print(millis(),DEC);
     Serial.print("   delta: ");
     Serial.print(millis() - timer,DEC);
     Serial.print("   out of ");
     Serial.print(deltaTime,DEC);
     Serial.println();
  	}
  	else //latch is closed
  	{
      lcd.print("          CLOSED");
  		//never have the alarm sounding or timer when latch is closed
  		soundAlarm = false;
  		timerEnabled = false;
      timer = 0;
  	}
  
  	if(soundAlarm) //are we sounding an alarm?
  	{
      Serial.println("ARLM");     
  		digitalWrite(BUZZER,HIGH);
  		delay(100);
      digitalWrite(BUZZER,LOW);
      delay(100);
  	}

   
  }while(button == NO_BUTTON);

  
  soundAlarm = false; //stop the timer/alarm because the user knows
  timerEnabled = false;
  
  //show the menu and get a new deltaTime;
  deltaTime = getValueFromMenu(deltaTime);

  Serial.println("deltaTime changed to");
  Serial.print(deltaTime,DEC);
  Serial.println();
  delay(1000);
}

long getValueFromMenu(long delta_time) //display a menu to the user and return a new deltavalue
{
  String value;
  long new_time = delta_time;
  short p;
  bool menu_open = true;
  
  while(menu_open)
  {
  
    //clear the screen and show the heading
    lcd.clear();
    lcd.print("Set Timer (ms)");
    lcd.setCursor(0,1);
    
    String value = String(new_time); //convert deltaTime into a String object
    p = value.length();               //get the length of the string
    
    lcd.setCursor(16-p,1); //right align by setting the cursor 'p' amount away from the right
    lcd.print(value.c_str()); //print the string
  
  
    button = analogRead(BUTTON) / 10;
    
    Serial.print(button,DEC);
    Serial.println();
    switch(button)
    {
      case MENU_SELECT:
      Serial.println("menu false");
        menu_open = false;
        break;
        
      case MENU_UP:
        new_time += 100;
        break;
      case MENU_DOWN:
        new_time -= 100;
        break;
      case MENU_LEFT:
        new_time += 1000;
        break;
      case MENU_RIGHT:
        new_time -= 1000;
        break;
        
      
    }
  delay(100);
  }
    return new_time;
}

