//-----------------------------
// Written by johvh & davidjo2.
//-----------------------------

#include <stdint.h>
#include <pic32mx.h>
#include "displayData.h"
#include "gameHeader.h"

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

#define DATA_ARRAY_SIZE 512

/**
 * Function to sleep X cycles.
 * @param cycles
 */
void sleep(int cycles) {
	int i;
	for(i = cycles; i > 0; i--);
}

/**
 * Send data to display buffer.
 */
void sendSPI(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
}

/**
 * Add a single pixel to the render buffer at specified position.
 */
void displayPixel(int x, int y) {
	if(x<128 && y<32 && !(x < 0) && !(y < 0)) {
		int yOffset = y % 8;            // array pos value (0-8, where 0 is upper pixel, 8 is lowest pixel)
		int page = y / 8;               // page position index
		int arrayPos = page*128 + x;    // position in the array (0-512)

		/* OR pixel with current value in the column. (1 = 1, 2 = 10, 3 = 100 ...) */
		dataArray[arrayPos] = dataArray[arrayPos] | (0x1 << yOffset);
	}
}

/**
 * Add a hex to the render buffer at specified location.
 */
void displayHex(int x, int line, int value) {
	if(x<128 && x>=0 && line >= 0 && line < 4) {
		int arrayPos = 128*line + x;
		dataArray[arrayPos] = dataArray[arrayPos] | value;
	}
}

/**
 * Draw a string on the display.
 */
void displayString(int x, int line, char* string) {
	const char* i;
	int j;
	int k = x;
	for (i = string; *i!='\0'; i++) {
		char c = *i;
		/* Dont draw outside the screen */
		if(j + k > 128) {
			continue;
		}
		/* Space character */
		if(c == 32) {
			k += 4;
			continue;
		}
		/* Display every hex value of a char */
		for (j = 0; j<5; j++) {
			/* Capital letters */
			if(c >= 65 && c <= 90) {
				dataArray[j + k + line*128] = charArray[(c - 65)*5 + j];
			/* Normal letters */
			} else if(c >= 97 && c <= 122) {
				dataArray[j + k + line*128] = charArray[(c - 65 - 32)*5 + j];
			/* Digits */
			} else if(c >= 48 && c <= 57) {
				dataArray[j + k + line*128] = charArray[(c - 48 + 26)*5 + j];
			}
		}
		/* Next letter and add space. */
		k += 7;
	}
}

/**
 * Update the display.
 */
void display_update(void) {
	// send render buffer to screen
	int i;
	for(i=0; i<DATA_ARRAY_SIZE; i++) {
		sendSPI(dataArray[i]);
	}
}

/**
 * Initialise display.
 */
void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    sleep(10);
	DISPLAY_ACTIVATE_VDD;
    sleep(1000000);

    sendSPI(0xAE);
	DISPLAY_ACTIVATE_RESET;
    sleep(10);
	DISPLAY_DO_NOT_RESET;
    sleep(10);

    sendSPI(0x8D);
    sendSPI(0x14);

    sendSPI(0xD9);
    sendSPI(0xF1);

	DISPLAY_ACTIVATE_VBAT;
    sleep(10000000);

    sendSPI(0xA1);
    sendSPI(0xC8);

    sendSPI(0xDA);
    sendSPI(0x20);

	sendSPI(0x20); // Set addressing mode
	sendSPI(0x0);  // Horizontal addressing mode

    sendSPI(0xAF); // Turn on display
	sleep(100);
	DISPLAY_CHANGE_TO_DATA_MODE;
}
