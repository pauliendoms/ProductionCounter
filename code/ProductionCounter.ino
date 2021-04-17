/* keypad -------------------- */
#include <Keypad.h>

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


/* ultrasonic ---------------- */

#define EchoPin 10
#define TrigPin 11

int duration;


/* lcd ----------------------- */

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

/*Aansluitingen:
 * LCD SDA -> A4
 * LCD SCL -> A5
 * LCD GND -> GND
 * LCD VCC ->  5V
 */

/* --------------------------- */

void setup(){
    pinMode(TrigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(EchoPin, INPUT);
    Serial.begin(9600);

    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.print("Dag Wereld!");
}

void loop() {
    
    /* KEYPAD TEST */
    char key = keypad.getKey();
  
    if (key){
    Serial.println(key);
    }

    /* ULTROSONIC TEST */

    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    duration = pulseIn(EchoPin, HIGH);
    delay(1000);
    lcd.clear();
    lcd.print(duration);
}