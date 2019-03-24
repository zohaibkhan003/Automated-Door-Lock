/*
 * Author : Zohaib Khan
 */



#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h> 


LiquidCrystal lcdScreen(12,11,5,4,3,2); 
Servo Servo1; 
char password[5]; //array for odd password
char beginningPassword[5];
char changedPassword[5]; // array for new password
char displayPassword[4];
char pressedButton = 0; // keypad is pressed
char pressedKey = 0;
int sensor = 5; // A5 mode for photoresistor
long sensorValue = 0; 
long largestValue = 0; // sensors uses to make sure in night time
int blueTooth = 0;
int timeVal = 0; // time value 
int led = 10;
int led1 = 13;  //led connected in pin 13
int servoPin = 6; 
int i=0;
int j,k,y,z; // used for while and for loops

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {A4, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2, A3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
   Serial.begin(9600); //intializes bluetooth
   Servo1.attach(servoPin); // intializes servo
   pinMode(servoPin, OUTPUT); // sets the pin as output
   pinMode(led, OUTPUT);
   pinMode(led1,OUTPUT);
   lcdScreen.begin(16,2);// intializes lcd
   enterPassword();
   initialpassword(); // gets into intial password written from eeprom
}

void loop() {

  if(pressedButton=='*'){ // if pressed will go to change password
    change();
    delay(500);
    enterPassword();
    i = 0;
  }
  
if (pressedButton == 'A'){ // if pressed will go to night mode
   nightMode();
    delay(1500);
    enterPassword();
    i = 0;
}

if (pressedButton == 'B'){ // if pressed will go to bluetooth mode
   bluetoothMode();
   delay(1500);
   enterPassword();
   i = 0;
}

//////////// all modes done //////////////  
  pressedButton = keypad.getKey(); // gets button pressed and puts in variable
  if(pressedKey =='C'|| pressedKey =='D'||pressedKey =='1'|| pressedKey =='2'|| pressedKey =='3'|| pressedKey =='4'|| pressedKey =='5'|| pressedKey =='6'|| pressedKey =='7'|| pressedKey =='8'|| pressedKey =='9'|| pressedKey =='0'|| pressedKey =='#'){
){             
    lcdScreen.print("*");       
    password[i]=pressedButton;    // will store all char values in the password array 
    i = i + 1;
  }
  if(i == 5){
    delay(200);                     // if i = 5 which means that 5 values came from the user
    j = 1;
    for(j = 1;j <= 5;j++){
      beginningPassword[j]=EEPROM.read(j); // it reads from the password of eeprom and puts in an array aswell
    }

    if((strncmp(password, beginningPassword,5))== 0){ // then the password that was saved and the user password are compared
       servoClose(); // password accepted   
       lcdOpen();   // writes closed to lcd
       delay(3000);
       i=0;
    }
    else if ((strncmp(password, beginningPassword,5))!= 0){
      servoOpen();  // password unaccepted
      lcdClosed();    // writes open to lcd
      delay(3000); 
      i=0;
    }
  }
}


//////////////////////////// ***********************ALL FUNCTION CALLS***************************** //////////////////////////////////////////////////////
void initialpassword(){     
   EEPROM.write(0,'1');
   EEPROM.write(1,'2');
   EEPROM.write(2,'3');  // puts 1234# in password if user hasnt changed it
   EEPROM.write(3,'4');
   EEPROM.write(4,'#');
   
   beginningPassword[0]=EEPROM.read(0);
   beginningPassword[1]=EEPROM.read(1);
   beginningPassword[2]=EEPROM.read(2);
   beginningPassword[3]=EEPROM.read(3);
   beginningPassword[4]=EEPROM.read(4);
} 
void servoClose(){      // closes servo and outputs a led
    analogWrite(servoPin, 255);
    Servo1.write(100);
    digitalWrite(led, HIGH);
    digitalWrite(led1, LOW);
}
void servoOpen(){       // opens servo and outputs a led
     analogWrite(servoPin, 0);
     Servo1.write(10); 
     digitalWrite(led1, HIGH);
     digitalWrite(led, LOW);
}
void lcdClosed(){     //lcd prints closed
   lcdScreen.clear();
   lcdScreen.print("Locked Closed");
   lcdScreen.setCursor(0,1);
}
void lcdOpen(){      //lcd prints open
   lcdScreen.clear();
   lcdScreen.print("Lock Open");
   lcdScreen.setCursor(0,1);
}

void enterPassword(){    //prompts user to enter password
   lcdScreen.clear();
   lcdScreen.print("Enter Password");
   lcdScreen.setCursor(0,1);
}

void wrongPassword(){  //prompts user that it is incorrect password
    lcdScreen.clear();
    lcdScreen.print("Wrong Password");
    delay(1200);
    lcdScreen.clear();
    lcdScreen.print("Please Press");
    lcdScreen.setCursor(0,1);
    lcdScreen.print("* to try again");
    delay(2000);
}

