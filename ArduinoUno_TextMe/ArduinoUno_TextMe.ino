#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message;
int totalLines;
int buttonPressed = 0;
int currentLine = 0;

//function to get substring for Nth line
String GetLine(String message, int lineNr = 0){
  return message.substring(lineNr*16,lineNr*16+16);
}

void DisplayMessage(String message, int lineNr){
  message = message + "                ";
  lcd.setCursor(0, 0);
  lcd.print(GetLine(message,lineNr));
  lcd.setCursor(0, 1);
  lcd.print(GetLine(message,lineNr+1));

}

// maybe review this so that it will not pause everything...
void Beep(){   
  tone(8, 1479.98, 175);
  delay(175);
  noTone(8);
  delay(175);
  tone(8, 1479.98, 175);
  delay(175);
  noTone(8);
  delay(600); //
  tone(8, 1479.98, 175);
  delay(175);
  noTone(8);
  delay(175);
  tone(8, 1479.98, 175);
  delay(175);
  noTone(8);
}

void setup() {
  Wire.begin(9); // 9 is the address of this Arduino as a slave
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(13, OUTPUT); //turn off internal led
  pinMode(8,OUTPUT); //passive speaker
  lcd.begin(16, 2);
  //Beep(); //it was here just to test beeping

  // Print the stanby message to the LCD.
  message="Waiting for     message...";
  //message="Things may not have gone as planned last week for the flying cellphone on Mars, but just because Ingenuitys flying career is over doesnt mean theres no more work to do.";
  DisplayMessage(message,0);
}

void UpdateCurrentLine(){
  buttonPressed = ReadButtons();
  totalLines = message.length()/16; //will be rounded down, so the number will indicate the second to last line
  if(buttonPressed==2 && currentLine<totalLines-1){  //increase page number
    currentLine+=1;
    DisplayMessage(message,currentLine);
    delay(500);
  } else if(buttonPressed==1 && currentLine>0){  //decrease page number
    currentLine-=1;
    DisplayMessage(message,currentLine);
    delay(500);
  }
}

void loop() {
  /*
  check for incoming I2C communication
    if comunication exists
      receive char array
      update message string
      display line 0
      call Beep()
  */

  UpdateCurrentLine();
  
  
}





int ReadButtons(){
  int A0Value = analogRead(A0);
  if(A0Value<100){
    return 1;     //button 1
  } else if(A0Value>400 && A0Value<600) {
    return 2;     //button 2
  } else if(A0Value>900) {
    return 0;     //no button
  } else return -1;  //error
}

void receiveEvent() {
  while (Wire.available()) {
    char incomingChar = Wire.read();
    Serial.print(incomingChar);
  }
}
