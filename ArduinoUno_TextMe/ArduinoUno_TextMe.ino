#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String message;
String messageAux;
int totalLines;
int buttonPressed = 0;
int currentLine = 0;

//function to get substring for Nth line
String GetLine(String message, int lineNr = 0){
  return message.substring(lineNr*16,lineNr*16+16);
}

void DisplayMessage(String message, int lineNr){
  message = message + "                " + "                ";
  lcd.setCursor(0, 0);
  lcd.print(GetLine(message,lineNr));
  lcd.setCursor(0, 1);
  lcd.print(GetLine(message,lineNr+1));
}

//200ms tone in F#
void Beep(){
  tone(8, 1479.98, 200);
}

String formatMessage(String inputString) { //turns out this is more difficult than expected and it adds very little to the purpose of the project.
  String formattedString = inputString.substring(0,16); // this is the part with the name of the sender
  inputString = inputString.substring(16);  //this is the message itself
  
  int lastSpaceIndex = 0;
  int currentLineIndex = 0;
  String currentLine = "";
  
  //inputString = BreakLargeWords(inputString); disabled, I think this is breaking the ram size
  
  for(int i=0;i < inputString.length();i++ ){
    if((String)inputString[i]==" "){
      lastSpaceIndex = i;
      currentLineIndex = currentLine.length();
    }
    if(currentLine.length()==15 && (String)inputString[i]==" "){   //line ending with " "
      currentLine += inputString[i];
      formattedString += currentLine;
      currentLine = "";
    } else if(currentLine.length()==16 && (String)inputString[i]==" "){   //next line starting with " "
      formattedString += currentLine;
      currentLine = "";
    } else if(currentLine.length()<16){  //while there is space in the line, a character will be added
      currentLine += inputString[i];
    } else if (currentLine.length()==16) {  //if it reaches this if, means that for sure, a word is broken
      currentLine = currentLine.substring(0,currentLineIndex);
      currentLine += "                ";
      currentLine = currentLine.substring(0,16);
      formattedString += currentLine;
      currentLine = "";
      i = lastSpaceIndex;
    }
  }
  formattedString += currentLine;
  return formattedString;
}

//this breakes large words, adding spaces
String BreakLargeWords(String inputString){
  int nonSpaceCounter = 0;
  String stringAux = "";
  for(int i=0;i < inputString.length();i++ ){
    Serial.println((String)i+" "+(String)nonSpaceCounter);
    if((String)inputString[i]==" "){
      nonSpaceCounter = 0;
    } else { 
      nonSpaceCounter++;
    }
    if(nonSpaceCounter>16){
      stringAux += (" "+(String)inputString[i]);
      nonSpaceCounter = 0;
    } else {
      stringAux += (String)inputString[i];
    }
  }
  //inputString = stringAux;
  return stringAux;
}

void UpdateCurrentLine(){
  buttonPressed = ReadButtons();
  if(buttonPressed==2 && currentLine<totalLines-1){  //increase page number
    currentLine+=2;
    DisplayMessage(message,currentLine);
    delay(500); //to avoid multiple presses
  } else if(buttonPressed==1 && currentLine>0){  //decrease page number
    currentLine-=2;
    DisplayMessage(message,currentLine);
    delay(500);
  }
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
  char c = Wire.read();
  messageAux+=c;
  
  if (messageAux.endsWith("#END#")){
    message = messageAux.substring(0,messageAux.indexOf("#END#"));
    Serial.println("");
    Serial.println(millis()/1000);
    Serial.println(message);
    messageAux="";
    buttonPressed = 0;
    currentLine = 0;
    message = formatMessage(message);
    totalLines = message.length()/16;
    DisplayMessage(message,currentLine);
    Beep();
  }
}

void setup() {
  Wire.begin(9); // 9 is the address of this Arduino (slave)
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(13, OUTPUT); //turn off internal led
  pinMode(8,OUTPUT); //passive speaker
  lcd.begin(16, 2);
  //Serial.println("Break test: "+BreakLargeWords("this is me tryingToBreakThisDamnThingIWonderHowMany words are needed"));
  
  // Print the stanby message to the LCD.
  message="Waiting for     message...";
  //message ="Things may not have gone as planned last week for the flying cellphone on Mars, but just because Ingenuitys flying career is over doesnt mean theres no more work to do.";
  //message ="Joao:           hello :) Things may not have gone as planned last week for the flying cellphone on Mars";
  //message = formatMessage(message); 
  totalLines = message.length()/16;
  DisplayMessage(message,0);
}

void loop() {
  UpdateCurrentLine(); 
}