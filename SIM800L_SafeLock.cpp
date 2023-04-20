#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial sim800(3, 2); // gsm module connected here.
int relay = 4;
long unlockDuration = 30000;//30 seconds
const String PHONE =""; // Insert your phonr number here,country code first

void setup() {
  //Begin serial communication with Arduino and Arduino IDE
  Serial.begin(9600);
  //Begin serial communication with Arduino and SIM800L
  sim800.begin(9600); 
  //Relay mode
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);

  Serial.println("Initializing...");
  delay(1000);

  sim800.println("AT+CLTS=1"); //Update simcard time
  delay(1000);
  updateSerial();
  sim800.println("AT&W"); // Save simcard time
  delay(1000);
  updateSerial();
  sim800.println("AT+CCLK?"); //Check simcard time
  delay(1000);
  updateSerial();
  sim800.println("AT+CMGF=1"); //Set sms format as text
  delay(1000);
  updateSerial();
  sim800.println("AT+CNMI=1,2,0,0,0");  // Decides how newly arrived SMS messages should be handled
  delay(1000);
  updateSerial();
  sim800.println("AT+CMGD=1,4"); //Delete sms
  delay(1000);
  updateSerial();

}

void loop() 
{
  if(sim800.available()>0){
  inchar = sim800.read(); 
  Serial.println(inchar);
  delay(20);
  
//    for relay contolling the safe lock
    if ( (inchar == 'b'))
    {
      //Unlock the safe
      digitalWrite(relay, HIGH);
      Serial.println("The safe is unlocking");
      sim800.print("AT+CMGF=1\r");
      delay(500);
      sim800.print("AT+CMGS=\""+PHONE+"\"\r");
      sim800.print("Retrieve your items. Thank you.");
      delay(500);
      sim800.write(0x1A); //ASCII code for ctrl 26 -can also be sim800.printIn(char(26))
      delay(500);
      Serial.println("SMS sent successfully.");

      delay(unlockDuration);
      //Lock the safe
      digitalWrite(relay, LOW);
      Serial.println("The safe is locked now.");
    }
}
}

void updateSerial(){
  delay(500);
  while (Serial.available()) 
  {
    sim800.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800.available()) 
  {
    Serial.write(sim800.read());//Forward what Software Serial received to Serial Port
  }
}

