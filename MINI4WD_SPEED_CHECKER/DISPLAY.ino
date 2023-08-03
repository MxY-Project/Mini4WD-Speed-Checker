void show_rpm(int rpm_val){  
    display.clearDisplay();

    set_header("Speed Checker");
    set_kmh(rpm_val);
    set_max_kmh(rpm_val);
    set_rpm(rpm_val);
    set_time();
    set_battery();

    display.display();
}

void set_time(){
  int sec = 0;
  if (previousTimerMillis > 0) {
    sec = (millis() - previousTimerMillis) / 1000;
  }
  sprintf (str_cnt, "%ds", sec);
  display.setCursor(2, 52);
  display.setTextSize(1);
  display.print(str_cnt);
}

void set_rpm(int rpm_val){
  // display RPM
  sprintf (str_cnt, "%5d", rpm_val);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(40, 44);
  display.print(str_cnt);

  display.setCursor(102, 52);
  display.setTextSize(1);
  display.print("rpm");
}

void set_kmh(int rpm_val){
  // display KmH
  double kmh = toKmH(rpm_val);

  sprintf (str_cnt, "%2d", (int)(kmh));
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(52, 18);
  display.print(str_cnt);

  sprintf(str_cnt, ".%d", ExtractDecimalPart(kmh));
  display.setCursor(86, 30);
  display.setTextSize(1);
  display.print(str_cnt);

  display.setCursor(102, 30);
  display.setTextSize(1);
  display.print("km/h");
}

void set_max_kmh(int rpm_val){

  maxKmh = max(maxKmh, toKmH(rpm_val));
  // display Max KmH
  sprintf (str_cnt, "%.1f", maxKmh);
  display.setTextSize(1);
  display.setCursor(2, 30);
  display.print(str_cnt);

  display.drawRect(0, 18, 18, 9, WHITE);
  display.fillTriangle(26, 18, 18, 26, 18, 18, WHITE);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(1, 18);
  display.setTextSize(1);
  display.print("max");
}

void set_header(char *label){
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextColor(WHITE);
  display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h); //calc width of new string
  display.setCursor((128 - w) / 2, 4);
  display.print(label);
  display.drawLine(0, 14, 127, 14, WHITE);
}

void set_battery(){
  batteryPercentage = (int)(getBatteryPercentage() / 10.0); // scale from 1 - 10
  batteryPercentage = min(10, max(0, batteryPercentage)); // safe bound value to 1 - 10
  display.drawRect(128 - 18, 4, 14, 8, WHITE);
  display.drawRect(128 -  4, 5,  2, 6, WHITE);
  display.fillRect(128 - 18 + 2, 4 + 2, batteryPercentage, 4, WHITE);
}