/**
 * @function measureLine : Lee los sensores de línea y determina si se está detecctando un fondo blanco
 * @param : none
 * @return : retorna 0 si no se detecta una línea, 1 en caso contrario
 */
int measureLine(){
  line_sensor_f_left = analogRead(PIN_LINE_F_LEFT);
  line_sensor_f_right = analogRead(PIN_LINE_F_RIGHT);
  line_sensor_b_left = analogRead(PIN_LINE_B_LEFT);
  line_sensor_b_right = analogRead(PIN_LINE_B_RIGHT);
  
  #if DEBUG > NONE
    Serial.print("\n>>> LOG: mediciones de linea: \t");
    Serial.print(line_sensor_f_left);
    Serial.print("\t");
    Serial.print(line_sensor_f_right);
    Serial.print("\t");
    Serial.print(line_sensor_b_left);
    Serial.print("\t");
    Serial.print(line_sensor_b_right);
  #endif
  return (line_sensor_f_left < TH_LINE) || (line_sensor_f_right < TH_LINE) || (line_sensor_b_left < TH_LINE) || (line_sensor_b_right < TH_LINE);
}

/**
 * @function measureEnemy : Lee los sensores reflex difusos y determina si hay un enemigo cerca
 * @param : none
 * @return : retorna 0 si no hay enemigos cerca, 1 en caso contrario.
 */
bool measureEnemy(){
  enemy_sensor_left = !digitalRead(PIN_SENSOR_LEFT);
  enemy_sensor_f_left = !digitalRead(PIN_SENSOR_F_LEFT);
  enemy_sensor_front = !digitalRead(PIN_SENSOR_FRONT);
  enemy_sensor_f_right = !digitalRead(PIN_SENSOR_F_RIGHT);
  enemy_sensor_right = !digitalRead(PIN_SENSOR_RIGHT);

  #if DEBUG > NONE
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
  #endif
  return (enemy_sensor_left) || (enemy_sensor_f_left) || (enemy_sensor_front) || (enemy_sensor_f_right) || (enemy_sensor_right);
}
