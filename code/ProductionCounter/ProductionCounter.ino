/* algemeen ------------------ */

#define DELAY 500
#define ENTER '#'
#define X 15

int done;
int number_of_people;
int count = 0;
int product_code;
float temperature;
float workspeed_seconds;
float workspeed_per_person_seconds;
float workspeed_minutes;
float workspeed_per_person_minutes;
unsigned long start_time;
unsigned long stop_time;
int minutes;
int seconds;

/* keypad -------------------- */
#include <Keypad.h> //auteurs: Mark Stanley, Alexander Brevig Versie: 3.1.1

char key;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char readKey() {
  char key = keypad.getKey();

  return key;
}


/* ultrasonic ---------------- */

#define ECHO_PIN 10
#define TRIG_PIN 11

long duration;
int distance;

float readDistance() { // bron:https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6

  delay(DELAY);

  float Vair;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH); 

  distance = (duration * 0.034) / 2;

  return distance;
}


/* lcd ----------------------- */

#include <LiquidCrystal_I2C.h> // Auteur: Frank de Brabander Versie: 1.1.2

LiquidCrystal_I2C lcd(0x27,16,2);

/*Aansluitingen:
 * LCD SDA -> A4
 * LCD SCL -> A5
 * LCD GND -> GND
 * LCD VCC ->  5V
 */

/* functies ------------------------- */

char numberOfPeople() {
  int number;
  char enter;
  char key;
  String tussenresultaat = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nbr of people:");
  lcd.setCursor(0, 1);
  lcd.print("#=ENTER *=RESET");
  while((key = readKey()) == NO_KEY) {
    // wait for key
  }
  lcd.setCursor(14, 0);
  lcd.print(key);
  Serial.println(key);
  while((enter = readKey()) == NO_KEY) {
    // wait for key
  }

  tussenresultaat += key;

  if (enter == '#') {
    done = 1;
  } else if (enter == '*'){
    done = 0;
  } else if (enter >= '0' && enter <= '9'){
    tussenresultaat += enter;
    lcd.print(enter);
    while((enter = readKey()) == NO_KEY) { 
      // wait for key
    }

    if (enter == '#') {
    done = 1;
    } else if (enter == '*'){
    done = 0;
    }
  }

  number = tussenresultaat.toInt();
  Serial.println("p" + String(number));
  return number;
}

int productcode() {
  int code;
  String tussenresultaat = "";
  char key;
  char enter;
  char reset;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Productcode:");
  lcd.setCursor(0, 1);
  lcd.print("#=ENTER *=RESET");
  lcd.setCursor(12, 0);

  while (tussenresultaat.length() < 4){
    while((key = readKey()) == NO_KEY) {
    // wait for key
    }
    tussenresultaat += key;
    lcd.print(key);
  }

  while((enter = readKey()) == NO_KEY) {
    // wait for key
  }

  if (enter == '#') {
      done = 1;
      code = tussenresultaat.toInt();
      Serial.println("r" + String(code));
      return code;
  } else if (enter == '*') {
      done = 0;
  }

}

int countProduction() {

  if (readDistance() < X) {
    count += 1;
    Serial.println("c" + String(count));
    while(readDistance() < X) {
      //wait for package to pass
    }
  }

}

/* ---------------------------------------------- */

void setup(){
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
  
    Serial.begin(9600);

    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.print("Dag Wereld!");
}

void loop() {

    done = 0;

    Serial.println("C");

    while(done == 0) {
      number_of_people = numberOfPeople();
      delay(1000);
    }

    done = 0;

    while(done == 0) {
      product_code = productcode();
      delay(1000);
    }

    while(key != 'A') {

      lcd.clear();
      lcd.print("Press 'A' to");
      lcd.setCursor(0, 1);
      lcd.print("start counting.");
      while((key = readKey()) == NO_KEY) {
      // wait for key
      }
    }

    start_time = millis();
    lcd.clear();
    lcd.print("Counting...");
    lcd.setCursor(0, 1);
    lcd.print("'B'=FINISH");
    Serial.println("A");
    Serial.println("c" + String(count));

    done = 0;

    while(done == 0){
      countProduction();
      if (readKey() == 'B') {
        done = 1; // nog timer toevoegen!
        Serial.println("B");
        stop_time = millis();

        minutes = (stop_time - start_time) / 60000; // beter gericht op een grote productie
        seconds = (stop_time - start_time) / 1000; // accurater en geeft een beter beeld bij deze kleine versie

        delay(DELAY);

        Serial.println("m" + String(minutes));
        delay(DELAY);
        Serial.println("s" + String(seconds));
        delay(DELAY);

        workspeed_seconds = float(count) / seconds;
        workspeed_per_person_seconds = workspeed_seconds / number_of_people;
        workspeed_minutes = float(count) / minutes;
        workspeed_per_person_minutes = workspeed_minutes / number_of_people;

        Serial.println("z" + String(workspeed_seconds));
        delay(DELAY);
        Serial.println("y" + String(workspeed_per_person_seconds));
        delay(DELAY);
        Serial.println("x" + String(workspeed_minutes));
        delay(DELAY);
        Serial.println("w" + String(workspeed_per_person_minutes));
        delay(DELAY);
      }
    }

    

    while(key != 'C') {

      lcd.clear();
      lcd.print("Press 'C' to");
      lcd.setCursor(0, 1);
      lcd.print("start new count");
      while((key = readKey()) == NO_KEY) {
      // wait for key
      }
      count = 0;
      number_of_people = 0;
      product_code = 0;
    }

}
