//------------------------------------- UTILITY FXNS --------------------------------------
//---SET THE COLOR OF A SINGLE RGB LED
void set_color_led(int adex, int cred, int cgrn, int cblu) {
  leds1[adex].setRGB( cred, cgrn, cblu);
  leds2[adex].setRGB( cred, cgrn, cblu);
}



//---FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  int r;
  if (i < NUM_LEDS_2 - 1) {
    r = i + 1;
  }
  else {
    r = 0;
  }
  return r;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) {
    r = i - 1;
  }
  else {
    r = NUM_LEDS_2 - 1;
  }
  return r;
}


void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds1[Pixel].r = red;
  leds1[Pixel].g = green;
  leds1[Pixel].b = blue;
  leds2[Pixel].r = red;
  leds2[Pixel].g = green;
  leds2[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS_2; i++ ) {
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}

void parseCommand(String input) {
  int modeStart = input.indexOf("MODE:");
  int colorStart = input.indexOf("COLOR:");
  int brightStart = input.indexOf("BRIGHT:");
  
  if (modeStart != -1) {
    int modeEnd = input.indexOf(',', modeStart);
    if (modeEnd == -1) modeEnd = input.length();
    LED_MODE_VERTICAL = input.substring(modeStart + 5, modeEnd).toInt();
  }
  
  if (colorStart != -1) {
    int colorEnd = input.indexOf(',', colorStart);
    if (colorEnd == -1) colorEnd = input.length();
    Color = input.substring(colorStart + 6, colorEnd).toInt();
  }
  
  if (brightStart != -1) {
    int brightEnd = input.indexOf(',', brightStart);
    if (brightEnd == -1) brightEnd = input.length();
    BRIGHTNESS_VERTICAL_LEGS = input.substring(brightStart + 7, brightEnd).toInt();
  }
  
  Serial.print("Parsed values - Mode: ");
  Serial.print(LED_MODE_VERTICAL);
  Serial.print(", Color: ");
  Serial.print(Color);
  Serial.print(", Brightness: ");
  Serial.println(BRIGHTNESS_VERTICAL_LEGS);
}
