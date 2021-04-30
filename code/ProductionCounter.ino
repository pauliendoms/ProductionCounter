#define DELAY 1000
#define ENTER '#'

/* keypad -------------------- */
#include <Keypad.h>

char key;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char readKey() {
  char key = keypad.getKey();
  /*
  if (key){
    return key;
  } --> deze check wordt gedaan in de loop */

  return key;
}


/* ultrasonic ---------------- */

#define ECHO_PIN 10
#define TRIG_PIN 11

long duration;
int distance;

float readDistance() {
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  Serial.println("Debug: distance = " + String(distance));

  return distance;
}


/* lcd ----------------------- */

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

/*Aansluitingen:
 * LCD SDA -> A4
 * LCD SCL -> A5
 * LCD GND -> GND
 * LCD VCC ->  5V
 */

/* temperature sensor ----------------- */
#define TEMP_PIN A3

float temp;
float waarde;
float spanning;

float readTemperature() {
  waarde = analogRead(TEMP_PIN);
  spanning = (waarde / 1024) * 5;
  temp = spanning * 10;
  //delay(1000); -> was nodig om een deftige waarde te krijgen als deze code in de loop stond
  return temp;
}

/* functies ------------------------- */

char numberOfPeople() {
  int number;
  char enter;
  char key;

  lcd.setCursor(0, 0);
  lcd.print("Number of people");
  lcd.setCursor(0, 1);
  lcd.print("# = ENTER :");
  while((key = readKey()) == NO_KEY) {
    // wait for key
  }
  lcd.print(key);
  while((enter = readKey()) == NO_KEY) {
    if (enter == '#') {
      lcd.clear();
    }
  }
  
}



void setup(){
    pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(ECHO_PIN, INPUT);
    //pinMode(TEMP_PIN, INPUT); //-> ben niet zeker of dit erbij moet, eens testen of het werkt zonder
    Serial.begin(9600);

    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.print("Dag Wereld!");
}

void loop() {
    numberOfPeople();
}