#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "../ui/replay_button.h"
#include "../algo/bresenham.h"
#include "../algo/dda.h"
#include "../algo/midcircle.h"
#include "src/screens/kneeslide.h"

// PENGATURAN PARTIKEL
#define MAX_PARTICLES 40
typedef struct { float x, y, vx, vy; int life; } Particle;
static Particle particles[MAX_PARTICLES];

// VARIABEL STATE ANIMASI
static float pX = 850, pY = 600;
static float speedX = -4.5f, speedY = -2.5f;
static int state = 0; 
static float animT = 0;
static float timeScale = 1.0f;
static bool modeRangka = false;

// FUNGSI BANTU (HELPER)
static void GetLimb(float sx, float sy, float len, float angleDeg, float *ox, float *oy) {
    float rad = angleDeg * PI / 180.0f;
    *ox = sx + cosf(rad) * len;
    *oy = sy + sinf(rad) * len;
}

static void SpawnGrassParticle(float x, float y) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life <= 0) {
            particles[i].x = x; particles[i].y = y;
            particles[i].vx = (float)(rand() % 30 + 10) / 10.0f; 
            particles[i].vy = (float)(rand() % 30) / 10.0f - 1.5f; 
            particles[i].life = rand() % 20 + 10;
            break;
        }
    }
}

