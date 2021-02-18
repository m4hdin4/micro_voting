#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

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
String password = "";

String candidates[4][4] = {  //name               age    in politics  grade 
                             {"Mr.Ahmadinejad"  , "65" , "1999-2012" , "A"},
                             {"Mr.Roohany"      , "72" , "1990-2020" , "A"},
                             {"Mr.Ghalibaf"     , "59" , "2000-2020" , "B"},
                             {"Mr.MirSalim"     , "73" , "1990-2016" , "C"}
                          };
byte voteNums[4] = {0 , 0 , 0 , 0};
byte value;
boolean isIn = false;

void setup()
{
  // put your setup code here, to run once:
  lcd.begin(20 , 4);
  pinMode(LED_PIN , OUTPUT);
  digitalWrite(LED_PIN , HIGH);

  
  if(EEPROM.read(0) != 0)
  {
    EEPROM.write(0 , 0);
    lcd.clear();
    lcd.setCursor(0 , 0);
    lcd.print("your default pass is");
    lcd.setCursor(0 , 1);
    lcd.print("        1234");
    lcd.setCursor(0 , 2);
    lcd.print(" you can change it ");
    lcd.setCursor(0 , 3);
    lcd.print(" whenever you want ");
    password = "1234";
    EEPROM.write(100 , '1');
    delay(10);
    EEPROM.write(101 , '2');
    delay(10);
    EEPROM.write(102 , '3');
    delay(10);
    EEPROM.write(103 , '4');
    delay(10);
    EEPROM.write(104 , '$');
    delay(10);
    for(int i = 0 ; i < 4 ; i++)
    {
      EEPROM.write(8+i , 0);
      delay(100);
    }
  }
  else
  {
    for(int i = 0; i < 4 ; i++)
    {
      voteNums[i] = EEPROM.read(8 + i);
      delay(10);
    }
    int i = 100;
    char p;
    password = "";
    while ((p = (char)EEPROM.read(i)) != '$'){
      password += p;
      i++;
      delay(10);
    }
  }
  lcd.clear();
  lcd.setCursor(0 , 1);
  lcd.print("  initializations!");
  lcd.setCursor(0 , 2);
  lcd.print("    please wait!  ");


  
  Serial.begin (9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  delay(100);

  for(int i=0 ; i<4 ; i++)
  {
    for(int j=0 ; j<4 ; j++)
    {
      Serial.println (candidates[i][j]);
      Serial1.println(candidates[i][j]);
      Serial2.println(candidates[i][j]);
      Serial3.println(candidates[i][j]);
      delay(5);
    }
  }

  digitalWrite(LED_PIN , LOW);
      
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(!isIn)
  {
    lcd.clear();
    lcd.setCursor(0 , 0);
    lcd.print(" please enter your");
    lcd.setCursor(0 , 1);
    lcd.print("      password");
    lcd.setCursor(0 , 3);
    lcd.print("    = to confirm");
    lcd.setCursor(0 , 2);
    String pass = "";
    while (true)
    {
      key = keypad.waitForKey();
      if (isDigit(key))
      {
        lcd.print("*");
        pass += key;
      }
      else if (key == '=')
      {
        if (pass == password) 
        {
          digitalWrite(LED_PIN , HIGH);
          lcd.clear();
          lcd.setCursor(0 , 0);
          lcd.print("   ==============");
          lcd.setCursor(0 , 1);
          lcd.print("   | YOU ARE IN |");
          lcd.setCursor(0 , 2);
          lcd.print("   ==============");
          delay(400);
          isIn = true;
          digitalWrite(LED_PIN , LOW);
          break;
        }
        else 
        {
          digitalWrite(LED_PIN , HIGH);
          lcd.clear();
          lcd.setCursor(0 , 1);
          lcd.print("  WRONG PASSWORD!!");
          delay(200);
          digitalWrite(LED_PIN , LOW);
          lcd.clear();
          lcd.setCursor(0 , 0);
          lcd.print(" please enter your");
          lcd.setCursor(0 , 1);
          lcd.print("      password");
          lcd.setCursor(0 , 3);
          lcd.print("    = to confirm");
          lcd.setCursor(0 , 2);
          pass = "";
        }
      }
      else if (key == 'c') 
      {
        lcd.clear();
        lcd.setCursor(0 , 0);
        lcd.print(" please enter your");
        lcd.setCursor(0 , 1);
        lcd.print("      password");
        lcd.setCursor(0 , 3);
        lcd.print("    = to confirm");
        lcd.setCursor(0 , 2);
        pass = "";
      }
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("     MAIN MENU:    ");
    lcd.setCursor(0,1);
    lcd.print("see votes         1");
    lcd.setCursor(0,2);
    lcd.print("reset votes       2");
    lcd.setCursor(0,3);
    lcd.print("change password   3");  
  }
  else
  {
    key = keypad.getKey();
    if(key != NO_KEY)
    {
      if (key == '1')
      {
        lcd.clear();
        for(int i = 0 ; i < 4 ; i++)
        {
          lcd.setCursor(0 , i);
          lcd.print(candidates[i][0]);
          lcd.setCursor(17 , i);
          lcd.print(voteNums[i]);
        }
        keypad.waitForKey();
      }
      else if(key == '2')
      {
        lcd.clear();
        lcd.setCursor(0 , 0);
        lcd.print("  Are you sure you  ");
        lcd.setCursor(0 , 1);
        lcd.print("   want to delete   ");
        lcd.setCursor(0 , 2);
        lcd.print("     all votes?     ");
        lcd.setCursor(0 , 3);
        lcd.print("  = YES        * NO");
  
        key = keypad.waitForKey();
        while (key != '=' && key != '*')
        {
          key = keypad.waitForKey();
        }
        if(key == '=')
        {
          digitalWrite(LED_PIN , HIGH);
          lcd.clear();
          lcd.setCursor(0 , 1);
          lcd.print("    reseting ...");
          for(int i = 0 ; i < 4 ; i++)
          {
            voteNums[i] = 0;
            EEPROM.write(8+i , 0);
            delay(100);
          }
          lcd.setCursor(0 , 2);
          lcd.print("        Done");
          digitalWrite(LED_PIN , LOW);
          delay(200);
        }
      }
      else if(key == '3')
      {
        lcd.clear();
        lcd.setCursor(0 , 1);
        lcd.print("enter your new pass:");
        lcd.setCursor(0 , 3);
        lcd.print("    = to confirm");
        lcd.setCursor(0 , 2);
        String pass = "";
        while (true)
        {
          key = keypad.waitForKey();
          if (isDigit(key))
          {
            lcd.print(key);
            pass += key;
          }
          else if (key == '=' && pass.length() > 3)
          {
            lcd.clear();
            lcd.setCursor(0 , 0);
            lcd.print("   are you sure??");
            lcd.setCursor(0 , 1);
            lcd.print("your new password is");
            lcd.setCursor(8 , 2);
            lcd.print(pass);
            lcd.setCursor(0 , 3);
            lcd.print("  = YES        * NO");
            key = keypad.waitForKey();
            while (key != '=' && key != '*')
            {
              key = keypad.waitForKey();
            }
            if(key == '=')
            {
              digitalWrite(LED_PIN , HIGH);
              lcd.clear();
              lcd.setCursor(0 , 1);
              lcd.print("  wait a moment ..");
              password = pass;
              pass+='$';
              for(int i = 0; i < pass.length(); i++){
                EEPROM.write(100 + i , pass.charAt(i));
                delay(10);
              }
              lcd.setCursor(0 , 2);
              lcd.print("  password changed");
              digitalWrite(LED_PIN , LOW);
              delay(200);
            }
            break;
          }
          else if (key == 'c')
          {
            lcd.clear();
            lcd.setCursor(0 , 1);
            lcd.print("enter your new pass:");
            lcd.setCursor(0 , 3);
            lcd.print("    = to confirm");
            lcd.setCursor(0 , 2);
            pass = "";
          }
          else if(key == '*')
          {
            break;
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
      lcd.setCursor(0,0);
      lcd.print("     MAIN MENU:    ");
      lcd.setCursor(0,1);
      lcd.print("see votes         1");
      lcd.setCursor(0,2);
      lcd.print("reset votes       2");
      lcd.setCursor(0,3);
      lcd.print("change password   3");  
    }
  }
}
void serialEvent()
{
  while(Serial.available())
  {
    value = Serial.read();
    voteNums[value] ++ ;
    EEPROM.write(8+value , voteNums[value]);
  }
}
void serialEvent1()
{
  while(Serial1.available())
  {
    value = Serial1.read();
    voteNums[value] ++ ;
    EEPROM.write(8+value , voteNums[value]);
  }
}
void serialEvent2()
{
  while(Serial2.available())
  {
    value = Serial2.read();
    voteNums[value] ++ ;
    EEPROM.write(8+value , voteNums[value]);
  }
}
void serialEvent3()
{
  while(Serial3.available())
  {
    value = Serial3.read();
    voteNums[value] ++ ;
    EEPROM.write(8+value , voteNums[value]);
  }
  
}