void oldPassword(){       // prompt the user to enter old password when they are changing passwords
  lcdScreen.clear();
  lcdScreen.print("To Change");
  lcdScreen.setCursor(0,1);
  lcdScreen.print("Old Password");
  delay(3000);
  lcdScreen.clear();
  lcdScreen.print("Please Enter");
  delay(2000);
  lcdScreen.clear();
  lcdScreen.print("Current Password:");
  lcdScreen.setCursor(0,1);
  
}


void change()
{
  int y = 0;
  pressedKey = 0;
  pressedButton = 0;
  int i = 0;
  int j = 0;

  oldPassword();

  while ( j < 4){       // allows user to enter 4 values from the keypad which will store the 4 values in a char value.
    pressedKey = keypad.getKey();
    if (pressedKey){
       if (pressedKey == 'A' || pressedKey == '*' || pressedKey == 'B'){  // when user eneters password, when they enter an A or * it will turn into a 0.
        pressedKey = '0';
      }
      changedPassword[j++] = pressedKey;
      lcdScreen.print(pressedKey);
    }
    pressedKey = 0;
  }
  changedPassword[4] = '#';
  delay (500);

 if((strncmp(changedPassword, beginningPassword, 5))!= 0){    // if the password entered and current password are no the same, it will prompt wrong password
    wrongPassword();
  }
  else if ((strncmp(changedPassword, beginningPassword, 5))== 0){ // if same will allow you to enter a new password
    j=0;
    lcdScreen.clear();
    lcdScreen.print("New Password:");
    lcdScreen.setCursor(0,1);
    for (j = 0; j < 4; j++)
    {
      pressedKey=keypad.getKey();
      if(pressedKey == 'A' || pressedKey == 'B'||pressedKey =='C'|| pressedKey =='D'||pressedKey =='1'|| pressedKey =='2'|| pressedKey =='3'|| pressedKey =='4'|| pressedKey =='5'|| pressedKey =='6'|| pressedKey =='7'|| pressedKey =='8'|| pressedKey =='9'|| pressedKey =='0'){
        if (pressedKey == 'A' || pressedKey == '*' || pressedKey == 'B'){  // when user changes password, when they enter an A or * it will turn into a 0.
        pressedKey = '0';
      }
        beginningPassword[j]=pressedKey;
        lcdScreen.print(pressedKey); 
        delay(1000);                  // will write the new password in eeprom
        EEPROM.write(j,pressedKey);
        j++;
      }
    } 
    lcdScreen.clear();
    lcdScreen.print("Password Changed");
    delay(4000);
    lcdScreen.clear();
    lcdScreen.print("New Password is");
    lcdScreen.setCursor(0,1);
    displayPassword[0] = EEPROM.read(0);
    displayPassword[1] = EEPROM.read(1);
    displayPassword[2] = EEPROM.read(2);  // allows the user to see what the new password they have entered and will be displayed on the lcd screen
    displayPassword[3] = EEPROM.read(3);
    lcdScreen.print(displayPassword[0]);
    lcdScreen.print(displayPassword[1]);
    lcdScreen.print(displayPassword[2]);
    lcdScreen.print(displayPassword[3]);
    delay(5000);
    pressedButton = 0;
    pressedKey = 0;
  }
  }


void nightMode(){
    lcdScreen.clear();
   lcdScreen.print("Night Mode On");
   lcdScreen.setCursor(0,1);
   lcdScreen.print("Please Wait...");
  while (timeVal <= 100){       // the mode last thing long - right now very small due to demo purposes
       delay(100);
      Serial.println(sensorValue); 
       ++timeVal;
  }
  if( timeVal > 100){                         // when time runs out it will read many anolog signals from the sensor
    while (z < 100){
      sensorValue = analogRead(sensor);
      Serial.print(sensorValue);
      Serial.print('\n');
      if ( sensorValue > largestValue){     // this gets the number. Beacause of my setup the highest value will determine if there is a lack of  light.
        largestValue = sensorValue;
      }
       //Serial.println(largestValue); 
       delay(100);
       ++z;
    }
  if ( largestValue > 1000){ //10231023
    servoOpen(); // password accepted         // if night time - it closes and displays closes
    lcdClosed();   // writes closed to lcd
    delay(1000);
  }
 }
   timeVal = 0;
   pressedButton = 0;
   z = 0;
  }


void bluetoothMode(){       // enables bluetooth mode
char keypressed = 0;
lcdScreen.clear();
lcdScreen.print("BlueTooth Mode:");
lcdScreen.setCursor(0,1);
lcdScreen.print("ON");
while (keypressed != 'B'){
  keypressed = keypad.getKey();
if(Serial.available() > 0)      // Send data only when you receive data:
   {
      blueTooth = Serial.read();        //Read the incoming data & store into data   
      if(blueTooth == 48){
          servoOpen(); // password accepted
          lcdClosed();   // writes closed to lcd
      }
      else if(blueTooth == 49) {    // values of the button on my phone
          servoClose();
          lcdOpen();
      }
   }
}
   lcdScreen.clear();
   lcdScreen.print("BlueTooth Mode:"); // turns off the bluetooth mode
   lcdScreen.setCursor(0,1);       
   lcdScreen.print("OFF");
   pressedButton = 0;
   keypressed = 0;
}
