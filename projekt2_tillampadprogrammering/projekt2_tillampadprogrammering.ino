//Just nu en kod med temperaturmätaren, servon, luftmätaren, jordfuktighetsmätaren, dubbelkolla så vattenpump fungerar

#include "DHT.h"
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

const int dhtPin = 2;
const int pumpPin = 4;
const int servoPin = 9;
const int hydroPin = A0;  //Hygrometer sensor analog pin output at pin A0 of Arduino
bool oppen;

Servo myservo;
DHT dht(dhtPin, DHTTYPE);

int pos = 0;
float jordfukt = 0;
float temp = 0;
float luftfukt = 0;


void setup() {

  Serial.begin(9600);
  dht.begin();
  myservo.attach(servoPin);
  pinMode(pumpPin, OUTPUT);

}

void loop() {

  updateraJordfukt();
  uppdateraDHT();
  
  if (temp > 25 && !oppen){
    //temperatur i rummet
    oppna();
  }
  else{
    stang();
  }
  

  if (luftfukt > 30 && !oppen){
  // luftfuktighet 
    oppna();
  } 
  else{
    stang();
  }

  if (jordfukt < 30) {
    pumpa(); // sätta delay på 10 sek så vattnet hinner åka ner emellan
  }
  delay(2000);
}


void updateraJordfukt() { //Jordfuktighetscensor
  jordfukt = analogRead(hydroPin);   //Read analog value
  jordfukt = constrain(jordfukt, 400, 1023); //Keep the ranges!
  jordfukt = map(jordfukt, 400, 1023, 100, 0); //Map value : 400 will be 100 and 1023 will be 0
  Serial.print("Jordfuktighet: ");
  Serial.print(jordfukt);
  Serial.println("%");
}

void uppdateraDHT() {
  luftfukt = dht.readHumidity();
  temp = dht.readTemperature();

  Serial.print("Luftfuktighet: ");
  Serial.print(luftfukt);
  Serial.println("%");

  Serial.print("Temperatur: ");
  Serial.print(temp);
  Serial.println(" grader");

}

void oppna(){
  myservo.write(90);
  oppen = true;
}

void stang(){
  myservo.write(0);
  oppen = false;
}

void pumpa() {
  digitalWrite(pumpPin, HIGH); // The motor starts to run
  delay(1000);
  Serial.println("Jag vattnar!");
  digitalWrite(pumpPin, LOW);
}
