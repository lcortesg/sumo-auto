/**
 * @function
 */
bool searchPID(){
    measure = ((enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right) == 0)?(measure):(enemy_sensor_left*1000 + enemy_sensor_f_left*2000 + enemy_sensor_front*3000 + enemy_sensor_f_right*4000 + enemy_sensor_right*5000)/(enemy_sensor_left + enemy_sensor_f_left + enemy_sensor_front + enemy_sensor_f_right + enemy_sensor_right);

    error = ref - measure;
 
    error_int += (actuation >= POWER_MAX)?(0):(error);

    error_dev = error - error_prev;
    
    error_prev = error;
    
    actuation = (error*KP + error_int*KI + error_dev*KD);
    actuation = (actuation >= POWER_MAX)?(POWER_MAX):(actuation <= -POWER_MAX)?(-POWER_MAX):(actuation);
    
    if(abs(actuation) > 50){
        ST.motor(1, -actuation);
        ST.motor(2, actuation);
    }

    #if DEBUG > NONE
        Serial.print("\n>>> LOG: measure:\t");
        Serial.println(measure);
        
        Serial.print("\n>>> LOG: error:\t");
        Serial.println(error);

        Serial.print("\n>>> LOG: error int:\t");
        Serial.println(error_int);

        Serial.print("\n>>> LOG: error_dev:\t");
        Serial.println(error_dev);

        Serial.print("\n>>> LOG: error_prev:\t");
        Serial.println(error_prev);

        Serial.print("\n>>> LOG: actuation:\t");
        Serial.println(actuation);
    #endif

    return ( (enemy_sensor_front == 1) && (abs(actuation) < 50) );
}

void fight(){
    switch(tactic){
        case 1 : {
            fight1();
        break;
        }  
        default : {
            fight1();
        break;  
        }
    }
    return;  
}

void fight1(){
    unsigned long next = millis() + 10000;
    actuation = 0;
    #if DEBUG > 0
        Serial.print("\n>>> LOG : millis : ");
        Serial.println(millis());
        Serial.print("\n>>> LOG : next : ");
        Serial.println(next);
    #endif
    
    if(actuation < POWER_MAX){
        while(millis() < next){
            actuation = (actuation >= POWER_MAX)?(POWER_MAX):(actuation+10);
            ST.motor(1, actuation);
            ST.motor(2, actuation);
            delay(20);
            if(measureLine()){
                survive();
                break; 
            }
        }
    }
    else{
        ST.motor(1, POWER_MAX);
        ST.motor(2, POWER_MAX);
    }
    return;
}

void fight2(){
    ST.motor(1, POWER_MAX/1);
    ST.motor(2, POWER_MAX/1);
    return;
}

void searchFWD(){
    ST.motor(1, POWER_MAX*0.75);
    ST.motor(2, POWER_MAX*0.75);
    delay(20);
}
