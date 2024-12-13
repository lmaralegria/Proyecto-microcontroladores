#include "Snake.h"

// Definir los sprites para el cuerpo de la serpiente y la comida
const uint8_t bodySprite[] PROGMEM = {
  0xff, 0xff, 0xc3, 0xc3, 0xa5, 0xa5, 0x81, 0x81, 0xa5, 0xa5, 
  0x81, 0x81, 0xa5, 0xa5, 0xc3, 0xc3, 0xff, 0xff, 0xff, 0xff
};


Snake::Snake(Adafruit_ILI9341 &display) : tft(display) {
  length = 3; // Longitud inicial
  int startX = display.width() / 20;
  int startY = display.height() / 20;
  body[0] = { startX, startY }; // Posición inicial
  for (int i = 1; i < length; i++) {
    body[i] = { startX - i, startY }; // Colocar segmentos iniciales sin colisión
  }
  direction = RIGHT;
  sprite = bodySprite;
}

void Snake::move() {
  for (int i = length - 1; i > 0; i--) {
    body[i] = body[i - 1];
  }
  switch (direction) {
    case UP: body[0].y -= 1; break;
    case DOWN: body[0].y += 1; break;
    case LEFT: body[0].x -= 1; break;
    case RIGHT: body[0].x += 1; break;
  }

  // Revisar bordes de la pantalla
  if (body[0].x < 0) body[0].x = tft.width() / 10 - 1;
  if (body[0].x >= tft.width() / 10) body[0].x = 0;
  if (body[0].y < 0) body[0].y = tft.height() / 10 - 1;
  if (body[0].y >= tft.height() / 10) body[0].y = 0;
}

void Snake::grow() {
  if (length < MAX_LENGTH) {
    body[length] = body[length - 1]; // Añade un nuevo segmento
    length++;
  }
}

void Snake::draw() {
  for (int i = 0; i < length; i++) {
    tft.drawBitmap(body[i].x * 10, body[i].y * 10, sprite, SPRITE_WIDTH, SPRITE_HEIGHT, ILI9341_GREEN);
  }
}

void Snake::setDirection(Direction dir) {
  if ((dir == UP && direction != DOWN) ||
      (dir == DOWN && direction != UP) ||
      (dir == LEFT && direction != RIGHT) ||
      (dir == RIGHT && direction != LEFT)) {
    direction = dir;
  }
}

bool Snake::hasCollided() {
  for (int i = 1; i < length; i++) {
    if (body[0].x == body[i].x && body[0].y == body[i].y) {
      return true;
    }
  }
  return false;
}

bool Snake::hasEatenFood(const Position &foodPos) {
  return body[0].x == foodPos.x && body[0].y == foodPos.y;
}

Position Snake::getHead() {
  return body[0];
}
