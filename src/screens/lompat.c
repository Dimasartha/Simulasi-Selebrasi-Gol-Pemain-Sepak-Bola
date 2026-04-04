#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "../ui/replay_button.h"
#include "../algo/bresenham.h"
#include "../algo/dda.h"
#include "../algo/midcircle.h"
#include "src/screens/lompat.h"

extern Sound fxSiuu;

// PENGATURAN PARTIKEL
#define MAX_PARTICLES 40
typedef struct { float x, y, vx, vy; int life; } Particle;
static Particle particles[MAX_PARTICLES];

// VARIABEL STATE ANIMASI
static float pX = 150, pY = 550; 
static float speedX = 6.0f, speedY = 0.0f;
static int state = 0; 
static float animT = 0;

// FUNGSI BANTU (HELPER)
static void GetLimb(float sx, float sy, float len, float angleDeg, float *ox, float *oy) {
    float rad = angleDeg * PI / 180.0f;
    *ox = sx + cosf(rad) * len;
    *oy = sy + sinf(rad) * len;
}

static void SpawnImpactParticles(float x, float y) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].x = x; 
        particles[i].y = y;
        particles[i].vx = (float)((rand() % 60) - 30) / 10.0f; 
        particles[i].vy = (float)-(rand() % 30 + 10) / 10.0f; 
        particles[i].life = rand() % 20 + 20;
    }
}

