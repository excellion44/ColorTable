#include <FastLED.h>
#include <EEPROM.h>
//---------------------[Настройки ленты]----------------------------------------
// Настройки для вертикальной ножки
#define LED_PIN_1 6
#define NUM_LEDS_1 10  // Укажите количество светодиодов в Левой ножке стола
CRGB leds1[NUM_LEDS_1];

// Настройки для вертикальной ножки
#define LED_PIN_2 4
#define NUM_LEDS_2 10  // Укажите количество светодиодов в Правой ножке стола
CRGB leds2[NUM_LEDS_2];

// Настройки для правой горизонтальной ножки стола
#define LED_PIN_3 7
#define NUM_LEDS_3 30  // Укажите количество светодиодов 
CRGB leds3[NUM_LEDS_3];

// Настройки для левой горизонтальной ножки стола
#define LED_PIN_4 10
#define NUM_LEDS_4 30  // Укажите количество светодиодов 
CRGB leds4[NUM_LEDS_4];


//---------------------[Общие настройки]----------------------------------------
#define HUE_STEP 5      // Шаг изменения оттенка для мерцания
#define FADE_SPEED 3    // Скорость затухания/загорания (1-10)
#define DELAY_MS 30     // Задержка между обновлениями

uint8_t hue = 0;        // Оттенок для мерцающих лент
uint8_t fadeHue = 96;   // Фиксированный оттенок для третьей ленты (0-255, например 96 - сине-зелёный)
uint8_t fadeValue = 0;  // Текущая яркость третьей ленты
bool fadingUp = true;   // Направление изменения яркости
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;
int thisstep = 10;   
int bouncedirection = 0;
int Color = 83;
int ibright = 0;

int LED_MODE_VERTICAL = 0;

uint8_t BRIGHTNESS_VERTICAL_LEGS = 100;  //яркость вертикальных ножек стола
uint8_t BRIGHTNESS_HORIZONTAL_LEGS = 50;  //яркость вертикальных ножек стола

void setup() 
{

  Serial.begin(9600);              // открыть порт для связи
  // Инициализация лент
  FastLED.addLeds<WS2812, LED_PIN_1, GRB>(leds1, NUM_LEDS_1);
  FastLED.addLeds<WS2812, LED_PIN_2, GRB>(leds2, NUM_LEDS_2);
  FastLED.addLeds<WS2812, LED_PIN_3, GRB>(leds3, NUM_LEDS_3);
  FastLED.addLeds<WS2812, LED_PIN_4, GRB>(leds4, NUM_LEDS_4);
  
  //FastLED.setBrightness(BRIGHTNESS);
}

void loop() 
{


//---------------------------[читаем монитор  порта]---------------------------------
   if (Serial.available() > 0) 
   {
        String input = Serial.readStringUntil('\n'); // Читаем строку до новой строки
        input.trim(); // Удаляем лишние пробелы в начале и конце строки
        parseCommand(input);
        //LED_MODE_VERTICAL = input.toInt();
   }


//------------------------[Вертикальные ноги]------------------------------------  
  if(LED_MODE_VERTICAL == 0) //выключить
  {
      fill_solid(leds1, NUM_LEDS_2, CHSV(0, 0, 0));
      fill_solid(leds2, NUM_LEDS_2, CHSV(0, 0, 0));
  }
  if(LED_MODE_VERTICAL == 1) //просто один цвет
  {
      fill_solid(leds1, NUM_LEDS_2, CHSV(Color, 255, BRIGHTNESS_VERTICAL_LEGS));
      fill_solid(leds2, NUM_LEDS_2, CHSV(Color, 255, BRIGHTNESS_VERTICAL_LEGS));
  }
  if(LED_MODE_VERTICAL == 2)
  {
      rainbow_fade();  
  }
  if(LED_MODE_VERTICAL == 3)
  {
      rainbow_loop(thisstep);  //thisstep означает ШАГ 
  }
  if(LED_MODE_VERTICAL == 4)
  {
      random_burst();
  }
  if(LED_MODE_VERTICAL == 5)
  {
      color_bounce(Color); 
  }
  if(LED_MODE_VERTICAL == 6)
  {
      color_bounceFADE(Color); 
  }
  if(LED_MODE_VERTICAL == 7)
  {
      pulse_one_color_all(Color); 
  }
   
   
//------------------------[Вертикальные ноги КОНЕЦ]------------------------------------ 


  
  // 1. Обновляем мерцающие ленты (пины 4 и 6)
  //hue += HUE_STEP;
  //fill_solid(leds1, NUM_LEDS_1, CHSV(hue, 255, BRIGHTNESS_VERTICAL_LEGS));
  //fill_solid(leds2, NUM_LEDS_2, CHSV(hue, 255, BRIGHTNESS_VERTICAL_LEGS));
  
  // 2. Обновляем третью ленту с эффектом затухания (пин 10)
  if(fadingUp) 
  {
    fadeValue += FADE_SPEED;
    if(fadeValue >= 255) 
    {
      fadeValue = 255;
      fadingUp = false;
    }
  } 
  else 
  {
    fadeValue -= FADE_SPEED;
    if(fadeValue <= 0) 
    {
      fadeValue = 0;
      fadingUp = true;
    }
  }
  
  // Заполняем третью ленту фиксированным оттенком с меняющейся яркостью
  fill_solid(leds3, NUM_LEDS_3, CHSV(fadeHue, 255, fadeValue));
  
  // Отправка данных на все ленты
  FastLED.show();
  
  // Задержка для плавности
  delay(DELAY_MS);
}
