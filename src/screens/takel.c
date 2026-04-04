#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "../ui/replay_button.h"
#include "../algo/bresenham.h"
#include "../algo/dda.h"
#include "../algo/midcircle.h"
#include "src/screens/takel.h"

// PENGATURAN PARTIKEL (Bendera Pelangi)
#define MAX_PARTICLES 50 
typedef struct { float x, y, vx, vy; Color c; int life; } Particle;
static Particle particles[MAX_PARTICLES];

// VARIABEL STATE ANIMASI
static float pX = 100, pY = 550; 
static float speedX = 6.0f;
static int state = 0; 
static float animT = 0;
static bool flagBroken = false; 

// Titik target tiang bendera corner
#define FLAG_X 850
#define FLAG_Y_BASE 570

// FUNGSI BANTU (HELPER)
static void GetLimb(float sx, float sy, float len, float angleDeg, float *ox, float *oy) {
    float rad = angleDeg * PI / 180.0f;
    *ox = sx + cosf(rad) * len;
    *oy = sy + sinf(rad) * len;
}

static void SpawnFlagParticles(float x, float y) {
    Color rainbow[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].x = x; 
        particles[i].y = y;
        particles[i].vx = (float)(rand() % 70 + 30) / 10.0f; 
        particles[i].vy = (float)-(rand() % 60 + 20) / 10.0f; 
        particles[i].c = rainbow[rand() % 6]; 
        particles[i].life = rand() % 60 + 40; 
    }
}

