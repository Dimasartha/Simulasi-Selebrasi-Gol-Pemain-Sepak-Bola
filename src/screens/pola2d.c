#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "../algo/bresenham.h"
#include "../algo/dda.h"
#include "../algo/midcircle.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

static void GetLimb(float sx, float sy, float len, float angleDeg, float *ox, float *oy) {
    float rad = angleDeg * PI / 180.0f;
    *ox = sx + cosf(rad) * len;
    *oy = sy + sinf(rad) * len;
}

void UpdateDrawPola2D(Screen *currentScreen) {
    if (BackButtonPressed()) { 
        *currentScreen = MENU_UTAMA; 
        return; 
    }

    ClearBackground((Color){20, 25, 35, 255});

    // GAMBAR GRID KARTESIAN
    Color gridColor = (Color){50, 70, 90, 100};
    Color axisColor = (Color){100, 150, 200, 150};

    // Garis vertikal & horizontal
    for (int i = 0; i < SCREEN_W; i += 40) BresenhamLine(i, 0, i, SCREEN_H, gridColor);
    for (int i = 0; i < SCREEN_H; i += 40) BresenhamLine(0, i, SCREEN_W, i, gridColor);
    
    // Garis Sumbu Utama (X = SCREEN_W/2, Y = SCREEN_H/2)
    BresenhamLine(SCREEN_W/2, 0, SCREEN_W/2, SCREEN_H, axisColor); // Sumbu Y
    BresenhamLine(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, axisColor); // Sumbu X

    // KOORDINAT ANATOMI (POSE BERDIRI TEGAP)    
    float pX = SCREEN_W/2; // Titik pusat di tengah layar
    float pY = SCREEN_H/2; 
    
    float nx, ny; 
    GetLimb(pX, pY, 70, 270, &nx, &ny); // Leher (lurus ke atas)

    float kxL, kyL, axL, ayL, kxR, kyR, axR, ayR; 
    float wxL, wyL, hxL, hyL, wxR, wyR, hxR, hyR; 

    // Kaki Lurus ke bawah (Sudut 80 dan 100 agar mengangkang sedikit)
    GetLimb(pX, pY, 50, 100, &kxL, &kyL); GetLimb(kxL, kyL, 50, 90, &axL, &ayL); 
    GetLimb(pX, pY, 50, 80, &kxR, &kyR);  GetLimb(kxR, kyR, 50, 90, &axR, &ayR); 
    
    // Tangan merentang ke bawah (Pose rileks)
    GetLimb(nx, ny, 45, 135, &wxL, &wyL); GetLimb(wxL, wyL, 40, 110, &hxL, &hyL); 
    GetLimb(nx, ny, 45, 45, &wxR, &wyR);  GetLimb(wxR, wyR, 40, 70, &hxR, &hyR); 

    //GAMBAR KERANGKA
    Color boneColor = (Color){255, 255, 255, 150};

    // Gambar Tulang menggunakan garis tebal tipis
    Bres_ThickLine(pX, pY, nx, ny, 8, boneColor); // Tulang Punggung
    
    Bres_ThickLine(pX, pY, kxL, kyL, 6, boneColor); Bres_ThickLine(kxL, kyL, axL, ayL, 4, boneColor); // Kaki Kiri
    Bres_ThickLine(pX, pY, kxR, kyR, 6, boneColor); Bres_ThickLine(kxR, kyR, axR, ayR, 4, boneColor); // Kaki Kanan
    
    Bres_ThickLine(nx, ny, wxL, wyL, 5, boneColor); Bres_ThickLine(wxL, wyL, hxL, hyL, 3, boneColor); // Tangan Kiri
    Bres_ThickLine(nx, ny, wxR, wyR, 5, boneColor); Bres_ThickLine(wxR, wyR, hxR, hyR, 3, boneColor); // Tangan Kanan

    // Lingkaran Kepala
    MidcircleDashed(nx, ny - 30, 30, 10, 5, YELLOW);
    
    // GAMBAR TITIK SENDI (NODE/VERTEX)    
    Color jointColor = RED;
    int jSize = 5;
    
    MidcircleFilled(pX, pY, jSize+2, YELLOW); // Panggul (Titik Pusat Gravitasi)
    MidcircleFilled(nx, ny, jSize, jointColor); // Leher
    
    MidcircleFilled(kxL, kyL, jSize, jointColor); MidcircleFilled(axL, ayL, jSize, jointColor); // Sendi Kaki Kiri
    MidcircleFilled(kxR, kyR, jSize, jointColor); MidcircleFilled(axR, ayR, jSize, jointColor); // Sendi Kaki Kanan
    
    MidcircleFilled(wxL, wyL, jSize, jointColor); MidcircleFilled(hxL, hyL, jSize, jointColor); // Sendi Tangan Kiri
    MidcircleFilled(wxR, wyR, jSize, jointColor); MidcircleFilled(hxR, hyR, jSize, jointColor); // Sendi Tangan Kanan

    // TEKS
    DrawRectangle(20, 70, 380, 240, (Color){0, 0, 0, 200});
    DrawText("REKONSTRUKSI OBJEK 2D", 35, 85, 18, YELLOW);
    
    DrawText("- Objek manusia diabstraksikan menjadi", 35, 120, 16, WHITE);
    DrawText("  kumpulan garis tebal (Bones).", 35, 140, 16, WHITE);
    
    DrawText("- Persendian ditandai dengan titik Merah", 35, 170, 16, WHITE);
    DrawText("  menggunakan MidcircleFilled.", 35, 190, 16, WHITE);
    
    DrawText("- Pusat panggul (Kuning) menjadi Root", 35, 220, 16, WHITE);
    DrawText("  Coordinate untuk Forward Kinematics.", 35, 240, 16, WHITE);

    DrawText("- Penggambaran murni menggunakan Bresenham.", 35, 270, 16, GREEN);

    // Menampilkan koordinat dari Panggul
    char coordText[50];
    sprintf(coordText, "Root (Panggul) = X: %.0f, Y: %.0f", pX, pY);
    DrawText(coordText, pX + 20, pY - 10, 16, YELLOW);

    DrawText("POLA 2D MODELING", 200, 15, 20, WHITE);
    DrawBackButton();
}