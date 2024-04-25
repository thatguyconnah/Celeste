#include <gb/gb.h>
#include <stdio.h>
#include "Tiles/celeste_tiles.h"
#include "Tiles/background.h"
#include "Structure/Character.h"

#define PLAYER_1_ANIMATION_FRAMES 4;

Character player1;
uint8_t frameCount = 0;
int8_t lastMovementX = 0;
int8_t lastMovementY = 0;
int8_t slowingX = 0;

void SetUpBackground(void)
{
    set_bkg_data(0, background_tileset_size, background_tileset);

    set_bkg_tiles(0, 0, 20, 18, background_tilemap);
}

void MovementPhysics(Character *character, uint8_t slowDownFrames)
{
    uint8_t inAir = 0;
    inAir = character-> y < 128;

    if((lastMovementX != 0 && character->movementForceX == 0) || (lastMovementX == 0 && character->movementForceX != 0))
    {
        frameCount = 0;
    }

    if(lastMovementX != 0 && character->movementForceX == 0)
    {
        slowingX = lastMovementX > 0 ? 1 : -1;
    }

    if(character->movementForceX != 0)
    {
        if(frameCount == 2 || frameCount == 4)
        {
            character->velocityX = 0;
        }
        else
        {
            character->velocityX = character->movementForceX;
        }

        frameCount++;

        if(frameCount == 4)
        {
            frameCount = 0;
        }
    }
    else
    {
        if(slowingX && frameCount < slowDownFrames)
        {
            frameCount++;

            if(frameCount == slowDownFrames - 1)
            {
                character->velocityX = 0;
                slowingX = 0;
            }
            else
            {
                character->velocityX = slowingX;
            }
        }
    }
    uint8_t predictedX = character->x + character->velocityX;
    MoveCharacter(character, predictedX, character->y);

    lastMovementX = character->movementForceX;
    lastMovementY = character->movementForceY;
}

void main(void)
{
    SetUpBackground();

    set_sprite_data(0, celeste_tileset_size, celeste_tileset);

    SetupCharacter(&player1, 0, 2, 2, 0, 4, celeste_tilemap);

    MoveCharacter(&player1, 8, 128);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while(1)
    {
        MoveCharacterwithJoypad(&player1);

        MovementPhysics(&player1, 3);

        wait_vbl_done();
    }
}