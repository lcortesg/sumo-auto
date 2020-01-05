void survive(){
  switch(tactic){
    case 1: {
      survive1();
      break;  
    }
    default : {
      survive1();
      break;  
    }  
  }
  return;  
}

void survive1(){
  if(line_sensor_f_left < TH_LINE){
    ST.motor(1, -POWER_MAX);
    ST.motor(2, -POWER_MAX);
    delay(200);
    ST.motor(1, -POWER_MAX);
    ST.motor(2, POWER_MAX);
    delay(TIME_SURVIVE_1);
  }
  else if(line_sensor_f_right < TH_LINE){
    ST.motor(1, -POWER_MAX);
    ST.motor(2, -POWER_MAX);
    delay(200);
    ST.motor(1, POWER_MAX);
    ST.motor(2, -POWER_MAX); 
    delay(TIME_SURVIVE_1);
  }
  else if(line_sensor_b_left < TH_LINE){
    ST.motor(1, POWER_MAX);
    ST.motor(2, POWER_MAX); 
    delay(TIME_SURVIVE_1);
  }
  else if(line_sensor_b_right < TH_LINE){
    ST.motor(1, POWER_MAX);
    ST.motor(2, POWER_MAX); 
    delay(TIME_SURVIVE_1);
  }
  else{
    ;  
  }
  return;
}
