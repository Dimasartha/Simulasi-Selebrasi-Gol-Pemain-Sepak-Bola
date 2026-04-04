#include "../../screen_type.h"
#include "../ui/back_button.h"
#include "../algo/bresenham.h"
#include "src/screens/about.h"
#include "raylib.h"

static void DrawAlgoBlock(int x, int y, int startCol, int endCol, int row, int scale, Color c) {
    int px1 = x + startCol * scale;
    int px2 = x + (endCol + 1) * scale;
    int py = y + row * scale;
    
    for (int i = 0; i < scale; i++) {
        BresenhamLine(px1, py + i, px2, py + i, c);
    }
}

static void DrawTrademarkAlien(int x, int y, int scale) {
    Color c = (Color){150, 50, 220, 255}; 

    // Baris 0 (Antena Atas)
    DrawAlgoBlock(x, y, 2, 2, 0, scale, c);
    DrawAlgoBlock(x, y, 8, 8, 0, scale, c);
    
    // Baris 1 (Tengah Antena)
    DrawAlgoBlock(x, y, 3, 3, 1, scale, c);
    DrawAlgoBlock(x, y, 7, 7, 1, scale, c);
    
    // Baris 2 (Atap Kepala)
    DrawAlgoBlock(x, y, 2, 8, 2, scale, c);
    
    // Baris 3 (Mata / Jeda)
    DrawAlgoBlock(x, y, 1, 2, 3, scale, c);
    DrawAlgoBlock(x, y, 4, 6, 3, scale, c);
    DrawAlgoBlock(x, y, 8, 9, 3, scale, c);
    
    // Baris 4 (Badan Tengah Penuh)
    DrawAlgoBlock(x, y, 0, 10, 4, scale, c);
    
    // Baris 5 (Lengan Atas & Bawah Mata)
    DrawAlgoBlock(x, y, 0, 0, 5, scale, c);
    DrawAlgoBlock(x, y, 2, 8, 5, scale, c);
    DrawAlgoBlock(x, y, 10, 10, 5, scale, c);
    
    // Baris 6 (Kaki Turun & Lengan Jatuh)
    DrawAlgoBlock(x, y, 0, 0, 6, scale, c);
    DrawAlgoBlock(x, y, 2, 2, 6, scale, c);
    DrawAlgoBlock(x, y, 8, 8, 6, scale, c);
    DrawAlgoBlock(x, y, 10, 10, 6, scale, c);
    
    // Baris 7 (Telapak Kaki)
    DrawAlgoBlock(x, y, 3, 4, 7, scale, c);
    DrawAlgoBlock(x, y, 6, 7, 7, scale, c);
}

void UpdateDrawAbout(Screen *currentScreen) {
    if (BackButtonPressed()) { 
        *currentScreen = MENU_UTAMA; 
        return; 
    }

    ClearBackground((Color){20, 25, 35, 255});

    DrawText("TENTANG PROYEK", 50, 55, 30, WHITE);
    DrawRectangle(50, 90, 300, 4, GREEN); 

    DrawText("DESKRIPSI", 50, 120, 20, (Color){100, 200, 255, 255});
    DrawText("Proyek ini merupakan implementasi Praktikum Grafika Komputer.", 50, 150, 18, LIGHTGRAY);
    DrawText("Berfokus pada topik 'Simulasi Realitas dalam Bidang 2D', seluruh", 50, 175, 18, LIGHTGRAY);
    DrawText("animasi dan objek direkonstruksi menggunakan algoritma", 50, 200, 18, LIGHTGRAY);
    DrawText("pembentuk garis dasar (Bresenham & DDA) secara matematis,", 50, 225, 18, LIGHTGRAY);
    DrawText("tanpa menggunakan aset gambar (image) eksternal sama sekali.", 50, 250, 18, LIGHTGRAY);

    DrawText("PANDUAN PENGGUNAAN", 50, 310, 20, (Color){100, 200, 255, 255});
    
    int stepY = 340;
    DrawText("1.", 50, stepY, 18, YELLOW);
    DrawText("Gunakan klik MOUSE untuk memilih opsi di menu utama.", 80, stepY, 18, RAYWHITE);
    
    DrawText("2.", 50, stepY += 30, 18, YELLOW);
    DrawText("Menu 'POLA 2D' untuk melihat struktur kerangka algoritma.", 80, stepY, 18, RAYWHITE);
    
    DrawText("3.", 50, stepY += 30, 18, YELLOW);
    DrawText("Menu 'ANIMASI 2D' berisi pilihan simulasi selebrasi pemain.", 80, stepY, 18, RAYWHITE);
    
    DrawText("4.", 50, stepY += 30, 18, YELLOW);
    DrawText("Tekan tombol '< BACK' di pojok kiri atas untuk kembali.", 80, stepY, 18, RAYWHITE);

    float trademarkX = SCREEN_W - 250;
    float trademarkY = SCREEN_H - 180;
    
    DrawText("Developed by:", trademarkX, trademarkY, 16, GRAY);
    DrawText("JUNED", trademarkX, trademarkY + 25, 24, WHITE);
    
    DrawTrademarkAlien(trademarkX + 90, trademarkY - 10, 6);

    DrawBackButton();
}