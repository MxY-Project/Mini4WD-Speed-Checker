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
  for(int i = 0; i < sizeof(batteryVoltages) / sizeof(batteryVoltages[0]); i++){
    if(i > 0 ) {
      if( batVolt >= batteryVoltages[i] && batVolt <= batteryVoltages[i - 1] ) {
        return batteryPercentages[i - 1];
      }
    }
  }
  return 0;
}

void setBatteryVolt(){
  // R1 = 10K, R2 = 40K
  // Vbat max = 4.2volt
  // Vbat min = 3.0 volt
  // Vout = 10 * Vbat / (10 + 40)
  // Vout max = 0.84 ~> 860 adc val (when 1v ~> 1023 adc val)
  // Vout min = 0.60 ~> 614 adc val

  if (millis() - previousAdcReadMillis >= adcReadPeriod && adcCounter < 20) {
    adcVal = adcVal + min(analogRead(A0), 860); //Read analog Voltage
    previousAdcReadMillis += adcReadPeriod;
    adcCounter = adcCounter + 1;
  }
  if(adcCounter == 20){
    batVolt = (float) adcVal / 20.0;           //Find average of 20 values
    batVolt = (float) (batVolt / 860.0) * 4.2; //Convert to voltage
    batVolt = max(3.0, min(4.2, batVolt));     //Make sure value between 3.0v - 4.2v
    
    adcCounter = 0;
    adcVal = 0;
  }
}

long movingAverage(int rpm_val) {
  ////Perform average on rpm_list
  long average;
  // subtract the last reading:
  totalRpm = totalRpm - rpm_list[readIndex];
  // read the sensor:
  rpm_list[readIndex] = rpm_val;
  // add value to total:
  totalRpm = totalRpm + rpm_list[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = totalRpm / numReadings;

  return average;
}