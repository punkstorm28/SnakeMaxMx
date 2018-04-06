

#include <MaxMatrix.h>

/* Joystick PINs */
#define VRX     A0
#define VRY     A1
#define SW      2

/* Display PINs */
#define CLK     6
#define CS      5
#define DIN     7
int maxInUse = 2;

#define SIZE    8
#define ADVANCE_DELAY 30

MaxMatrix m(DIN, CS, CLK, maxInUse); 

int snake[16*8][2];
int length;
int food[2], v[2];
bool is_game_over = false;
long current_time;
long prev_advance;
int blink_count;
const short messageSpeed = 5;




void init_game() {
  prev_advance = current_time = 0;
  blink_count = 3;
  int half = SIZE / 2;
  length = SIZE / 3;

  for (int i = 0; i < length; i++) {
      snake[i][0] = half - 1;
      snake[i][1] = half + i;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  v[0] = 0;
  v[1] = -1;
}

void render() {
  for (int i = 0; i < length; i++) {
    m.setDot(snake[i][0], snake[i][1], true);
  }
  m.setDot(food[0], food[1], true);
}

void clearScreen() {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      m.setDot(x, y, false);
    }
  }
}

/**
 * moves the snake forward
 * returns true if the game is over
 */
bool advance() {
  int head[2] = {snake[0][0] + v[0], snake[0][1] + v[1]};

  if (head[0] < 0 || head[0] >= SIZE) {

            delay(1000);
    showGameOverMessage();
      return true;
  }

  if (head[1] < 0 || head[1] >= SIZE) {

            delay(1000);
    showGameOverMessage();
      return true;
  }

  for (int i = 0; i < length; i++) {
      if (snake[i][0] == head[0] && snake[i][1] == head[1]) {
            delay(1000);
        showGameOverMessage();
          return true;
      }
  }

  bool grow = (head[0] == food[0] && head[1] == food[1]);
  if (grow) {
      length++;  
      randomSeed(current_time);    
      food[0] = random(SIZE);
      food[1] = random(SIZE*2);
  }

  for (int i = length - 1; i >= 0; i--) {
      snake[i + 1][0] = snake[i][0];
      snake[i + 1][1] = snake[i][1];
  }
  snake[0][0] += v[0];
  snake[0][1] += v[1];
  return false;
}

void setup() {
  m.init(); 
  m.setIntensity(15); 

  pinMode(SW, INPUT_PULLUP);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  attachInterrupt(digitalPinToInterrupt(SW), restart, RISING);

  init_game();
  render();
}

void loop() {
  if (!is_game_over) {
    clearScreen();
    render();

    if (current_time - prev_advance > ADVANCE_DELAY) {
      is_game_over = advance();
      prev_advance = current_time;    
    }
  } else {
    while (blink_count > 0) {
      clearScreen();
      delay(300);
      render();
      delay(300);
      blink_count--;     
         
    }
  }
  readControls();
  current_time++;
}

void restart() {  
  init_game();
  is_game_over = false;
}

void readControls() {
  int dx = map(analogRead(VRX), 0, 906, 2, -2);
  int dy = map(analogRead(VRY), 0, 906, 2, -2);
  if (dx != 0) {dx = dx / abs(dx);}
  if (dy != 0) {dy = dy / abs(dy);}

  if (dy != 0 && v[0] != 0) {
    v[0] = 0;
    v[1] = dy;    
  }

  if (dx != 0 && v[1] != 0) {
    v[0] = dx;
    v[1] = 0;
  }

}

const PROGMEM bool gameOverMessage[8][90] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}
}; 
void showGameOverMessage() {
  for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
    for (int col = 0; col < 16; col++) {
      delay(messageSpeed);
      for (int row = 0; row < 8; row++) {
        // this reads the byte from the PROGMEM and displays it on the screen
        m.setDot(col, row, pgm_read_byte(&(gameOverMessage[row][col + d])));
      }
    }
  }
}
