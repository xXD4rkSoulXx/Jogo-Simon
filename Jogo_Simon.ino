#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_YELLOW 2
#define LED_BLUE 3
#define LED_RED 4
#define LED_GREEN 5

#define BUTTON_YELLOW 8
#define BUTTON_BLUE 9
#define BUTTON_RED 10
#define BUTTON_GREEN 11
#define BUTTON_RESET 12

#define BUZZER 13

#define STATE_START_GAME 0
#define STATE_SHOW_SEQUENCE 1
#define STATE_PLAYER_TURN 2
#define STATE_GAME_OVER 3
#define STATE_VICTORY 4

const int MAX_ROUNDS = 20;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int gameState = STATE_START_GAME;

int gameSequence[MAX_ROUNDS];
int currentRound = 1;
int playerStep = 0;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);
  
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  pinMode(BUZZER, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jogo do Simon!");
  
  tone(BUZZER, 1047);
  delay(40);
  tone(BUZZER, 1319);
  delay(40);
  tone(BUZZER, 1568);
  delay(40);
  tone(BUZZER, 2093);
  delay(150);
  noTone(BUZZER);
  delay(1730);
}

void loop()
{
  if (digitalRead(BUTTON_RESET) == LOW) {
    for (int i = 150; i < 800; i += 15) {
      tone(BUZZER, i, 8);
      delay(10);
    }

    tone(BUZZER, 1800, 50); 
    delay(80);
    noTone(BUZZER);
    delay(50);

    for (int i = 1200; i > 60; i -= 25) {
      tone(BUZZER, i, 12);
      delay(8);
    }

    noTone(BUZZER);
    
    resetGameData();
    return;
  }
  
  switch (gameState) {
    case STATE_START_GAME: {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ronda: ");
      lcd.print(currentRound);
      lcd.setCursor(0, 1);
      lcd.print("Presta atencao!");
      delay(1500);
      
      generateSequence();
      gameState = STATE_SHOW_SEQUENCE;
      break;
    }
    case STATE_SHOW_SEQUENCE: {
      showSequence();
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sua vez!");
      
      playerStep = 0;
      gameState = STATE_PLAYER_TURN;
      break;
    }
    case STATE_PLAYER_TURN: {
      int pressedButton = readPlayerInput();
      
      if (pressedButton != -1) {
        buttonFeedback(pressedButton);

        if (pressedButton != gameSequence[playerStep]) {
          triggerGameOverScreen();
          gameState = STATE_GAME_OVER;
        } else {
          playerStep++;
          
          if (playerStep >= currentRound) {
            if (currentRound == MAX_ROUNDS) {
              triggerVictoryScreen();
              gameState = STATE_VICTORY;
            } else {
              triggerNextRoundScreen();
              gameState = STATE_START_GAME;
            }
          }
        }
      }
      
      break;
    }
    case STATE_GAME_OVER: {
      break;
    }
    case STATE_VICTORY: {
      break;
    }
  }
}

void resetGameData() {
  currentRound = 1;
  playerStep = 0;
  
  for (int i = 0; i < MAX_ROUNDS; i++) {
    gameSequence[i] = 0;
  }
  
  lcd.clear();
  lcd.print("A reiniciar...");
  delay(1000);
  
  lcd.clear();
  lcd.print("Jogo do Simon!");
  delay(1500);
  
  gameState = STATE_START_GAME; 
  
  while (digitalRead(BUTTON_RESET) == LOW) {
    delay(10);
  }
}

void generateSequence() {
  int positionIndex = currentRound - 1;
  
  gameSequence[positionIndex] = random(0, 4);
}

void showSequence() {
  for (int i = 0; i < currentRound; i++) {
    if (digitalRead(BUTTON_RESET) == LOW) {
      return;
    }
    
    int colorCode = gameSequence[i];
    int ledPin = 0;

    switch (colorCode) {
      case 0:
		ledPin = LED_GREEN;
		break;
      case 1:
		ledPin = LED_RED;
		break;
      case 2:
		ledPin = LED_YELLOW;
		break;
      case 3:
		ledPin = LED_BLUE;
		break;
      default: {
        Serial.print("ERRO CRITICO: colorCode invalido recebido -> ");
        Serial.println(colorCode);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ERRO DE SISTEMA");
        lcd.setCursor(0, 1);
        lcd.print("Reinicie o jogo");

        while (true); 
        break;
      }
    }

    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

int readPlayerInput() {
  if (digitalRead(BUTTON_GREEN) == LOW) {
    return 0;
  }

  if (digitalRead(BUTTON_RED) == LOW) {
    return 1;
  }

  if (digitalRead(BUTTON_YELLOW) == LOW) {
    return 2;
  }

  if (digitalRead(BUTTON_BLUE) == LOW) {
    return 3;
  }
  
  return -1;
}

void buttonFeedback(int colorCode) {
  int ledPin = 0;
  int frequency = 0;

  switch (colorCode) {
    case 0:
      ledPin = LED_GREEN;
      frequency = 262;
      break;
    case 1:
      ledPin = LED_RED;
      frequency = 294;
      break;
    case 2:
      ledPin = LED_YELLOW;
      frequency = 330;
      break;
    case 3:
      ledPin = LED_BLUE;
      frequency = 349;
      break;
    default:
      return;
  }

  digitalWrite(ledPin, HIGH);
  tone(BUZZER, frequency);

  while (readPlayerInput() == colorCode) {
    if (digitalRead(BUTTON_RESET) == LOW) {
      break;
    }
    
    delay(10);
  }

  digitalWrite(ledPin, LOW);
  noTone(BUZZER);
  delay(100);
}

void triggerGameOverScreen() {
  tone(BUZZER, 1109);
  delay(40);
  tone(BUZZER, 1319);
  delay(40);
  tone(BUZZER, 1568);
  delay(40);
  tone(BUZZER, 1865);
  delay(240);
  noTone(BUZZER);  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jogo Terminou!");
  lcd.setCursor(0, 1);
  lcd.print("Pontos: ");
  lcd.print(currentRound - 1);
}

void triggerVictoryScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MEMORIA LENDARIA");
  lcd.setCursor(0, 1);
  lcd.print("Ganhaste o Jogo!");
  
  tone(BUZZER, 988);
  delay(180);
  tone(BUZZER, 988);
  delay(180);
  tone(BUZZER, 988);
  delay(100);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 880);
  delay(160);
  tone(BUZZER, 988);
  delay(160);
  tone(BUZZER, 1175);
  delay(180);
  tone(BUZZER, 988);
  delay(180);
  noTone(BUZZER);
  delay(180);
  tone(BUZZER, 880);
  delay(180);
  tone(BUZZER, 988);
  delay(240);
  noTone(BUZZER);
  delay(200);
  tone(BUZZER, 784);
  delay(180);
  noTone(BUZZER);
  delay(120);
  tone(BUZZER, 784);
  delay(240);
  tone(BUZZER, 740);
  delay(240);
  tone(BUZZER, 740);
  delay(240);
  noTone(BUZZER);
  delay(120);
  tone(BUZZER, 880);
  delay(200);
  tone(BUZZER, 932);
  delay(400);
  
  noTone(BUZZER);
}

void triggerNextRoundScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acertou!");
  lcd.setCursor(0, 1);
  lcd.print("Proxima ronda...");
  
  currentRound++;
  delay(2000);
}
