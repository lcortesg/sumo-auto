/**
 * @brief: Firmware desarrollado para el Robot Sumo Don Fede que por ahora solo encuentra al rival usando un control PID.
 * @author : Diego Pandolfa.
 * @date: 12/10/2018
 */
#include <SoftwareSerial.h>
#include <Sabertooth.h>

#define PIN_SENSOR_LEFT 10
#define PIN_SENSOR_F_LEFT 12
#define PIN_SENSOR_FRONT 8
#define PIN_SENSOR_F_RIGHT 11
#define PIN_SENSOR_RIGHT 9

#define PIN_LINE_F_LEFT A0
#define PIN_LINE_F_RIGHT A1
#define PIN_LINE_B_LEFT A2
#define PIN_LINE_B_RIGHT A3

#define TH_LINE 1000

#define POWER_MAX 255

#define KP 0.018
#define KI 0.0
#define KD 0.008

#define REF 3000


SoftwareSerial SWSerial(NOT_A_PIN, 1);
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

void setup(){
  SWSerial.begin(9600);
  ST.autobaud();
  //Serial.begin(115200);
  pinMode(PIN_SENSOR_LEFT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_F_LEFT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_FRONT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_F_RIGHT, INPUT_PULLUP);
  pinMode(PIN_SENSOR_RIGHT, INPUT_PULLUP);

  pinMode(PIN_LINE_F_LEFT, INPUT);
  pinMode(PIN_LINE_F_RIGHT, INPUT);
  pinMode(PIN_LINE_B_LEFT, INPUT);
  pinMode(PIN_LINE_B_RIGHT, INPUT);
}

void loop(){

  line_sensor_f_left = analogRead(PIN_LINE_F_LEFT);
  line_sensor_f_right = analogRead(PIN_LINE_F_RIGHT);
  line_sensor_b_left = analogRead(PIN_LINE_B_LEFT);
  line_sensor_b_right = analogRead(PIN_LINE_B_RIGHT);
/*
  Serial.print("\n>>> LOG: mediciones de linea: \t");
  Serial.print(line_sensor_f_left);
  Serial.print("\t");
  Serial.print(line_sensor_f_right);
  Serial.print("\t");
  Serial.print(line_sensor_b_left);
  Serial.print("\t");
  Serial.print(line_sensor_b_right);
*/
  if( (line_sensor_f_left > TH_LINE) || (line_sensor_f_right > TH_LINE) || (line_sensor_b_left > TH_LINE) || (line_sensor_b_right > TH_LINE) ){
    //implementar lógica de permanencia en el escenario aqui...   <----------------------------------------------------------------------------------------------
      ST.motor(1, -POWER_MAX/4);
      ST.motor(2, -POWER_MAX/4);
  }
  else{
    enemy_sensor_left = !digitalRead(PIN_SENSOR_LEFT);
    enemy_sensor_f_left = !digitalRead(PIN_SENSOR_F_LEFT);
    enemy_sensor_front = !digitalRead(PIN_SENSOR_FRONT);
    enemy_sensor_f_right = !digitalRead(PIN_SENSOR_F_RIGHT);
    enemy_sensor_right = !digitalRead(PIN_SENSOR_RIGHT);
/*
    Serial.print("\n>>> LOG: mediciones de enemigo: \t");
    Serial.print(enemy_sensor_left);
    Serial.print("\t");
    Serial.print(enemy_sensor_f_left);
    Serial.print("\t");
    Serial.print(enemy_sensor_front);
    Serial.print("\t");
    Serial.print(enemy_sensor_f_right);
    Serial.print("\t");
    Serial.print(enemy_sensor_right);
*/
    measure = ((enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right) == 0)?(2500):(enemy_sensor_left*1000 + enemy_sensor_f_left*2000 + enemy_sensor_front*3000 + enemy_sensor_f_right*4000 + enemy_sensor_right*5000)/(enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right);
/*
    Serial.print("\n>>> LOG: measure:\t");
    Serial.println(measure);
  */  
    error = measure - ref;
/*    Serial.print("\n>>> LOG: error:\t");
    Serial.println(error);
  */  
    error_int += (actuation >= POWER_MAX)?(0):(error);
 /*   Serial.print("\n>>> LOG: error int:\t");
    Serial.println(error_int);
   */ 
    error_dev = error - error_prev;
 /*   Serial.print("\n>>> LOG: error_dev:\t");
    Serial.println(error_dev);
   */ 
    error_prev = error;
   /* Serial.print("\n>>> LOG: error_prev:\t");
    Serial.println(error_prev);
*/
    //if(error == 0.0){
      
      // implementar lógica de combate aqui...  <-------------------------------------------------------------------------------------------------------------------
      //ST.motor(1, POWER_MAX/5);
      //ST.motor(2, POWER_MAX/5);
    //}
    //else{

      // implementar lógica de búsqueda aquí...  <--------------------------------------------------------------------------------------------------------------------
      actuation = (actuation >= POWER_MAX)?(POWER_MAX):(error*KP + error_int*KI + error_dev*KD);
/*
      Serial.print("\n>>> LOG: actuation:\t");
      Serial.println(actuation);
  */  
      ST.motor(1, actuation);
      ST.motor(2, -actuation);
      //ST.motor(1, -actuation);
      //ST.motor(2, actuation);
    //}
  }
  delay(10);
}

