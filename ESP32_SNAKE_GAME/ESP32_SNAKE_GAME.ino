#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Joystick Pins
const int VRX_PIN = 34;
const int VRY_PIN = 35;
const int SW_PIN = 15;

// Snake Game Variables
const int boxSize = 4;
const int maxLength = (SCREEN_WIDTH / boxSize) * (SCREEN_HEIGHT / boxSize);
int snakeX[maxLength], snakeY[maxLength];
int snakeLength = 5;
int foodX, foodY;
int score = 0;

int dirX = 1;  // 1: Right, -1: Left
int dirY = 0;  // 1: Down, -1: Up

bool gameOver = false;
bool gameStarted = false;

// Calibration Variables
int joystickCenterX = 0;
int joystickCenterY = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialization failed");
    while (true);
  }

  pinMode(SW_PIN, INPUT_PULLUP);

  calibrateJoystick();
  showStartScreen();
  resetGame();
}

void loop() {
  if (!gameStarted) {
    if (digitalRead(SW_PIN) == LOW) {
      gameStarted = true;
    }
    return;
  }

  if (gameOver) {
    displayGameOver();
    return;
  }

  readJoystick();
  moveSnake();
  checkCollision();
  drawGame();

  delay(150);
}

void calibrateJoystick() {
  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("Calibrating...");
  display.display();

  delay(2000);

  joystickCenterX = analogRead(VRX_PIN);
  joystickCenterY = analogRead(VRY_PIN);
}

void showStartScreen() {
  display.clearDisplay();
  display.setCursor(20, 20);
  display.println("Press Button to Start");
  display.display();
}

void readJoystick() {
  int xValue = analogRead(VRX_PIN) - joystickCenterX;
  int yValue = analogRead(VRY_PIN) - joystickCenterY;

  if (abs(xValue) > abs(yValue)) {
    if (xValue > 800 && dirX == 0) {
      dirX = 1; dirY = 0;
    } else if (xValue < -800 && dirX == 0) {
      dirX = -1; dirY = 0;
    }
  } else {
    if (yValue > 800 && dirY == 0) {
      dirX = 0; dirY = 1;
    } else if (yValue < -800 && dirY == 0) {
      dirX = 0; dirY = -1;
    }
  }
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] += dirX * boxSize;
  snakeY[0] += dirY * boxSize;

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    score++;
    spawnFood();
  }
}

void checkCollision() {
  if (snakeX[0] < boxSize || snakeX[0] >= SCREEN_WIDTH - boxSize ||
      snakeY[0] < boxSize || snakeY[0] >= SCREEN_HEIGHT - boxSize) {
    gameOver = true;
    return;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true;
      return;
    }
  }
}

void spawnFood() {
  do {
    foodX = (random(1, (SCREEN_WIDTH / boxSize) - 2) * boxSize);
    foodY = (random(1, (SCREEN_HEIGHT / boxSize) - 2) * boxSize);
  } while (checkFoodCollision());
}

bool checkFoodCollision() {
  for (int i = 0; i < snakeLength; i++) {
    if (snakeX[i] == foodX && snakeY[i] == foodY) {
      return true;
    }
  }
  return false;
}

void drawGame() {
  display.clearDisplay();

  // Draw Border
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  // Draw Snake
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i], snakeY[i], boxSize, boxSize, SSD1306_WHITE);
  }

  // Draw Food
  display.fillRect(foodX, foodY, boxSize, boxSize, SSD1306_WHITE);

  // Draw Score
  display.setCursor(5, 5);
  display.print("Score: ");
  display.print(score);

  display.display();
}

void displayGameOver() {
  display.clearDisplay();
  display.setCursor(20, 20);
  display.println("Game Over!");
  display.setCursor(20, 35);
  display.print("Score: ");
  display.print(score);
  display.setCursor(20, 50);
  display.println("Press to Restart");
  display.display();

  if (digitalRead(SW_PIN) == LOW) {
    resetGame();
  }
}

void resetGame() {
  snakeLength = 5;
  score = 0;
  dirX = 1;
  dirY = 0;
  gameOver = false;

  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = SCREEN_WIDTH / 2 - (i * boxSize);
    snakeY[i] = SCREEN_HEIGHT / 2;
  }
  spawnFood();
} 