// FUNGSI UTAMA LAYAR
void UpdateDrawLompatScreen(Screen *currentScreen) {
    // INIT & NAVIGASI
    static bool inited = false;
    if (!inited) {
        pX = 150; pY = 550; speedX = 6.5f; speedY = 0.0f; 
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
    animT += 0.3f; 
    pX += speedX;
    pY += speedY;

    if (state == 0) {
        if (pX > 450) {
            state = 1;
            speedX = 3.0f;   
            speedY = -14.0f; 
        }
    } else if (state == 1) {
        speedY += 0.6f; 
        if (speedY >= 0) state = 2; 
    } else if (state == 2) {
        speedY += 0.8f; 
        if (pY >= 550) {
            pY = 550; 
            speedY = 0; speedX = 0;
            state = 3; 
            SpawnImpactParticles(pX, pY + 40); 
            PlaySound(fxSiuu);
        }
    }

    // SUDUT ANGGOTA TUBUH
    float tLA, tRA, cLA, cRA, aLA, aRA, torsoAngle;

    if (state == 0) {
        torsoAngle = 260; 
        float swing = sinf(animT) * 45.0f;
        tLA = 90 - swing;  tRA = 90 + swing;  
        cLA = tLA + 20;    cRA = tRA + 20;    
        aLA = 90 + swing;  aRA = 90 - swing;  
    } else if (state == 1 || state == 2) {
        torsoAngle = 270; 
        tLA = 110; cLA = 80;  
        tRA = 70;  cRA = 100; 
        aLA = 45;  aRA = 135; 
    } else {
        torsoAngle = 270; 
        tLA = 125; cLA = 125;   
        tRA = 55;  cRA = 55;  
        aLA = 150; aRA = 30; 
    }

    // LOGIKA GAMBAR
    // Warna
    Color fieldDark  = (Color){34, 100, 42, 255};
    Color fieldLight = (Color){40, 130, 50, 255};
    Color skin   = (Color){220, 160, 120, 255}; 
    Color shirtW = WHITE;                       
    Color shirtB = (Color){30, 30, 30, 255};    
    Color shorts = (Color){20, 20, 20, 255};
    Color socks  = (Color){20, 20, 20, 255};
    Color hair   = (Color){20, 20, 20, 255};

    // Background & Lingkungan
    ClearBackground(fieldDark);
    for (int y = 200; y < SCREEN_H; y += 30) {
        Bres_DashedLine(0, y, SCREEN_W, y, 20, 15, fieldLight);
    }
    BresenhamLine(0, 570, SCREEN_W, 570, (Color){255,255,255,100}); 

    // Hitung Koordinat Sendi
    float nx, ny; 
    GetLimb(pX, pY, 35, torsoAngle, &nx, &ny);

    float kxL, kyL, axL, ayL, kxR, kyR, axR, ayR; 
    float wxL, wyL, hxL, hyL, wxR, wyR, hxR, hyR; 

    GetLimb(pX, pY, 25, tLA, &kxL, &kyL); GetLimb(kxL, kyL, 25, cLA, &axL, &ayL); 
    GetLimb(pX, pY, 25, tRA, &kxR, &kyR); GetLimb(kxR, kyR, 25, cRA, &axR, &ayR); 
    GetLimb(nx, ny, 22, aLA, &wxL, &wyL); GetLimb(wxL, wyL, 20, aLA, &hxL, &hyL); 
    GetLimb(nx, ny, 22, aRA, &wxR, &wyR); GetLimb(wxR, wyR, 20, aRA, &hxR, &hyR); 

    float distanceToGround = 550 - pY; 
    float shadowWidth = 35 - (distanceToGround * 0.3f); 
    if (shadowWidth < 5) shadowWidth = 5;

    Color shadowColor = (Color){20, 50, 25, 120}; // Hijau sangat gelap & transparan
    Bres_ThickLine(pX - shadowWidth, 570, pX + shadowWidth, 570, 8, shadowColor);

    // Tubuh Kiri (Belakang)
    Bres_ThickLine(nx, ny, wxL, wyL, 10, shirtB); 
    Bres_ThickLine(wxL, wyL, hxL, hyL, 8, skin); 
    Bres_ThickLine(pX, pY, kxL, kyL, 16, shorts); 
    Bres_ThickLine(kxL, kyL, axL, ayL, 12, skin); 
    Bres_ThickLine(axL-(axL-kxL)*0.5f, ayL-(ayL-kyL)*0.5f, axL, ayL, 14, socks); 

    // Badan & Nomor Punggung
    Bres_ThickLine(pX - 8, pY, nx - 8, ny, 10, shirtB); 
    Bres_ThickLine(pX, pY, nx, ny, 12, shirtW);         
    Bres_ThickLine(pX + 8, pY, nx + 8, ny, 10, shirtB); 
    float midX = (pX + nx)/2.0f, midY = (pY + ny)/2.0f;
    Bres_ThickLine(midX - 3, midY - 10, midX + 5, midY - 10, 3, BLACK); 
    Bres_ThickLine(midX + 5, midY - 10, midX - 1, midY + 8, 3, BLACK);  

    // Kepala
    MidcircleFilled(nx, ny - 10, 14, skin);
    MidcircleFilled(nx + 1, ny - 13, 15, hair); 

    // Tubuh Kanan (Depan)
    Bres_ThickLine(pX, pY, kxR, kyR, 16, shorts); 
    Bres_ThickLine(kxR, kyR, axR, ayR, 12, skin); 
    Bres_ThickLine(axR-(axR-kxR)*0.5f, ayR-(ayR-kyR)*0.5f, axR, ayR, 14, socks); 
    Bres_ThickLine(nx, ny, wxR, wyR, 10, shirtW); 
    Bres_ThickLine(wxR, wyR, hxR, hyR, 8, skin); 

    // Partikel (Efek Debu)
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            particles[i].vy += 0.2f; 
            particles[i].life--;
            BresenhamLine(particles[i].x, particles[i].y, 
                          particles[i].x - particles[i].vx * 2.0f, 
                          particles[i].y - particles[i].vy * 2.0f, 
                          (Color){200, 200, 200, 200});
        }
    }

    // UI Layer
    if (state == 3) DrawText("SIUUUUUU!", pX - 50, pY - 100, 30, YELLOW);
    
    DrawRectangle(0, 0, SCREEN_W, 70, (Color){0, 0, 0, 150});
    DrawText("ANIMASI: CRISTIANO RONALDO (Lompatan Vertikal)", 300, 15, 20, WHITE);
    if (state == 1 || state == 2) DrawText("Fase: LOMPATAN GRAVITASI", 300, 45, 18, ORANGE);
    else if (state == 3) DrawText("Fase: MENDARAT (SIUUU!)", 300, 45, 18, GREEN);

    DrawBackButton();
    DrawReplayButton();
}