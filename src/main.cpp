/**************************************************************************
 * Written by Marco Hendra Dani
 * mdani@uw.edu
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Shape.hpp>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'pixil-frame-0 (1)', 20x20px
const unsigned char dino [] PROGMEM = {
	0x00, 0x1f, 0xe0, 0x00, 0x37, 0xf0, 0x00, 0x3f, 0xf0, 0x00, 0x3f, 0xf0, 0x00, 0x3f, 0xf0, 0x00, 
	0x3f, 0x00, 0x00, 0x7f, 0xe0, 0x80, 0xff, 0x00, 0xc1, 0xff, 0x00, 0xe1, 0xff, 0xc0, 0xf3, 0xff, 
	0x40, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x7f, 0xff, 0x00, 0x3f, 0xde, 0x00, 0x03, 0x9c, 0x00, 
	0x01, 0x08, 0x00, 0x01, 0x08, 0x00, 0x01, 0x08, 0x00, 0x01, 0x8c, 0x00
};

const unsigned char dino_fly [] PROGMEM = {
	0x00, 0x01, 0x80, 0x00, 0x03, 0xc0, 0x00, 0x07, 0xc0, 0x80, 0x07, 0xf0, 0x80, 0x07, 0x70, 0x80, 
	0x0f, 0xf0, 0x80, 0x1f, 0xf0, 0xc0, 0x3f, 0xc0, 0xc0, 0x7e, 0xf0, 0xe1, 0xff, 0x00, 0xff, 0xff, 
	0x00, 0xff, 0xff, 0x00, 0x3f, 0xff, 0x00, 0x3f, 0xfe, 0x00, 0x1f, 0xfc, 0x00, 0x0f, 0xe0, 0x00, 
	0x7c, 0x7f, 0x00, 0x78, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char ptero_sprite [] PROGMEM = {
	0x04, 0x0f, 0xe0, 0x0e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x37, 0xff, 0xf0, 0xff, 0xff, 0x80, 0x3f, 
	0xff, 0xe0, 0x00, 0xfe, 0x00
};

// 'dino-eyes-mouth-closed', 20x20px
const unsigned char dino_eyes_mouth_closed [] PROGMEM = {
	0x1f, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0xe1, 0xff, 0xf0, 0xe1, 0xff, 0xf0, 0xe1, 0xff, 0xf0, 0xe1, 
	0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 
	0xf0, 0xff, 0xff, 0xf0, 0xff, 0x49, 0x20, 0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 
	0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00
};
// 'dino-eyes-mouth-open', 20x20px
const unsigned char dino_eyes_mouth_open [] PROGMEM = {
	0x1f, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0xe1, 0xff, 0xf0, 0xed, 0xff, 0xf0, 0xed, 0xff, 0xf0, 0xe1, 
	0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0xff, 0xff, 
	0xf0, 0xff, 0xff, 0xf0, 0xff, 0x49, 0x20, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xfe, 0x00, 
	0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 0xff, 0xfe, 0x00, 0x01, 0xfe, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 5;
const unsigned char* epd_bitmap_allArray[5] = {
	dino,
	dino_fly,
	ptero_sprite,
	dino_eyes_mouth_open,
	dino_eyes_mouth_closed
};

const int BUTTON = 4;
const int LED = 8;
const int MINAMP = 150;
const int BUZZER = 9;

const int SCORE_HEIGHT = 5;
const int SPAWN_X = 118;
const int CACTUS_SPAWN_Y = 52;
const int PTERODACTYL_SPAWN_Y = SCORE_HEIGHT + 1;

// cactus constants
int cactus_x = SPAWN_X;
int cactus_y = CACTUS_SPAWN_Y;
int cactus_speed = 4;
int cactus_size = 12; // size of sprite
int cactus_width = 5;
const int MAX_CACTUS_HEIGHT = 12;
const int NUM_CACTUS = 2;

// pterodactyl constants
int pterodactyl_x = SPAWN_X;
int pterodactyl_y = PTERODACTYL_SPAWN_Y;
int pterodactyl_height = 7;
int pterodactyl_width = 20;
int pterodactyl_speed = 1;

// Dinosaurs constants
const int DINO_WIDTH = 20;
const int DINO_HEIGHT = 20;
const int DINO_X_HIT = 15;
const int DINO_Y_HIT = 15;
const int INITIAL_V = -11;
int dino_jump_velocity = 0;
int dino_x = 0; 
int dino_y = SCREEN_HEIGHT - DINO_HEIGHT;
int dino_speed = 6;
const int DINO_GROUND = SCREEN_HEIGHT - DINO_HEIGHT;

// States
enum GameState {
	NEW_GAME,
	PLAYING,
	GAME_OVER,
	SCREAM,
};

const char START_MENU_STR[] = "Press button to play";

GameState _gameState = NEW_GAME;

int _gameOverTimeStamp = 0;
uint16_t blinky = 1;
unsigned long prev_millis = 0;
unsigned long start_menu_millis = 0;

int score = 0;
int hi_score = 0;

// helper function declaration
bool button_pressed(int button);
int listen();

class Cactus : public Rectangle {
  protected:
    bool _collide = false;

  public:
    Cactus(int x, int y, int width, int height) : Rectangle(x, y, width, height){}
};


class Ptero : public Rectangle {
	public:
	  Ptero(int x, int y, int width, int height) : Rectangle(x, y, width, height){}

	  void drawPtero(Adafruit_SSD1306& display, int x, int y) {
		display.drawBitmap(x, y, epd_bitmap_allArray[2], pterodactyl_width, pterodactyl_height, 1);
	  }
};

Cactus cactuses[NUM_CACTUS] = {
	Cactus(SPAWN_X, CACTUS_SPAWN_Y, cactus_width, cactus_size),
	Cactus(SPAWN_X, CACTUS_SPAWN_Y, cactus_width, cactus_size),

};

Ptero ptero = Ptero(0, PTERODACTYL_SPAWN_Y, pterodactyl_width, pterodactyl_height);

bool is_colliding(int a_x, int a_y, int a_width, int a_height, int b_x, int b_y, int b_width, int b_height) {
	bool OverX = (a_x < b_x + b_width) && (a_x + a_width > b_x);
	bool OverY = (a_y < b_y + b_height) && (a_y + a_height > b_y);
	return OverX && OverY;
}


void initEntities() {
	// set dinosaur spawn point
	dino_x = 0;
	dino_y = SCREEN_HEIGHT - DINO_HEIGHT;

	// initialize cactuses
	// right-most spawn point for cactus
	const int min_cactus_spawn_x = (SCREEN_WIDTH / 2) + 10;
	int prev_x = min_cactus_spawn_x;
	for (int i = 0; i < NUM_CACTUS; i++) {
		int space = random(50, 90);
		cactus_size = random(8, MAX_CACTUS_HEIGHT);
		cactuses[i].setX(space + prev_x);
		cactuses[i].setY(SCREEN_HEIGHT - cactus_size);
		cactuses[i].setDimensions(cactus_width, cactus_size);
		prev_x = cactuses[i].getX() + cactus_width;
	}

	// initialize pterodactyl
	ptero.setX(SCREEN_WIDTH);
	ptero.setY(PTERODACTYL_SPAWN_Y);
	ptero.setDimensions(pterodactyl_width, pterodactyl_height);

}

void nonGameLoop() {
	int16_t x1, y1, x2, y2;
	int16_t w, h;

	if (_gameState == NEW_GAME) {
		display.getTextBounds(START_MENU_STR, 0, 0, &x1, &y1, &w, &h);
		display.setCursor(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - 10);
		if (millis() - prev_millis > 500) {
			prev_millis = millis();
			if (blinky == 1) {
				blinky = 0;
				display.clearDisplay();
				display.drawBitmap(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + h, epd_bitmap_allArray[3], 20, 20, 1);
			}
			else {
				blinky = 1;
				display.clearDisplay();
				display.drawBitmap(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + h, epd_bitmap_allArray[4], 20, 20, 1);

			};
		}
		display.setTextColor(blinky);
		display.print(START_MENU_STR);
		display.setTextColor(1);

		
		if (button_pressed(BUTTON)) {
			initEntities();
			_gameState = PLAYING;
		}
		
		// screaming mode
		if (listen() > MINAMP) {
			initEntities();
			_gameState = SCREAM;
		}

	} else if (_gameState == GAME_OVER) {
		display.getTextBounds("GAME OVER", 0, 0, &x1, &y1, &w, &h);
		display.setCursor(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - 10);
		display.print("GAME OVER");
		display.getTextBounds("Press to continue", 0, 0, &x1, &y1, &w, &h);
		display.setCursor(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + h - 10);
		display.print("Press to continue");

		if (button_pressed(BUTTON)) {
			initEntities();
			_gameState = PLAYING;
		} else if (listen() > MINAMP) {
			initEntities();
			_gameState = SCREAM;
		}
	}
}

bool button_pressed(int pin) {
	if (digitalRead(pin) > 0) {
		Serial.println("Button pressed");
		return true;
	}
	return false;
}

void gamePlayLoop() {
	// move x of cactuses
	int interval = random(50, 90);
	for (int i = 0; i < NUM_CACTUS; i++) {
		// check if cactus out of bound
		// generate new one with random interval
		randomSeed(analogRead(A0));
		int new_height = random(8,MAX_CACTUS_HEIGHT);
		int new_x = cactuses[i].getX() - cactus_speed;
		if (new_x + cactus_width + interval < 0) {
			// spawn new cactus from the right of the screen
			// change cactus height
			new_x = SCREEN_WIDTH - cactus_speed;
			cactuses[i].setY(SCREEN_HEIGHT - new_height);
			cactuses[i].setDimensions(cactus_width, new_height);

			// increment score
			score++;
			
		}
		cactuses[i].setX(new_x);
		cactuses[i].draw(display);

		// dino collision
		if (is_colliding(dino_x, dino_y, DINO_X_HIT, DINO_Y_HIT,
			cactuses[i].getX(), cactuses[i].getY(), 
			cactuses[i].getWidth(), cactuses[i].getHeight())) {
				hi_score = max(score, hi_score);
				score = 0;
				tone(7, 500, 200);
				// buzz
				digitalWrite(BUZZER, HIGH);
				delay(500);
				digitalWrite(BUZZER, LOW);

				_gameState = GAME_OVER;
				_gameOverTimeStamp = millis();
				nonGameLoop();
			}
	}

	// handle pterodactyl generation and movement
	ptero.setX(ptero.getX() - pterodactyl_speed);
	if (ptero.getX() + pterodactyl_width + interval  < 0) {
		ptero.setX(SCREEN_WIDTH);
		score++;
	}

	// pterodactyl collision
	if (is_colliding(dino_x, dino_y, DINO_X_HIT, DINO_Y_HIT,
					ptero.getX(), ptero.getY(), pterodactyl_width, pterodactyl_height)) {
						hi_score = max(score, hi_score);
						score = 0;
						tone(7, 500, 200);
						// buzz and delay
						digitalWrite(BUZZER, HIGH);
						delay(500);
						digitalWrite(BUZZER, LOW);
						_gameState = GAME_OVER;
						_gameOverTimeStamp = millis();
						nonGameLoop();
					}

	// handle jump
	// change gravity based on whether jump is activated
	// for a more controllable and parabolic jump feel
	int gravity = 2;
	if ((button_pressed(BUTTON) && dino_y == DINO_GROUND && _gameState == PLAYING) || 
			(_gameState == SCREAM && listen() > MINAMP && dino_y == DINO_GROUND)) {
		// apply jump force
		dino_jump_velocity = INITIAL_V;
		tone(7, 200, 100);
	}
	
	if (dino_y > SCREEN_HEIGHT - DINO_HEIGHT) {
		dino_jump_velocity = 0;
	}

	dino_y = min(dino_y + dino_jump_velocity, DINO_GROUND);
	if ((_gameState == PLAYING && (button_pressed(BUTTON))) || (_gameState == SCREAM && listen() > MINAMP)) {
		dino_jump_velocity += gravity;
	} else {
		dino_jump_velocity += gravity + 3;
	}
	
	// draw dino
	if (dino_y < DINO_GROUND) {
		display.drawBitmap(dino_x, dino_y, epd_bitmap_allArray[1], DINO_WIDTH, DINO_HEIGHT, 1);
	} else {
		display.drawBitmap(dino_x, dino_y, epd_bitmap_allArray[0], DINO_WIDTH, DINO_HEIGHT, 1);
	}

	// draw ptero
	ptero.drawPtero(display, ptero.getX(), ptero.getY());

	// led activation
	if (score > hi_score) {
		digitalWrite(LED, HIGH);
	} else {
		digitalWrite(LED, LOW);
	}
}


void setup() {
  Serial.begin(9600);
  display.setCursor(100, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(200);
  display.clearDisplay();
}

// calculates voice amplitude with sample audio 5ms
int listen() {
	int signalMax = 676;
	int soundlevel = analogRead(A1);
	int amplitude = map(soundlevel, signalMax / 2, signalMax, 0, 676);
	
  return amplitude;
}

void loop() {
	// refresh display every loop
	if (_gameState == NEW_GAME || _gameState == GAME_OVER) {
		// non gameplay loop
		nonGameLoop();
	} else {
		// print score
		display.clearDisplay();
		display.setCursor(60, 0);
		display.print("score: ");
		display.print(score);
		display.setCursor(0,0);
		display.print("Hi: ");
		display.print(hi_score);
		// gameplay loop
		gamePlayLoop();
	}

	// display
	display.display();
}


