/**
 * @brief: Firmware developed for the Robot Sumo Auto named "Don Fede". It finds it's enemy using a PID controller.
 * @date: 20/04/10
 */

#include <SoftwareSerial.h>
#include <Sabertooth.h>


#define ENABLE 0        // Change this to 1 when testing for real.
#define DEBUG !ENABLE   // When enabled, this will print via serial prompt all the measurements and messages written into this file, as well as the "Tactic's" files.
#define NONE 0

//#define PIN_SENSOR_LEFT 8
//#define PIN_SENSOR_F_LEFT 11
//#define PIN_SENSOR_FRONT 10
//#define PIN_SENSOR_F_RIGHT 9
//#define PIN_SENSOR_RIGHT 12

#define PIN_LINE_F_LEFT A0
#define PIN_LINE_F_RIGHT A2
#define PIN_LINE_B_LEFT A1
#define PIN_LINE_B_RIGHT A3

//#define PIN_START_BUTTON 3
//#define PIN_DIP_SWITCH 2

#define TIME_SURVIVE_1 120

/**
 * @brief : Color threshold.
 * Under normal conditions white is measured under 100.
 * under normal conditions black is measured over 700.
 * A safe threshold should be defined under test measurements done in the Dohyo.
 *
 * @default 0 : It will not detect the white line.
 */
#define TH_LINE 100

/**
 * @brief : defines the maximum power given to the motors.
 */
#define POWER_MAX 255

/**
 * @brief : overflow defined for a loop cycle counter for debugging purposes.
 */
#define TEST_COUNT_MAX 500

/**
 * @brief : overflow defined for an enemy-not-found counter
 */
#define MISS_COUNT_MAX 1000

/*
#define KP 0.018
#define KI 0.0
#define KD 0.008
*/
/*
#define KP 0.008
#define KI 0.0008
#define KD 0.008
*/

/**
 * @brief : PID controller constants.
 * We used the Ziegler–Nichols method to tune this parameters.
 */

#define KP 0.15 //0.064
#define KI 0.0008
#define KD 0.016

/**
 * @brief: Reference value to follow with the PID controller.
 */
#define REF 3000

#define PIN_SERIAL_ST 6

#define PIN_LED 13
//#define PIN_LED 7

SoftwareSerial SWSerial(NOT_A_PIN, PIN_SERIAL_ST);
Sabertooth ST(128, SWSerial); // Address 128, and use SWSerial as the serial port.

double enemy;

double enemy_sensor_left = 0;
double enemy_sensor_f_left = 0;
double enemy_sensor_front = 0;
double enemy_sensor_f_right = 0;
double enemy_sensor_right = 0;

int line_sensor_f_left = 0;
int line_sensor_f_right = 0;
int line_sensor_b_left = 0;
int line_sensor_b_right = 0;

double actuation = 0;
double error_prev = 0;
double error = 0;
double error_int = 0;
double error_dev = 0;
double measure = 0;
double ref = REF;

int count = 0;
int miss_count = 0;
int tactic = 0;
int cantidad = 0;
byte var = B01100;
int cont = 0;


void setup(){

    SWSerial.begin(9600);
    ST.autobaud();

    #if DEBUG > NONE
        Serial.begin(115200);
        Serial.println("Init Sumo Robot - CoR 2019");
    #endif

    DDRB   = B100000;
    DDRC  |= B000000;
    DDRD  |= B000000;
    PORTB &= B011111;


    
    tactic = PINC & 000100;

    #if DEBUG > NONE
        for (int j = 0; j < 5; j++){if((var>>j & 1) == 0) cont +=1;}
        Serial.println(cont);
        Serial.println("Wait for Button Start ...");
    #endif

    
    attachInterrupt(digitalPinToInterrupt(PINC & 001000), stop, FALLING);
    while(!(PINC & 001000));
    PORTB |= B100000;

    #if DEBUG > NONE
        Serial.println("Wait for 5 seconds ... ");
    #endif

    delay(5050);

    #if DEBUG > NONE
        Serial.println("Fight!!!");
    #endif
    ST.motor(1, 255);
    ST.motor(2, 255);
    delay(250);
    ST.motor(1, 0);
    ST.motor(2, 0);
}

void loop(){
    /*
    count++; // Comment out this line for the final code

    if(measureLine()){
        survive();
    }
    searchFWD();
    */
    
    if(measureEnemy()){
        if(searchPID()){
            fight();
        }
    }
    else if(measureLine()){
        survive();
    }

/*
  #if DEBUG > NONE
    Serial.print("\nCount Test:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\t");
    Serial.println(count);
  #endif
  if(count >= TEST_COUNT_MAX){
    ST.motor(1, 0);
    ST.motor(2, 0);
    while(1);
  }*/
}


void stop(){
    ST.motor(1, 0);
    ST.motor(2, 0);
    digitalWrite(PIN_LED, LOW);
    while(true){
        ST.motor(1, 0);
        ST.motor(2, 0);
    }
}
