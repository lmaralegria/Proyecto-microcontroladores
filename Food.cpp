#include "Food.h"

// Definir el sprite para la comida
const uint8_t foodSprite[] PROGMEM = {
  0x3C, 0x00, 0x42, 0x00, 0xA5, 0x00, 0x81, 0x00, 0x99, 0x00, 
  0x81, 0x00, 0xA5, 0x00, 0x42, 0x00, 0x3C, 0x00, 0x00, 0x00
};


Food::Food(Adafruit_ILI9341 &display) : tft(display) {
  generate();
  sprite = foodSprite;
}

void Food::generate() {
  position.x = random(0, tft.width() / 10);
  position.y = random(0, tft.height() / 10);
}

void Food::draw() {
  tft.drawBitmap(position.x * 10, position.y * 10, sprite, SPRITE_WIDTH, SPRITE_HEIGHT, ILI9341_RED);
}

Position Food::getPosition() {
  return position;
}
