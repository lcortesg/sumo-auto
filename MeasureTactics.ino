/**
 * @function measureLine : This function reads the line sensors and returns whether or not the robot is over the white line. 
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
    enemy_sensor_left = !digitalRead(PIN_SENSOR_LEFT);
    enemy_sensor_f_left = !digitalRead(PIN_SENSOR_F_LEFT);
    enemy_sensor_front = !digitalRead(PIN_SENSOR_FRONT);
    enemy_sensor_f_right = !digitalRead(PIN_SENSOR_F_RIGHT);
    enemy_sensor_right = !digitalRead(PIN_SENSOR_RIGHT);

    #if DEBUG > NONE
        Serial.print("\n>>> LOG: enemy measurements: \t");
        Serial.print(enemy_sensor_left);
        Serial.print("\t");
        Serial.print(enemy_sensor_f_left);
        Serial.print("\t");
        Serial.print(enemy_sensor_front);
        Serial.print("\t");
        Serial.print(enemy_sensor_f_right);
        Serial.print("\t");
        Serial.print(enemy_sensor_right);
    #endif
        
    return (enemy_sensor_left) || (enemy_sensor_f_left) || (enemy_sensor_front) || (enemy_sensor_f_right) || (enemy_sensor_right);
}
