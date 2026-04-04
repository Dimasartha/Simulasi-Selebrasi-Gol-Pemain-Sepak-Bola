#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "raylib.h"
#include "menu_animasi.h"
#include "src/algo/dda.h"

extern Sound select;

static Rectangle btnKneeSlide = { SCREEN_W/2 - 150, 250, 300, 50 };
static Rectangle btnLompat    = { SCREEN_W/2 - 150, 320, 300, 50 };
static Rectangle btnTakeL     = { SCREEN_W/2 - 150, 390, 300, 50 };

void UpdateDrawMenuAnimasi(Screen *currentScreen) {
    Vector2 mousePos = GetMousePosition();

    if (BackButtonPressed()) {
        *currentScreen = MENU_UTAMA;
        return;
    }

    bool isKneeHover = CheckCollisionPointRec(mousePos, btnKneeSlide);
    bool isLompatHover = CheckCollisionPointRec(mousePos, btnLompat);
    bool isTakeLHover = CheckCollisionPointRec(mousePos, btnTakeL);

    DrawRectangleRec(btnKneeSlide, isKneeHover ? DARKGRAY : LIGHTGRAY);
    DrawRectangleRec(btnLompat, isLompatHover ? DARKGRAY : LIGHTGRAY);
    DrawRectangleRec(btnTakeL, isTakeLHover ? DARKGRAY : LIGHTGRAY);

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (isKneeHover) {
            PlaySound(select);
            *currentScreen = SELEBRASI_KNEESLIDE;
        }
        if (isLompatHover) {
            PlaySound(select);
            *currentScreen = SELEBRASI_LOMPAT;
        }
        if (isTakeLHover) {
            PlaySound(select);
            *currentScreen = SELEBRASI_TAKEL;
        }
    }

    DrawText("PILIH SIMULASI ANIMASI", SCREEN_W/2 - MeasureText("PILIH SIMULASI ANIMASI", 30)/2, 100, 30, WHITE);
    
    DDALine(SCREEN_W/2 - 170, 160, SCREEN_W/2 + 170, 160, GREEN);
    
    DrawText("1. Selebrasi Knee Slide", btnKneeSlide.x + 20, btnKneeSlide.y + 15, 20, BLACK);
    DrawText("2. Selebrasi Lompat (Siuuu)", btnLompat.x + 20, btnLompat.y + 15, 20, BLACK);
    DrawText("3. Selebrasi Corner Flag", btnTakeL.x + 20, btnTakeL.y + 15, 20, BLACK);

    DrawBackButton();
}