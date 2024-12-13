#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include "Snake.h"
#include "Food.h"
#include "Position.h"

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define BUTTON_UP    2
#define BUTTON_DOWN  3
#define BUTTON_LEFT  19
#define BUTTON_RIGHT  20
#define BUZZER_PIN 6

// Definir IRAM_ATTR si no está definido
#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Snake snake(display);
Food food(display);

Direction currentDirection = RIGHT;
bool gameOver = false;
int score = 0;

struct Button {
  const uint8_t PIN;
  volatile bool pressed;
};

Button buttonUp = {BUTTON_UP, false};
Button buttonDown = {BUTTON_DOWN, false};
Button buttonLeft = {BUTTON_LEFT, false};
Button buttonRight = {BUTTON_RIGHT, false};

void IRAM_ATTR isrUp() { buttonUp.pressed = true; }
void IRAM_ATTR isrDown() { buttonDown.pressed = true; }
void IRAM_ATTR isrLeft() { buttonLeft.pressed = true; }
void IRAM_ATTR isrRight() { buttonRight.pressed = true; }

// Función para reproducir música
void playMusic() {
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}

// Función para reproducir sonido al comer
void playEatSound() {
  tone(BUZZER_PIN, 523, 150); // Sonido breve cuando come
  delay(150);
  noTone(BUZZER_PIN);
}

void setup() {
  display.begin();
  display.setRotation(3);
  display.fillScreen(ILI9341_BLACK);

  pinMode(buttonUp.PIN, INPUT_PULLUP);
  pinMode(buttonDown.PIN, INPUT_PULLUP);
  pinMode(buttonLeft.PIN, INPUT_PULLUP);
  pinMode(buttonRight.PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonUp.PIN), isrUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonDown.PIN), isrDown, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonLeft.PIN), isrLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonRight.PIN), isrRight, FALLING);

  Serial.begin(9600); // Inicializamos la comunicación serie

  randomSeed(analogRead(0));
  playMusic(); // Reproduce música al inicio
}

void loop() {
  if (gameOver) {
    display.setCursor(50, 80);
    display.setTextColor(ILI9341_WHITE);
    display.setTextSize(2);
    display.println("Game Over!");
    display.setCursor(50, 120);
    display.print("Score: ");
    display.print(score);
    while(true); // Detiene el juego
  }

  // Leer el estado de los botones y cambiar la dirección de la serpiente
  if (buttonUp.pressed) {
    snake.setDirection(UP);
    Serial.println("Button UP pressed"); // Mensaje de depuración
    buttonUp.pressed = false;
  }
  if (buttonDown.pressed) {
    snake.setDirection(DOWN);
    Serial.println("Button DOWN pressed"); // Mensaje de depuración
    buttonDown.pressed = false;
  }
  if (buttonLeft.pressed) {
    snake.setDirection(LEFT);
    Serial.println("Button LEFT pressed"); // Mensaje de depuración
    buttonLeft.pressed = false;
  }
  if (buttonRight.pressed) {
    snake.setDirection(RIGHT);
    Serial.println("Button RIGHT pressed"); // Mensaje de depuración
    buttonRight.pressed = false;
  }

  // Limpiar la pantalla antes de redibujar
  display.fillScreen(ILI9341_BLACK);

  snake.move();
  
  // Verificación de colisiones
  if (snake.hasCollided()) {
    gameOver = true;
  }

  if (snake.hasEatenFood(food.getPosition())) {
    snake.grow();
    food.generate();
    score += 10; // Incrementar el puntaje
    playEatSound(); // Reproduce sonido al comer
  }

  // Dibuja la serpiente
  snake.draw();
  // Dibuja la comida
  food.draw();
  
  // Mostrar el puntaje
  display.setCursor(10, 10);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  display.print("Score: ");
  display.print(score);

  delay(50); // Controla la velocidad del juego
}