// FUNGSI UTAMA LAYAR
void UpdateDrawTakeLScreen(Screen *currentScreen) {
    // INIT & NAVIGASI
    static bool inited = false;
    if (!inited) {
        pX = 150; pY = 550; speedX = 6.5f; 
        state = 0; animT = 0; flagBroken = false;
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
    animT += 0.3f;
    pX += speedX;

    if (state == 0) {
        if (pX >= 720) {
            state = 1; 
        }
    } 
    else if (state == 1) {
        speedX *= 0.98f; 
        if (pY < 565) pY += 3.0f; 

        if (pX >= FLAG_X - 25 && !flagBroken) {
            state = 2; 
            flagBroken = true;
            SpawnFlagParticles(FLAG_X, FLAG_Y_BASE - 50); 
        }
    }
    else if (state == 2) {
        speedX *= 0.85f; 
        if (fabs(speedX) < 0.2f) {
            speedX = 0;
            state = 3; 
        }
    }

    // SUDUT ANGGOTA TUBUH
    float tLA, tRA, cLA, cRA, aLA, aRA, torsoAngle;

    if (state == 0) {
        torsoAngle = 250; 
        float swing = sinf(animT * 1.5f) * 45.0f;
        tLA = 90 - swing;  tRA = 90 + swing;  
        cLA = tLA + 20;    cRA = tRA + 20;    
        aLA = 90 + swing;  aRA = 90 - swing;  
    } 
    else if (state == 1 || state == 2) {
        torsoAngle = 230; 
        tLA = 350; cLA = 350;   
        tRA = 130; cRA = 20;  
        aLA = 180; aRA = 200; 
    }
    else {
        torsoAngle = 240; 
        tLA = 350; cLA = 350;   
        tRA = 140; cRA = 20;  
        aLA = 150; aRA = 150; 
    }

    // LOGIKA GAMBAR
    // Warna
    Color fieldDark  = (Color){34, 100, 42, 255};
    Color fieldLight = (Color){40, 130, 50, 255};
    Color skin   = (Color){255, 204, 153, 255};
    Color shirtBlue = (Color){0, 83, 160, 255};  
    Color shortsWhite = WHITE;
    Color socksWhite  = WHITE;
    Color hair   = (Color){110, 80, 50, 255}; 

    // Background & Lingkungan
    ClearBackground(fieldDark);
    for (int y = 200; y < SCREEN_H; y += 30) {
        Bres_DashedLine(0, y, SCREEN_W, y, 20, 15, fieldLight);
    }
    BresenhamLine(0, FLAG_Y_BASE, SCREEN_W, FLAG_Y_BASE, (Color){255,255,255,100}); 

    if (!flagBroken) {
        Bres_ThickLine(FLAG_X, FLAG_Y_BASE, FLAG_X, FLAG_Y_BASE - 100, 6, (Color){220, 220, 220, 255});
        int bh = FLAG_Y_BASE - 100; 
        int cy = 8; 
        DDA_ThickLine(FLAG_X + 3, bh, FLAG_X + 45, bh, cy, RED);
        DDA_ThickLine(FLAG_X + 3, bh + cy, FLAG_X + 45, bh + cy, cy, ORANGE);
        DDA_ThickLine(FLAG_X + 3, bh + cy*2, FLAG_X + 45, bh + cy*2, cy, YELLOW);
        DDA_ThickLine(FLAG_X + 3, bh + cy*3, FLAG_X + 45, bh + cy*3, cy, GREEN);
        DDA_ThickLine(FLAG_X + 3, bh + cy*4, FLAG_X + 45, bh + cy*4, cy, BLUE);
        DDA_ThickLine(FLAG_X + 3, bh + cy*5, FLAG_X + 45, bh + cy*5, cy, PURPLE);
    } else {
        Bres_ThickLine(FLAG_X, FLAG_Y_BASE, FLAG_X + 60, FLAG_Y_BASE - 10, 6, (Color){220, 220, 220, 255});
    }

    // Hitung Koordinat Sendi
    float nx, ny; 
    GetLimb(pX, pY, 35, torsoAngle, &nx, &ny);

    float kxL, kyL, axL, ayL, kxR, kyR, axR, ayR; 
    float wxL, wyL, hxL, hyL, wxR, wyR, hxR, hyR; 

    GetLimb(pX, pY, 25, tLA, &kxL, &kyL); GetLimb(kxL, kyL, 25, cLA, &axL, &ayL); 
    GetLimb(pX, pY, 25, tRA, &kxR, &kyR); GetLimb(kxR, kyR, 25, cRA, &axR, &ayR); 
    GetLimb(nx, ny, 22, aLA, &wxL, &wyL); GetLimb(wxL, wyL, 20, aLA, &hxL, &hyL); 
    GetLimb(nx, ny, 22, aRA, &wxR, &wyR); GetLimb(wxR, wyR, 20, aRA, &hxR, &hyR); 

    // Tubuh Kiri (Belakang)
    Bres_ThickLine(nx, ny, wxL, wyL, 10, skin); Bres_ThickLine(wxL, wyL, hxL, hyL, 8, skin); 
    Bres_ThickLine(pX, pY, kxL, kyL, 16, shortsWhite); 
    Bres_ThickLine(kxL, kyL, axL, ayL, 12, skin); 
    Bres_ThickLine(axL-(axL-kxL)*0.5f, ayL-(ayL-kyL)*0.5f, axL, ayL, 14, socksWhite); 

    // Badan & Kepala
    Bres_ThickLine(pX, pY, nx, ny, 32, shirtBlue);
    Bres_ThickLine(nx, ny, nx+5, ny-5, 3, WHITE); 
    MidcircleFilled(nx, ny - 10, 14, skin);
    MidcircleFilled(nx + 1, ny - 12, 15, hair); 

    // Tubuh Kanan (Depan)
    Bres_ThickLine(pX, pY, kxR, kyR, 16, shortsWhite); 
    Bres_ThickLine(kxR, kyR, axR, ayR, 12, skin); 
    Bres_ThickLine(axR-(axR-kxR)*0.5f, ayR-(ayR-kyR)*0.5f, axR, ayR, 14, socksWhite); 
    Bres_ThickLine(nx, ny, wxR, wyR, 10, skin); Bres_ThickLine(wxR, wyR, hxR, hyR, 8, skin); 

    // Partikel (Efek Serpihan)
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].vy += 0.2f; 
            particles[i].life--;
            
            DDA_ThickLine(particles[i].x, particles[i].y, 
                          particles[i].x + 5, particles[i].y + 5, 
                          4, particles[i].c);
        }
    }

    // UI Layer
    DrawRectangle(0, 0, SCREEN_W, 70, (Color){0, 0, 0, 150});
    DrawText("ANIMASI: JAMIE VARDY (Corner Flag Smash)", 300, 15, 20, WHITE);
    
    if (state == 0) DrawText("Fase: Lari Menyerang...", 300, 45, 18, LIGHTGRAY);
    else if (state == 1) DrawText("Fase: SLIDING TACKLE!", 300, 45, 18, ORANGE);
    else if (state == 2) DrawText("Fase: BENTURAN!", 300, 45, 18, RED);
    else DrawText("Fase: SELESAI. (Tergeletak)", 300, 45, 18, GREEN);

    DrawBackButton();
    DrawReplayButton();
}