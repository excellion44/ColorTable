    void rainbow_fade() 
    {         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
              ihue++;
              if (ihue > 255) 
              {
                ihue = 0;
              }
              for (int idex = 0 ; idex < NUM_LEDS_2; idex++ ) 
              {
                leds1[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
                leds2[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
              }
              FastLED.show();
              delay(20);
    }

    //Крутящая радуга #3
    void rainbow_loop(int ThisStep) 
    {                        //-m3-LOOP HSV RAINBOW
      idex++;
      ihue = ihue + ThisStep;
      if (idex >= NUM_LEDS_2) 
      {
        idex = 0;
      }
      if (ihue > 255) 
      {
        ihue = 0;
      }
      leds1[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
      leds2[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
      
      FastLED.show();
      delay(20);
    }
    
    //Случайная смена цветов №4
    void random_burst() 
    {                         
        idex = random(0, NUM_LEDS_2);
        ihue = random(0, 255);
        leds1[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
        leds2[idex] = CHSV(ihue, 255, BRIGHTNESS_VERTICAL_LEGS);
        FastLED.show();
        delay(20);
    }


    //Бегающий светодиод №5
    void color_bounce(int color) 
    {                       
        if (bouncedirection == 0) 
        {
          idex = idex + 1;
          if (idex == NUM_LEDS_2) 
          {
              bouncedirection = 1;
              idex = idex - 1;
          }
        }
        if (bouncedirection == 1) 
        {
          idex = idex - 1;
          if (idex == 0) 
          {
            bouncedirection = 0;
          }
        }
        for (int i = 0; i < NUM_LEDS_2; i++ ) 
        {
          if (i == idex) 
          {
            leds1[i] = CHSV(color, 255, BRIGHTNESS_VERTICAL_LEGS);
            leds2[i] = CHSV(color, 255, BRIGHTNESS_VERTICAL_LEGS);
          }
          else 
          {
            leds1[i] = CHSV(0, 0, 0);
            leds2[i] = CHSV(0, 0, 0);
          }
        }
        FastLED.show();
        delay(20);
}

//-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
void color_bounceFADE(int color) 
{                    
  if (bouncedirection == 0) 
  {
    idex = idex + 1;
    if (idex == NUM_LEDS_2) 
    {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1) 
  {
    idex = idex - 1;
    if (idex == 0) 
    {
      bouncedirection = 0;
    }
  }
  
  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);
  
  for (int i = 0; i < NUM_LEDS_2; i++ ) 
  {
    if (i == idex) 
    {
      leds1[i] = CHSV(color, 255, 255);
      leds2[i] = CHSV(color, 255, 255);
    }
    else if (i == iL1) 
    {
      leds1[i] = CHSV(color, 255, 150);
      leds2[i] = CHSV(color, 255, 150);
    }
    else if (i == iL2) 
    {
      leds1[i] = CHSV(color, 255, 80);
      leds2[i] = CHSV(color, 255, 80);
    }
    else if (i == iL3) 
    {
      leds1[i] = CHSV(color, 255, 20);
      leds2[i] = CHSV(color, 255, 20);
    }
    else if (i == iR1) 
    {
      leds1[i] = CHSV(color, 255, 150);
      leds2[i] = CHSV(color, 255, 150);
    }
    else if (i == iR2) 
    {
      leds1[i] = CHSV(color, 255, 80);
      leds2[i] = CHSV(color, 255, 80);
    }
    else if (i == iR3) 
    {
      leds1[i] = CHSV(color, 255, 20);
      leds2[i] = CHSV(color, 255, 20);
    }
    else 
    {
      leds1[i] = CHSV(0, 0, 0);
      leds2[i] = CHSV(0, 0, 0);
    }
  }
  FastLED.show();
  delay(40);
}

//Пульсация №7
void pulse_one_color_all(int color) 
{              
  if (bouncedirection == 0) 
  {
    ibright++;
    if (ibright >= BRIGHTNESS_VERTICAL_LEGS) 
    {
      bouncedirection = 1;
    }
  }
  if (bouncedirection == 1) 
  {
    ibright = ibright - 1;
    if (ibright <= 50) 
    {
      bouncedirection = 0;
    }
  }
  for (int idex = 0 ; idex < NUM_LEDS_2; idex++ ) 
  {
    leds1[idex] = CHSV(color, 255, ibright);
    leds2[idex] = CHSV(color, 255, ibright);
  }
  FastLED.show();
  delay(1);
}
