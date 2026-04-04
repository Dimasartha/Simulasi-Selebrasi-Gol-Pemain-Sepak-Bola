#include "src/screens/menu.h"
#include "src/algo/dda.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"

extern Sound select;
static Rectangle btnPola   = { SCREEN_W/2 - 150, 250, 300, 50 };
static Rectangle btnAnim   = { SCREEN_W/2 - 150, 320, 300, 50 };
static Rectangle btnAbout  = { SCREEN_W/2 - 150, 390, 300, 50 };
static Rectangle btnQuit   = { SCREEN_W/2 - 150, 460, 300, 50 };

void UpdateDrawMenuUtama(Screen *currentScreen) {
    Vector2 mousePos = GetMousePosition();

    // Fungsi bantu untuk menggambar dan mengecek tombol
    bool isPolaHover = CheckCollisionPointRec(mousePos, btnPola);
    bool isAnimHover = CheckCollisionPointRec(mousePos, btnAnim);
    bool isAboutHover = CheckCollisionPointRec(mousePos, btnAbout);
    bool isQuitHover = CheckCollisionPointRec(mousePos, btnQuit);

    DrawRectangleRec(btnPola, isPolaHover ? DARKGRAY : LIGHTGRAY);
    DrawRectangleRec(btnAnim, isAnimHover ? DARKGRAY : LIGHTGRAY);
    DrawRectangleRec(btnAbout, isAboutHover ? DARKGRAY : LIGHTGRAY);
    DrawRectangleRec(btnQuit, isQuitHover ? (Color){200, 50, 50, 255} : (Color){255, 100, 100, 255});

    // Logika Klik
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (isPolaHover) { PlaySound(select); *currentScreen = POLA_2D; }
        if (isAnimHover) { PlaySound(select); *currentScreen = MENU_ANIMASI; }
        if (isAboutHover) { PlaySound(select); *currentScreen = ABOUT; }
        if (isQuitHover) { *currentScreen = QUIT_APP; }
    }

    // Teks Judul
    DrawText("SIMULASI REALITAS DALAM BIDANG 2D", SCREEN_W/2 - MeasureText("SIMULASI REALITAS DALAM BIDANG 2D", 30)/2, 80, 30, WHITE);
    DrawText("Topik: Selebrasi Gol Pemain Sepakbola", SCREEN_W/2 - MeasureText("Topik: Selebrasi Gol Pemain Sepakbola", 20)/2, 120, 20, LIGHTGRAY);
    
    // Garis hiasan DDA
    DDALine(SCREEN_W/2 - 250, 160, SCREEN_W/2 + 250, 160, GREEN);

    // Teks Tombol
    DrawText("1. POLA 2D (Modeling)", btnPola.x + 20, btnPola.y + 15, 20, BLACK);
    DrawText("2. ANIMASI 2D (Simulasi)", btnAnim.x + 20, btnAnim.y + 15, 20, BLACK);
    DrawText("3. ABOUT", btnAbout.x + 20, btnAbout.y + 15, 20, BLACK);
    DrawText("4. QUIT", btnQuit.x + 20, btnQuit.y + 15, 20, WHITE);
}