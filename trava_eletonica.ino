/*
#################TRAVA ELETRÔNICA COM SENHA#################

	O sistema registra uma senha de 8 caracteres. Depois de 
registrar a senha a trava permanecerá trancada até que a se-
nha correta seja digitada. Depois de aberta, ao apertar o bo-
tão no pino 9 a trava é trancada novamente.
	A trava é representada pelo servo no pino 10.
    
** NO TECLADO A TECLA 'A' CONFIRMA E A TECLA 'B' CANCELA **

############################################################
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>



const int rs = 12, en = 11, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
const int speaker = 13, btnpin = 9;
const byte numRows= 4;
const byte numCols= 4;
bool welcome = false;

String password = String("");
String strentry = String("");
char keypressed;
char keymap[numRows][numCols]= { {'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'} };

byte rowPins[numRows] = {7,6,5,4}; //Rows 0 to 3
byte colPins[numCols]= {3,2,1,0}; //Columns 0 to 3

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo door;
Keypad keypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setpass() //Configura uma nova senha
{
	lcd.setCursor(0, 0);
  	lcd.print("Nova Senha");
  
    while (password.length() < 8)
    {
     	keypressed = keypad.getKey();
		if (keypressed != NO_KEY)
        {
          	tone(speaker, 500, 30);
          	password = password + keypressed;
        }
        lcd.setCursor(0, 1);
        lcd.print(password);
      	delay(100);
   	}
  	lcd.clear();
	lcd.setCursor(0, 0);
  	lcd.print(password);
  	lcd.setCursor(0, 1);
  	lcd.print ("Confirma?");
    keypressed = NO_KEY;
  	while (keypressed == NO_KEY || keypressed != 'A' || keypressed != 'B')
    {
      keypressed = keypad.getKey();
      if (keypressed == 'A')
      {
        tone(speaker, 500, 30);
        lcd.clear();
        lcd.setCursor(0, 0);
  		lcd.print("Senha Cadastrada");
        delay(2000);
        lcd.clear();
        break; 
      }
      if (keypressed == 'B')
      {
        tone(speaker, 500, 30);
        password = "";
        lcd.clear();
        delay(1000);
        break;
      }
    }  
}

void checkpass() //Verifica a entrada e compara com a senha
{
    while (strentry.length() < 8)
    {
     	keypressed = keypad.getKey();
		if (keypressed != NO_KEY)
        {	
          	tone(speaker, 500, 30);
          	strentry = strentry + keypressed;
        }
        lcd.setCursor((strentry.length() - 1), 0);
        lcd.print("*");
      	delay(100);
   	}
  	lcd.setCursor(0, 1);
  	lcd.print ("Confirma?");
    keypressed = NO_KEY;
  		while (keypressed == NO_KEY)
    	{
      		keypressed = keypad.getKey();
      		if (keypressed == 'A' && strentry == password)
      		{
       			tone(speaker, 500, 30);
        		strentry = "";
        		lcd.clear();
        		lcd.print("Bem Vindo!");
        		welcome = true;
        		delay(2000);
        		lcd.clear();
        		break; 
      		}  
      		if (keypressed == 'A' && strentry != password)
      		{
        		tone(speaker, 500, 30);
        		strentry = "";
        		lcd.clear();
        		lcd.print("Senha Errada!");
        		delay(2000);
        		lcd.clear();
        		break;
      		}
      		if (keypressed == 'B')
	      	{
		        tone(speaker, 500, 30);
		        strentry = "";
		        lcd.clear();
		        break;
		     }  
	    }  
}


void setup()
{
	lcd.begin(16, 2);
  	door.attach(10);
  	pinMode(speaker, OUTPUT);
  	pinMode(btnpin, INPUT);
}

void loop()
{
  	if (welcome == false)
    {
      	door.write(0);  
		if (password == "")
    	{
			setpass();	  
   		}else if (welcome == false)
    	{
			checkpass();
    	}
    }  
  	if (welcome == true)
    {
      door.write(90);
      if (digitalRead(btnpin) == HIGH) //Tranca a porta novamente
      {
            tone(speaker, 250, 30);
            lcd.print("Trancando...");
            delay(3000);
            door.write(0);
        	welcome = false;
        	lcd.clear();
      }
    }  
}