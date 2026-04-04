#ifndef SCREEN_TYPE_H
#define SCREEN_TYPE_H

#define SCREEN_W 1000
#define SCREEN_H 700

// Layar yang sedang aktif
typedef enum { 
    MENU_UTAMA,           // Menu utama aplikasi
    MENU_ANIMASI,         // Sub-menu untuk milih selebrasi
    POLA_2D,              // Layar untuk modeling statis/perhitungan
    SELEBRASI_KNEESLIDE, 
    SELEBRASI_LOMPAT, 
    SELEBRASI_TAKEL,
    ABOUT,                // Layar tentang project
    QUIT_APP              // Flag untuk keluar aplikasi
} Screen;

#endif