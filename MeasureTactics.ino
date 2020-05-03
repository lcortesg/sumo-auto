/**
 * @function measureLine : This function reads the line sensors and determines if robot is over the white line. 
 * @param : none
 * @return : Returns 1 if the white line is detected, 0 otherwise.
 */
/*
bool measureLine(){
    line_sensor_f_left = analogRead(PIN_LINE_F_LEFT);
    line_sensor_f_right = analogRead(PIN_LINE_F_RIGHT);
    line_sensor_b_left = analogRead(PIN_LINE_B_LEFT);
    line_sensor_b_right = analogRead(PIN_LINE_B_RIGHT);
  
    #if DEBUG > NONE
        Serial.print("\n>>> LOG: line measurements: \t");
        Serial.print(line_sensor_f_left);
        Serial.print("\t");
        Serial.print(line_sensor_f_right);
        Serial.print("\t");
        Serial.print(line_sensor_b_left);
        Serial.print("\t");
        Serial.print(line_sensor_b_right);
    #endif

    return (line_sensor_f_left < TH_LINE) || (line_sensor_f_right < TH_LINE) || (line_sensor_b_left < TH_LINE) || (line_sensor_b_right < TH_LINE);
}*/


bool measureLine(){
    pinMode(PIN_LINE_F_LEFT, OUTPUT);
    digitalWrite(PIN_LINE_F_LEFT, HIGH);
    delay(2);
    digitalWrite(PIN_LINE_F_LEFT, LOW);
    pinMode(PIN_LINE_F_LEFT, INPUT);
    delay(2);
    line_sensor_f_left = analogRead(PIN_LINE_F_LEFT);
    
    pinMode(PIN_LINE_F_RIGHT, OUTPUT);
    digitalWrite(PIN_LINE_F_RIGHT, HIGH);
    delay(2);
    digitalWrite(PIN_LINE_F_RIGHT, LOW);
    pinMode(PIN_LINE_F_RIGHT, INPUT);
    delay(2);
    line_sensor_f_right = analogRead(PIN_LINE_F_RIGHT);
    
    //line_sensor_b_left = analogRead(PIN_LINE_B_LEFT);
    //line_sensor_b_right = analogRead(PIN_LINE_B_RIGHT);
  
    #if DEBUG > NONE
        Serial.print("\n>>> LOG: line sensor measurements: \t");
        Serial.print(line_sensor_f_left);
        Serial.print("\t");
        Serial.print(line_sensor_f_right);
        Serial.print("\t");
        Serial.print(line_sensor_b_left);
        Serial.print("\t");
        Serial.print(line_sensor_b_right);
    #endif

    return (line_sensor_f_left < TH_LINE) || (line_sensor_f_right < TH_LINE) /*|| (line_sensor_b_left < TH_LINE) || (line_sensor_b_right < TH_LINE)*/;
}
/**
 * @function measureEnemy : This function reads the diffuse reflex sensors and determines an enemy is detected.
 * @param : none
 * @return : Returns 0 if an enemy is detected, 1 otherwise.
 */


bool measureEnemy(){
    int out_enemy = 0; 
    enemy = (PINB & B00011111);
    if (enemy < B00011111) out_enemy = 1;
    return (out_enemy);
}
