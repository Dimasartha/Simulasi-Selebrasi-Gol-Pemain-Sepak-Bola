#include "raylib.h"
#include "screen_type.h"
#include "src/ui/back_button.h"
#include "src/screens/menu.h"
#include "src/screens/menu_animasi.h"
#include "src/screens/kneeslide.h"
#include "src/screens/lompat.h"
#include "src/screens/takel.h"
#include "src/screens/pola2d.h"
#include "src/screens/about.h"

Music bgm;
Sound select;
Sound fxSiuu;

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Simulasi Animasi Selebrasi Gol - Raylib");
    InitAudioDevice();

    bgm = LoadMusicStream("bgm.mp3");
    select = LoadSound("select.wav");
    fxSiuu = LoadSound("fxsiuu.wav");

    PlayMusicStream(bgm);
    SetMusicVolume(bgm, 0.5f);
    
    SetTargetFPS(60);
    Screen currentScreen = MENU_UTAMA;

    while (!WindowShouldClose() && currentScreen != QUIT_APP) {
        UpdateMusicStream(bgm);

        BeginDrawing();
        ClearBackground((Color){15, 20, 30, 255});

        switch(currentScreen) {
            case MENU_UTAMA: 
                UpdateDrawMenuUtama(&currentScreen); 
                break;
            case MENU_ANIMASI: 
                UpdateDrawMenuAnimasi(&currentScreen); 
                break;
            case POLA_2D:
                UpdateDrawPola2D(&currentScreen);
                break;
            case ABOUT:
                UpdateDrawAbout(&currentScreen);
                break;
            case SELEBRASI_KNEESLIDE:
                UpdateDrawKneeSlideScreen(&currentScreen);
                break;
            case SELEBRASI_LOMPAT:
                UpdateDrawLompatScreen(&currentScreen);
                break;
            case SELEBRASI_TAKEL:
                UpdateDrawTakeLScreen(&currentScreen);
                break;
            default: break;
        }

        EndDrawing();
    }
    
    UnloadMusicStream(bgm);
    UnloadSound(select);
    UnloadSound(fxSiuu);
    CloseAudioDevice();
    
    CloseWindow();
    return 0;
}