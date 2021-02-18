#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define BUZZER_PIN 20
#define LED_PIN 21

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {22,23,24,25};
byte colPins[COLS] = {26,27,28,29};
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'c', '0', '=', '+'}
};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

String candidates[4][4];
byte voteNums[4] = {0 , 0 , 0 , 0};

void setup()
{
  // put your setup code here, to run once:
  pinMode(BUZZER_PIN , OUTPUT);
  pinMode(LED_PIN , OUTPUT);

  lcd.begin(20 , 4);
  lcd.clear();

  lcd.setCursor(0 , 1);
  lcd.print("  initializations!");
  lcd.setCursor(0 , 2);
  lcd.print("    please wait!  ");
  digitalWrite(LED_PIN , HIGH);
  

  Serial.begin(9600);
  for(int i=0 ; i<4 ; i++)
  {
    for(int j=0 ; j<4 ; j++)
    {
      while(Serial.available() == 0);
      candidates[i][j] = Serial.readStringUntil('\n');
    }
  }
  digitalWrite(LED_PIN , LOW);

  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("   welcome to the");
  lcd.setCursor(0 , 1);
  lcd.print("   voting machine");
  lcd.setCursor(0 , 2);
  lcd.print("     vote  -> +");
  lcd.setCursor(0 , 3);
  lcd.print(" more details  -> /");  
}

void loop() 
{
  // put your main code here, to run repeatedly:
  key = keypad.getKey();
  if(key != NO_KEY)
  {
    if(key == '+')
    {
      lcd.clear();
      lcd.setCursor(0 , 1);
      lcd.print("  Which one do you");
      lcd.setCursor(0 , 2);
      lcd.print("    want to vote");
      delay(200);
      for(int i = 0 ; i < 4 ; i++)
      {
        lcd.clear();
        lcd.setCursor(0 , 0);
        lcd.print("Name: ");
        lcd.print(candidates[i][0]);
        lcd.setCursor(0 , 1);
        lcd.print("Age: ");
        lcd.print(candidates[i][1]);
        lcd.setCursor(0 , 2);
        lcd.print("profession:");
        lcd.print(candidates[i][2]);
        lcd.setCursor(0 , 3);
        lcd.print("Grade:");
        lcd.print(candidates[i][3]);
        lcd.setCursor(10 , 3);
        lcd.print("code: ");
        lcd.print(i+1);
        delay (300);
      }
      lcd.clear();
      for(int i = 0; i < 4 ; i++)
      {
        lcd.setCursor(0 , i);
        lcd.print(candidates[i][0]);
        lcd.setCursor(18 , i);
        lcd.print(i+1);
      }
      key = keypad.waitForKey();
      while(key != '1' && key != '2' && key != '3' && key != '4' && key != '*')
      {
        lcd.clear();
        lcd.setCursor(0 , 1);
        lcd.print("     wrong key!");
        delay(100);
        lcd.clear();
        for(int i = 0; i < 4 ; i++)
        {
          lcd.setCursor(0 , i);
          lcd.print(candidates[i][0]);
          lcd.setCursor(18 , i);
          lcd.print(i+1);
        }
        key = keypad.waitForKey();
      }
      if(key != '*')
      {
          byte vote = key - 49;
          Serial.write(vote);
          lcd.clear();
          lcd.setCursor(0 , 1);
          lcd.print("  thanks for your");
          lcd.setCursor(0 , 2);
          lcd.print("        VOTE");
          digitalWrite(BUZZER_PIN , HIGH);
          digitalWrite(LED_PIN , HIGH);
          delay(200);
          digitalWrite(BUZZER_PIN , LOW);
          digitalWrite(LED_PIN , LOW);
        }
    }
    else if(key == '/')
    {
      lcd.clear();
      for(int i = 0; i < 4 ; i++)
      {
        lcd.setCursor(0 , i);
        lcd.print(candidates[i][0]);
        lcd.setCursor(18 , i);
        lcd.print(i+1);
      }
      key = keypad.waitForKey();
      while(key != '1' && key != '2' && key != '3' && key != '4' && key != '*')
      {
        lcd.clear();
        lcd.setCursor(0 , 1);
        lcd.print("     wrong key!");
        delay(200);
        lcd.clear();
        for(int i = 0; i < 4 ; i++)
        {
          lcd.setCursor(0 , i);
          lcd.print(candidates[i][0]);
          lcd.setCursor(18 , i);
          lcd.print(i+1);
        }
        key = keypad.waitForKey();
      }
      if(key != '*')
      {
        int choose = key - 49;
        lcd.clear();
        lcd.setCursor(0 , 0);
        lcd.print("Name: ");
        lcd.print(candidates[choose][0]);
        lcd.setCursor(0 , 1);
        lcd.print("Age: ");
        lcd.print(candidates[choose][1]);
        lcd.setCursor(0 , 2);
        lcd.print("profession:");
        lcd.print(candidates[choose][2]);
        lcd.setCursor(0 , 3);
        lcd.print("Grade:");
        lcd.print(candidates[choose][3]);

        lcd.setCursor(10 , 3);
        lcd.print("exit -> *");
        while (key != '*')
        {
          key = keypad.waitForKey();
        }
      }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0 , 1);
      lcd.print("     wrong key!");
      delay(200);
    }
    lcd.clear();
    lcd.setCursor(0 , 0);
    lcd.print("   welcome to the");
    lcd.setCursor(0 , 1);
    lcd.print("   voting machine");
    lcd.setCursor(0 , 2);
    lcd.print("     vote  -> +");
    lcd.setCursor(0 , 3);
    lcd.print(" more details  -> /");  
  }
}
