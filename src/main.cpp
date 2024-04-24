/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	dino
};




const int SCORE_HEIGHT = 5;
const int SPAWN_X = 118;
const int CACTUS_SPAWN_Y = 52;
const int PTERODACTYL_SPAWN_Y = SCORE_HEIGHT + 1;
int cactus_x = SPAWN_X;
int cactus_y = CACTUS_SPAWN_Y;
int cactus_speed = 2;  // lower speed = faster
int pterodactyl_speed = 1;
int cactus_size = 12; // size of sprite
int cactus_width = 5;

int pterodactyl_x = SPAWN_X;
int pterodactyl_y = PTERODACTYL_SPAWN_Y;
int pterodactyl_size = 5;
int pterodactyl_width = 12;

int score = 0;

const int DINO_WIDTH = 20;
const int DINO_HEIGHT = 20;
const int DINO_X_HIT = 15;
const int DINO_Y_HIT = 15;
int dino_x = 0; 
int dino_y = SCREEN_HEIGHT - DINO_HEIGHT;

bool is_colliding(int a_x, int a_y, int a_width, int a_height, int b_x, int b_y, int b_width, int b_height) {
	bool OverX = (a_x < b_x + b_width) && (a_x + a_width > b_x);
	bool OverY = (a_y < b_y + b_height) && (a_y + a_height > b_y);
	return OverX && OverY;
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
  delay(2000);
  display.clearDisplay();
}

void loop() {
	int dino_control = analogRead(A0);
	dino_control = map(dino_control, 0, 1023, SCREEN_HEIGHT - DINO_HEIGHT, 6);
	dino_y = dino_control;
	// draw score
	display.setCursor(60, 0);
	display.print("score: ");
	display.print(score);
	// draw pterodactyl
	display.fillRect(pterodactyl_x, pterodactyl_y, pterodactyl_width, pterodactyl_size, 1);
	// draw cactus
	display.fillRect(cactus_x, cactus_y, cactus_width, cactus_size, 1);
	// draw dino
	display.drawBitmap(dino_x, dino_y, epd_bitmap_allArray[0], DINO_WIDTH, DINO_HEIGHT, 1);

	display.display();
	display.clearDisplay();
	cactus_x -= 3;
	pterodactyl_x -= 2;

	// handle cactus generation
	if (cactus_x < -10) {
		cactus_x = SPAWN_X;
		cactus_y = CACTUS_SPAWN_Y;  // reset y-coordinate
		// varrying cactus height
		cactus_size = random(8,20); // max height of cactus = 20
		cactus_y = SCREEN_HEIGHT - cactus_size;
		score++;
	}

	// handle pterodactyl generation
	if (pterodactyl_x < -pterodactyl_width) {
		pterodactyl_x = SPAWN_X;
		pterodactyl_y = random(SCORE_HEIGHT + 1,15); 
		score++;
	}

	// Collision
	if (is_colliding(dino_x, dino_y, DINO_WIDTH, DINO_HEIGHT, cactus_x, cactus_y, cactus_width, cactus_size)) {
		score = 0;
	}
	
	
}


