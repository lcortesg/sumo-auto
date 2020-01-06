/**
 * @brief: Firmware desarrollado para el Robot Sumo Don Fede que por ahora solo encuentra al rival usando un control PID.
 * @author : Diego Pandolfa.
 * @date: 16/11/2018
 */
#include <SoftwareSerial.h>
#include <Sabertooth.h>

#define PIN_SENSOR_LEFT 9
#define PIN_SENSOR_F_LEFT 8
#define PIN_SENSOR_FRONT 10
#define PIN_SENSOR_F_RIGHT 11
#define PIN_SENSOR_RIGHT 12

#define PIN_LINE_F_LEFT A1
#define PIN_LINE_F_RIGHT A3
#define PIN_LINE_B_LEFT A0
#define PIN_LINE_B_RIGHT A2

#define PIN_START_BUTTON 4

#define TH_LINE 500

#define POWER_MAX 250
#define TEST_COUNT_MAX 1000

//#define KP 0.018
//#define KI 0.0
//#define KD 0.008

#define KP 0.008
#define KI 0.0008
#define KD 0.008

#define REF 3000


SoftwareSerial SWSerial(NOT_A_PIN, 6);
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
  
  pinMode(PIN_START_BUTTON, INPUT_PULLUP);
  while(digitalRead(PIN_START_BUTTON));

  Serial.println("Stay for 5 seconds ... ");
  delay(5100);
  Serial.println("Fight!!!");
}

void loop(){
  count++; // comentar linea para codigo final
  
  line_sensor_f_left = analogRead(PIN_LINE_F_LEFT);
  line_sensor_f_right = analogRead(PIN_LINE_F_RIGHT);
  line_sensor_b_left = analogRead(PIN_LINE_B_LEFT);
  line_sensor_b_right = analogRead(PIN_LINE_B_RIGHT);

  Serial.print("\n>>> LOG: mediciones de linea: \t");
  Serial.print(line_sensor_f_left);
  Serial.print("\t");
  Serial.print(line_sensor_f_right);
  Serial.print("\t");
  Serial.print(line_sensor_b_left);
  Serial.print("\t");
  Serial.print(line_sensor_b_right);

  if( (line_sensor_f_left > TH_LINE) || (line_sensor_f_right > TH_LINE) ){
    //implementar lógica de permanencia en el escenario aqui...   <----------------------------------------------------------------------------------------------
    if((line_sensor_f_left > TH_LINE)){
      ST.motor(1, -POWER_MAX/2);
      ST.motor(2, -POWER_MAX);
    }
    else{
      ST.motor(1, -POWER_MAX);
      ST.motor(2, -POWER_MAX/2);
    }
    delay(300);
  }
  else if( (line_sensor_b_left > TH_LINE) || (line_sensor_b_right > TH_LINE) ){
    //implementar lógica de permanencia en el escenario aqui...   <----------------------------------------------------------------------------------------------
    if((line_sensor_f_left > TH_LINE)){
      ST.motor(1, POWER_MAX);
      ST.motor(2, POWER_MAX/2);
    }
    else{
      ST.motor(1, POWER_MAX/2);
      ST.motor(2, POWER_MAX);
    }
    delay(300);
  }
  else{
    enemy_sensor_left = !digitalRead(PIN_SENSOR_LEFT);
    enemy_sensor_f_left = !digitalRead(PIN_SENSOR_F_LEFT);
    enemy_sensor_front = !digitalRead(PIN_SENSOR_FRONT);
    enemy_sensor_f_right = !digitalRead(PIN_SENSOR_F_RIGHT);
    enemy_sensor_right = !digitalRead(PIN_SENSOR_RIGHT);

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

    measure = ((enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right) == 0)?(measure):(enemy_sensor_left*1000 + enemy_sensor_f_left*2000 + enemy_sensor_front*3000 + enemy_sensor_f_right*4000 + enemy_sensor_right*5000)/(enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right);

    Serial.print("\n>>> LOG: measure:\t");
    Serial.println(measure);
    
    error = ref - measure;
    Serial.print("\n>>> LOG: error:\t");
    Serial.println(error);
    
    error_int += (actuation >= POWER_MAX)?(0):(error);
    Serial.print("\n>>> LOG: error int:\t");
    Serial.println(error_int);
    
    error_dev = error - error_prev;
    Serial.print("\n>>> LOG: error_dev:\t");
    Serial.println(error_dev);
    
    error_prev = error;
    Serial.print("\n>>> LOG: error_prev:\t");
    Serial.println(error_prev);

    if(error == 0){
      
      // implementar lógica de combate aqui...  <-------------------------------------------------------------------------------------------------------------------
      if(enemy_sensor_f_left || enemy_sensor_f_right){
        ST.motor(1, POWER_MAX);
        ST.motor(2, POWER_MAX);  
      }
      else{
        ST.motor(1, POWER_MAX/4);
        ST.motor(2, POWER_MAX/4);
      }
    }
    else{

      // implementar lógica de búsqueda aquí...  <--------------------------------------------------------------------------------------------------------------------
      
      actuation = (error*KP + error_int*KI + error_dev*KD);
      actuation = (actuation >= POWER_MAX)?(POWER_MAX):
                  (actuation <= -POWER_MAX)?(-POWER_MAX):(actuation);
      Serial.print("\n>>> LOG: actuation:\t");
      Serial.println(actuation);
    
      ST.motor(1, actuation);
      ST.motor(2, -actuation);
    }
  }
  delay(10);

  /**
   * comentar para codigo final
   */
  Serial.print("\nCount Test:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\t");
  Serial.println(count);
  
  if(count >= TEST_COUNT_MAX){
    ST.motor(1, 0);
    ST.motor(2, 0);
    while(1);
  }
}

