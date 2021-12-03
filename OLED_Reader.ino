#include "GyverOLED.h"                            // Подключение библиотек
#include "GyverButton.h"

GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;   // Объявлеем дисплей
GButton button(3); // Объявляем кнопку

//                                                <-- ТЕКСТ -->
const char text[] PROGMEM = {"111111111111111111111111111111111111111111111111111111111111111111111111111111111111222222222222222222222222222222222222222222222222222222222222222222222222222222222222333333333333333333333333333333333333333333333333333333333333333333333333333333333333444444444444444444444444444444444444444444444444444444444444444444444444444444444444555555555555555555555555555555555555555555555555555555555555555555555555555555555555"};

int startPos = 0;                                 // Позиция, с которой выводим текст
int endPos = 0;

int positions[500];
int currentPos = 0;

void setup() {
  button.setClickTimeout(200);                    // Устанавливаем таймаут кнопки

  oled.init();                                    // Инициализация дисплей
  oled.clear();                                   // Очистить дисплей
  oled.autoPrintln(true);                         // Автоматически переносить текст

  oled.home();                                    // Курсор в 0,0
  oled.setScale(2);                               // Масштаб 2
  oled.print("AcruxTech OLED v1.0");              // Выводим логотип
  initText();
  delay(2000);                                    // Ждём 2 сек

  oled.clear();                                   // Очищаем дисплей
  oled.home();                                    // Идём в 0,0
  oled.setScale(1);                               // мастшаб 1
}
void loop() {
  static bool flag = true;
  button.tick();                                  // Опрос кнопки
  if (button.isSingle()) {                        // Если кнопка нажата 1 раз
    oled.clear();                                 // Очищаем дислей
    oled.home();                                  // Идём в 0,0
    if (flag) {
      oled.print(drawText(positions[currentPos]));
      flag = false;
      currentPos = 1;
    }
    else {
      oled.print(drawText(positions[currentPos]));
      if (currentPos < endPos - 1) currentPos++;
    }
  }
  if (button.isDouble()) {                        // Если кнопка нажата 2 раза
    oled.clear();                                 // Очищаем дислей
    oled.home();                                  // Идём в 0,0
    if (currentPos > 0) currentPos--;
    oled.print(drawText(positions[currentPos]));
  }
}

String drawText(int _startPos) {
  int _amountSymbolsDouble = 0;
  String _outputString = "";

  for (int i = _startPos; i < strlen_P(text); i++) {
    if (_amountSymbolsDouble > 168) break;
    if (pgm_read_byte(&text[i]) > 127) {
      _outputString += (char)pgm_read_byte(&text[i]);
      _amountSymbolsDouble++;
      continue;
    }
    else {
      _outputString += (char)pgm_read_byte(&text[i]);
      _amountSymbolsDouble += 2;
    }
  }
  return _outputString;
}

void initText() {
  int _amountSymbolsDouble = 0;
  bool _flag = true;

  positions[0] = 0;

  for (int pos = 1; pos < 500; pos++) {
    _amountSymbolsDouble = 0;
    if (!_flag) {
      endPos = pos - 1;
      break;
    }
    for (int i = startPos; i < strlen_P(text); i++) {
      if (i == strlen_P(text) - 1) _flag = false;
      if (_amountSymbolsDouble > 168) {
        startPos += i - startPos - 1;
        positions[pos] = startPos;
        break;
      }
      if (pgm_read_byte(&text[i]) > 127) {
        _amountSymbolsDouble++;
        continue;
      }
      else {
        _amountSymbolsDouble += 2;
      }
    }
  }
}
