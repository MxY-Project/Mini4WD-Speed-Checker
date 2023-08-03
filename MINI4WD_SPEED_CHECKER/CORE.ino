ICACHE_RAM_ATTR void sensor_handler(){
  counter++;
}

double toKmH(int rpm){
  return (perimeter * rpm) * 60.0 / (1000.0 * 1000.0);
}

int ExtractDecimalPart(double Value) {
  return  10 * (Value - (int)(Value));
}

int getBatteryPercentage(){
  return ((batVolt - 3.0) / (4.2 - 3.0)) * 100;  // convert to percentage
}

void setBatteryVolt(){
  // R1 = 10K, R2 = 40K
  // Vbat max = 4.2volt
  // Vbat min = 3.0 volt
  // Vout = 10 * Vbat / (10 + 40)
  // Vout max = 0.84 ~> 860 adc val (when 1v ~> 1023 adc val)
  // Vout min = 0.60 ~> 614 adc val

  if (millis() - previousAdcReadMillis >= adcReadPeriod && adcCounter < 10) {
    adcVal = adcVal + min(analogRead(A0), 860); //Read analog Voltage
    previousAdcReadMillis += adcReadPeriod;
    adcCounter = adcCounter + 1;
  }
  if(adcCounter == 10){
    batVolt = (float) adcVal / 10.0;           //Find average of 10 values
    batVolt = (float) (batVolt / 860.0) * 4.2; //Convert to voltage
    batVolt = max(3.0, min(4.2, batVolt));    //Make sure value between 0.6v - 0.84v
    
    adcCounter = 0;
    adcVal = 0;
  }
}