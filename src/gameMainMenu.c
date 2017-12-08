#include <stdint.h>
#include <pic32mx.h>
#include "gameHeader.h"
#include "entities.h"

void mainMenuScreen() {
    int startX = 36;
    int k;
    for (k = 0; k < 512; k++) {
        /* Draw line above the word Stranger */
        if(k >= startX && k<startX+55) dataArray[k] = 0x80;
        /* Zero the array */
        else dataArray[k] = 0x0;
    }
    /* Top line */
    /* Side lines below S and R*/
    int i;
    for (i = 0; i < 6; i++) {
        displayPixel(36 + i,19); // left line on things.
        displayPixel(85 + i,19); // right line on things.
    }
    /* Normal letters */
    displayString(startX + 7,1,"TRANGE");
    displayString(startX + 7,2,"THINGS");

    /* Larger S top */
    displayHex(startX, 1,0x9C);
    displayHex(startX + 1, 1,0xB6);
    displayHex(startX + 2, 1,0x22);
    displayHex(startX + 3, 1,0x22);
    displayHex(startX + 4, 1,0x66);
    displayHex(startX + 5, 1,0xCC);
    /* Larger S bottom */
    displayHex(startX, 2,0x1);
    displayHex(startX + 1, 2,0x2);
    displayHex(startX + 2, 2,0x2);
    displayHex(startX + 3, 2,0x2);
    displayHex(startX + 4, 2,0x3);
    displayHex(startX + 5, 2,0x1);

    /* Larger R top */
    displayHex(startX + 49, 1,0xFE);
    displayHex(startX + 49 + 1, 1,0xFE);
    displayHex(startX + 49 + 2, 1,0x22);
    displayHex(startX + 49 + 3, 1,0x22);
    displayHex(startX + 49 + 4, 1,0x76);
    displayHex(startX + 49 + 5, 1,0xDC);
    /* Larger R bottom */
    displayHex(startX + 49, 2,0x3);
    displayHex(startX + 49 + 1, 2,0x3);
    displayHex(startX + 49 + 5, 2,0x3);
    displayHex(startX + 49 + 6, 2,0x2);
}

void animation() {
    player.type = PLAYER;
    player.x = -2;
    player.y = FLOOR_Y_UP;
    player.playerScore = 0;
    player.jumping = 0;
    player.crouching = 0;
    player.legDown = 0;
    player.hitbox.width = 4;
    player.hitbox.height = 6;

    int counter = 0;
    Monster monster;
    monster.x = -30;
    int legDownMonster = 1;
    while (counter < 1500) {
        binaryNumber = getsw();
        if(counter % 10 == 0) {
            player.x += 1;
            monster.x += 1;
            updatePlayer();
        }
        if(counter % 50 == 0) legDownMonster = !legDownMonster;
        mainMenuScreen();
        render(player.type,player.x,player.y);
        renderMonster(monster.x, FLOOR_Y_UP, legDownMonster);
        display_update();
        counter++;
    }
}

/* Code duplication!
*/
void updateMainMenu() {
    mainMenuScreen();
    display_update();
    int buttons = getbtns();
    binaryNumber = getsw();
    /* A button is pressed */
    if(buttons != 0) {
        animation();
        GAMESTATE = 2;
        if(buttons == 1)      DIFFICULTY = 16; //Right most button
        else if(buttons == 2) DIFFICULTY = 8; //Middle button
        else if(buttons == 4) DIFFICULTY = 4; //Left most button
    }
}
