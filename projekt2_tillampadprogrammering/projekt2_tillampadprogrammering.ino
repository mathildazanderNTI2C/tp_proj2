
#include "DHT.h"  // inkluderar biblioteken DHT och Servo
#include <Servo.h>

#define DHTPIN 2 
#define DHTTYPE DHT11   // DHT 11

const int dhtPin = 2;
const int pumpPin = 4;
const int servoPin = 9;
const int hydroPin = A0;  
//Här definieras olika konstanter och pinnar för användning i koden. DHT-sensorn är kopplad till pinne 2, pumpen till pinne 4, servomotorn till pinne 9, och en hydrometer (för jordfuktighet) till pinne A0.

bool oppen;

Servo myservo;
DHT dht(dhtPin, DHTTYPE);

int pos = 0;
float jordfukt = 0;
float temp = 0;
float luftfukt = 0;

//Här deklareras olika variabler, inklusive en boolean (oppen) för att hålla reda på om något är öppet eller stängt, en servoobjekt (myservo) och DHT-sensorobjekt (dht), samt variabler för position, jordfuktighet, temperatur och luftfuktighet.


void setup() {

  Serial.begin(9600);
  dht.begin();
  myservo.attach(servoPin);
  pinMode(pumpPin, OUTPUT);

// I setup-funktionen initieras seriell kommunikation, DHT-sensorn, servomotorn och pinnar för pumpen.

}

void loop() {

  updateraJordfukt();
  uppdateraDHT();
  
  if (temp > 25 && !oppen){
    //kollar om temperaturen är under 25 grader samt att servorn inte är öppen. Stämmer inte detta ska den stängas.
    oppna();
  }
  else{
    stang();
  }
  

  if (luftfukt > 30 && !oppen){
  // om luftfukten är över 30% och servorn inte är öppen ska servorn öppnas. Annars stängas.
    oppna();
  } 
  else{
    stang();
  }

  if (jordfukt < 30) {
    pumpa(); // om jordfuktigheten är under 30% ska det pumpas vatten.
  }
  delay(2000); 
//loop-funktionen innehåller de huvudsakliga stegen som upprepas. Den läser och uppdaterar jordfuktighet och DHT-värden, och beroende på dessa värden, utförs olika åtgärder som att öppna och stänga en servomotor samt starta en pump.
}


void updateraJordfukt() { //Jordfuktighetscensor
  jordfukt = analogRead(hydroPin);   
  jordfukt = constrain(jordfukt, 400, 1023); 
  jordfukt = map(jordfukt, 400, 1023, 100, 0); 
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
//Det finns tre ytterligare funktioner: updateraJordfukt(), uppdateraDHT(), och pumpa(). Dessa används för att uppdatera jordfuktighetsvärden, DHT-värden och för att starta pumpen, beroende på olika villkor.
}
