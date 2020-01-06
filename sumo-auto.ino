/**
 * @brief: Firmware desarrollado para el Robot Sumo Don Fede que por ahora solo encuentra al rival usando un control PID.
 * @author : Diego Pandolfa.
 * @date: 27/11/2018
 */
#include <SoftwareSerial.h>
#include <Sabertooth.h>

#define NONE 0
#define INFO 1

#define DEBUG INFO

#define PIN_SENSOR_LEFT 8
#define PIN_SENSOR_F_LEFT 11
#define PIN_SENSOR_FRONT 10
#define PIN_SENSOR_F_RIGHT 9
#define PIN_SENSOR_RIGHT 12

#define PIN_LINE_F_LEFT A0
#define PIN_LINE_F_RIGHT A1
#define PIN_LINE_B_LEFT A2
#define PIN_LINE_B_RIGHT A3

#define PIN_START_BUTTON 4
#define PIN_DIP_SWITCH 2

#define TIME_SURVIVE_1 120

/**
 * @brief : threshold de color.
 * normalmente mide bajo 100 en blanco y sobre 700 en negro
 * definir umbral seguro con desperfectos en el escenario.
 * 
 * @default 0 nunca detecta linea blanca
 */
#define TH_LINE 0

/**
 * @brief : define la potencia máxima entregada a los motores
 */
#define POWER_MAX 250

/**
 * @brief : overflow definido para un contador de ciclos del loop para uso de testeo
 */
#define TEST_COUNT_MAX 500

/**
 * @brief : overflow definido para un contador de ciclos sin encontrar un enemigo
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
 * @brief : constantes del controlador PID
 */
#define KP 0.032
#define KI 0.0008
#define KD 0.016

/**
 * @brief: referencia a seguir con el control PID
 */
#define REF 3000

#define PIN_SERIAL_ST 6

SoftwareSerial SWSerial(NOT_A_PIN, PIN_SERIAL_ST);
Sabertooth ST(128, SWSerial); // Address 128, and use SWSerial as the serial port.

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

void setup(){
  SWSerial.begin(9600);
  ST.autobaud();
  Serial.begin(115200);
  Serial.println("Init Sumo Robot - CoR 2018");
  pinMode(PIN_SENSOR_LEFT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_F_LEFT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_FRONT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_F_RIGHT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_RIGHT, INPUT_PULLUP);

  pinMode(PIN_LINE_F_LEFT, INPUT);
  pinMode(PIN_LINE_F_RIGHT, INPUT);
  pinMode(PIN_LINE_B_LEFT, INPUT);
  pinMode(PIN_LINE_B_RIGHT, INPUT);

  Serial.println("Stay for Buton Start ...");

  pinMode(PIN_DIP_SWITCH, INPUT_PULLUP);
  tactic = digitalRead(PIN_DIP_SWITCH);
  pinMode(PIN_START_BUTTON, INPUT_PULLUP);
  while(digitalRead(PIN_START_BUTTON));

  Serial.println("Stay for 5 seconds ... ");
  delay(5100);
  Serial.println("Fight!!!");
}

void loop(){
  count++; // comentar linea para codigo final

  if(measureLine()){
    survive(); 
  }
  searchFWD();

  if(measureEnemy()){
    if(searchPID()){
      fight();
    }
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