// FUNGSI UTAMA LAYAR
void UpdateDrawKneeSlideScreen(Screen *currentScreen) {
    // INIT & NAVIGASI
    static bool inited = false;
    if (!inited) {
        pX = 850; pY = 600; speedX = -6.0f; speedY = -3.5f; 
        state = 0; animT = 0;
        for (int i=0; i<MAX_PARTICLES; i++) particles[i].life = 0;
        inited = true;
    }

    if (BackButtonPressed()) {
        *currentScreen = MENU_ANIMASI;
        inited = false; return;
    }
    
    if (ReplayButtonPressed()) {
        inited = false;
    }

    // LOGIKA UPDATE
    animT += (0.3f * timeScale); 
    pX += (speedX * timeScale);
    pY += (speedY * timeScale);

    if (state == 0) {
        if (pX < 500) state = 1;
    } else if (state == 1) {
        speedX *= 0.95f; 
        speedY *= 0.95f; 
        if (rand() % 100 < 60) SpawnGrassParticle(pX - 10, pY + 20);
        if (fabs(speedX) < 0.1f) state = 2; 
    } else if (state == 2) {
        speedX = 0;
        speedY = 0;
    }

    // SUDUT ANGGOTA TUBUH
    float tLA, tRA, cLA, cRA, aLA, aRA, torsoAngle;

    if (state == 0) {
        torsoAngle = 250; 
        float swing = sinf(animT) * 40.0f;
        tLA = 90 + swing;  tRA = 90 - swing;  
        cLA = tLA + 20;    cRA = tRA + 20;    
        aLA = 90 - swing;  aRA = 90 + swing;  
    } else {
        torsoAngle = 280; 
        tLA = 140; cLA = 0;   
        tRA = 160; cRA = 20;  
        aLA = 200; aRA = 340; 
    }

    // LOGIKA GAMBAR
    // Warna
    Color fieldDark  = (Color){34, 100, 42, 255};
    Color fieldLight = (Color){40, 130, 50, 255};
    Color skin   = (Color){255, 204, 153, 255};
    Color shirt  = (Color){20, 30, 160, 255};
    Color shorts = WHITE; 
    Color socks  = WHITE; 
    Color hair   = (Color){30, 20, 20, 255};

    // Background & Lingkungan
    ClearBackground(fieldDark);
    for (int y = 200; y < SCREEN_H; y += 30) {
        Bres_DashedLine(0, y, SCREEN_W, y, 20, 15, fieldLight);
    }
    BresenhamLine(250, 0, 0, SCREEN_H, (Color){255,255,255,150});

    // Hitung Koordinat Sendi
    float nx, ny; 
    GetLimb(pX, pY, 35, torsoAngle, &nx, &ny);

    float kxL, kyL, axL, ayL, kxR, kyR, axR, ayR; 
    float wxL, wyL, hxL, hyL, wxR, wyR, hxR, hyR; 

    GetLimb(pX, pY, 25, tLA, &kxL, &kyL); GetLimb(kxL, kyL, 25, cLA, &axL, &ayL); 
    GetLimb(pX, pY, 25, tRA, &kxR, &kyR); GetLimb(kxR, kyR, 25, cRA, &axR, &ayR); 
    GetLimb(nx, ny, 22, aLA, &wxL, &wyL); GetLimb(wxL, wyL, 20, aLA, &hxL, &hyL); 
    GetLimb(nx, ny, 22, aRA, &wxR, &wyR); GetLimb(wxR, wyR, 20, aRA, &hxR, &hyR); 
    
    if (!modeRangka) {
        // Tubuh Kiri (Belakang)
        Bres_ThickLine(nx, ny, wxL, wyL, 10, skin); Bres_ThickLine(wxL, wyL, hxL, hyL, 8, skin); 
        Bres_ThickLine(pX, pY, kxL, kyL, 16, shorts); 
        Bres_ThickLine(kxL, kyL, axL, ayL, 12, skin); 
        Bres_ThickLine(axL-(axL-kxL)*0.5f, ayL-(ayL-kyL)*0.5f, axL, ayL, 14, socks); 

        // Badan & Nomor Punggung
        Bres_ThickLine(pX, pY, nx, ny, 32, shirt);
        float midX = (pX + nx)/2.0f, midY = (pY + ny)/2.0f;
        Bres_ThickLine(midX - 5, midY - 10, midX + 5, midY - 10, 3, WHITE); 
        Bres_ThickLine(midX + 5, midY - 10, midX - 3, midY + 8, 3, WHITE);  

        // Kepala
        MidcircleFilled(nx, ny - 8, 14, skin);
        MidcircleFilled(nx + 2, ny - 10, 15, hair); 

        // Tubuh Kanan (Depan)
        Bres_ThickLine(pX, pY, kxR, kyR, 16, shorts); 
        Bres_ThickLine(kxR, kyR, axR, ayR, 12, skin); 
        Bres_ThickLine(axR-(axR-kxR)*0.5f, ayR-(ayR-kyR)*0.5f, axR, ayR, 14, socks); 
        Bres_ThickLine(nx, ny, wxR, wyR, 10, skin); Bres_ThickLine(wxR, wyR, hxR, hyR, 8, skin);
    } else {
        Color bone = LIGHTGRAY;
        BresenhamLine(nx, ny, wxL, wyL, bone);  BresenhamLine(wxL, wyL, hxL, hyL, bone);
        BresenhamLine(pX, pY, kxL, kyL, bone);  BresenhamLine(kxL, kyL, axL, ayL, bone);
        BresenhamLine(pX, pY, nx, ny, bone);
        BresenhamLine(pX, pY, kxR, kyR, bone);  BresenhamLine(kxR, kyR, axR, ayR, bone);
        BresenhamLine(nx, ny, wxR, wyR, bone);  BresenhamLine(wxR, wyR, hxR, hyR, bone);
        
        MidcircleFilled(pX, pY, 5, YELLOW);
        MidcircleFilled(nx, ny, 4, RED);
        MidcircleFilled(kxL, kyL, 4, RED);  MidcircleFilled(axL, ayL, 4, RED);
        MidcircleFilled(kxR, kyR, 4, RED);  MidcircleFilled(axR, ayR, 4, RED);
    }

    // Partikel (Efek Rumput)
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].vy += 0.15f; 
            particles[i].life--;
            BresenhamLine(particles[i].x, particles[i].y, 
                          particles[i].x - particles[i].vx * 1.5f, 
                          particles[i].y - particles[i].vy * 1.5f, 
                          (Color){100, 200, 80, 200});
        }
    }

    // UI Layer
    DrawRectangle(0, 0, SCREEN_W, 70, (Color){0, 0, 0, 150});
    DrawText("ANIMASI: DAVID VILLA (Knee Slide)", 300, 15, 20, WHITE);
    
    if (state == 0) DrawText("Fase: Berlari...", 300, 45, 18, LIGHTGRAY);
    else if (state == 1) DrawText("Fase: KNEE SLIDE AKTIF!", 300, 45, 18, ORANGE);
    else DrawText("Fase: SELESAI. (Berhenti)", 300, 45, 18, GREEN);
    
    // Pengatur Kecepatan
    Rectangle btn05 = { 20, 100, 50, 30};
    Rectangle btn10 = { 80, 100, 50, 30};
    Rectangle btn20 = { 140, 100, 50, 30};
    Vector2 mouse = GetMousePosition();
    
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btn05)) timeScale = 0.5f;
        if (CheckCollisionPointRec(mouse, btn10)) timeScale = 1.0f;
        if (CheckCollisionPointRec(mouse, btn20)) timeScale = 2.0f;
    }
    
    DrawRectangleRec(btn05, timeScale == 0.5f ? GREEN : DARKGRAY);
    DrawRectangleRec(btn10, timeScale == 1.0f ? GREEN : DARKGRAY);
    DrawRectangleRec(btn20, timeScale == 2.0f ? GREEN : DARKGRAY);
    
    DrawText("0.5x", btn05.x + 10, btn05.y + 8, 16, WHITE);
    DrawText("1.0x", btn10.x + 10, btn10.y + 8, 16, WHITE);
    DrawText("2.0x", btn20.x + 10, btn20.y + 8, 16, WHITE);
    
    Rectangle btnRangka = { 20, 140, 170, 30 };
    if (CheckCollisionPointRec(mouse, btnRangka) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        modeRangka = !modeRangka;
    }
    
    DrawRectangleRec(btnRangka, modeRangka ? RED : DARKGRAY);
    DrawText("Mode Rangka", btnRangka.x + 10, btnRangka.y + 8, 14, WHITE);
    
    Rectangle btnTest = { 200, 140, 1000, 30 };
    DrawRectangleRec(btnTest, LIGHTGRAY);
    
    Rectangle polban1 = { 450, 80, 100, 50};
    DrawRectangleRec(polban1, WHITE);
    DDA_ThickLine(polban1.x + 50, polban1.y+5, polban1.x + 10, polban1.y + 20, 5, ORANGE);
    DDA_ThickLine(polban1.x + 65, polban1.y+10, polban1.x + 10, polban1.y + 25, 5, ORANGE);
    DDA_ThickLine(polban1.x + 80, polban1.y+22, polban1.x + 10, polban1.y + 30, 5, ORANGE);
    DDA_ThickLine(polban1.x + 10, polban1.y+30, polban1.x + 50, polban1.y + 40, 5, BLUE);
    DDA_ThickLine(polban1.x + 50, polban1.y+40, polban1.x + 80, polban1.y + 30, 5, BLUE);
    DDA_ThickLine(polban1.x + 80, polban1.y+30, polban1.x + 80, polban1.y + 25, 5, BLUE);
    
    Rectangle polban2 = { 550, 80, 100, 50};
    DrawRectangleRec(polban2, WHITE);
    DDA_ThickLine(polban2.x + 50, polban2.y+5, polban2.x + 10, polban2.y + 20, 5, ORANGE);
    DDA_ThickLine(polban2.x + 65, polban2.y+10, polban2.x + 10, polban2.y + 25, 5, ORANGE);
    DDA_ThickLine(polban2.x + 80, polban2.y+22, polban2.x + 10, polban2.y + 30, 5, ORANGE);
    DDA_ThickLine(polban2.x + 10, polban2.y+30, polban2.x + 50, polban2.y + 40, 5, BLUE);
    DDA_ThickLine(polban2.x + 50, polban2.y+40, polban2.x + 80, polban2.y + 30, 5, BLUE);
    DDA_ThickLine(polban2.x + 80, polban2.y+30, polban2.x + 80, polban2.y + 25, 5, BLUE);
    
    Rectangle polban3 = { 650, 80, 100, 50};
    DrawRectangleRec(polban3, WHITE);
    DDA_ThickLine(polban3.x + 50, polban3.y+5, polban3.x + 10, polban3.y + 20, 5, ORANGE);
    DDA_ThickLine(polban3.x + 65, polban3.y+10, polban3.x + 10, polban3.y + 25, 5, ORANGE);
    DDA_ThickLine(polban3.x + 80, polban3.y+22, polban3.x + 10, polban3.y + 30, 5, ORANGE);
    DDA_ThickLine(polban3.x + 10, polban3.y+30, polban3.x + 50, polban3.y + 40, 5, BLUE);
    DDA_ThickLine(polban3.x + 50, polban3.y+40, polban3.x + 80, polban3.y + 30, 5, BLUE);
    DDA_ThickLine(polban3.x + 80, polban3.y+30, polban3.x + 80, polban3.y + 25, 5, BLUE);
    
    DrawBackButton();
    DrawReplayButton();
}