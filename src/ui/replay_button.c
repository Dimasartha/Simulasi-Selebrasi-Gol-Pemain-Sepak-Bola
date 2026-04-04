#include "src/ui/replay_button.h"
#include "raylib.h"

// Posisi X = 130 (Berada di sebelah kanan tombol BACK yang lebarnya 110 + jarak 20)
static Rectangle replayBtn = { 130, 12, 110, 34 };

int ReplayButtonPressed(void) {
    Vector2 mouse = GetMousePosition();
    int hover = CheckCollisionPointRec(mouse, replayBtn);
    
    // Deteksi klik mouse ATAU tombol 'R' di keyboard
    if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return 1;
    if (IsKeyPressed(KEY_R)) return 1; 
    
    return 0;
}

void DrawReplayButton(void) {
    Vector2 mouse = GetMousePosition();
    int hover = CheckCollisionPointRec(mouse, replayBtn);
    
    // Warna dasar hijau gelap, jika di-hover menjadi hijau terang
    Color bg  = hover ? (Color){60, 160, 100, 230} : (Color){30, 100, 60, 220};
    Color brd = hover ? WHITE : (Color){100, 200, 140, 255};
    
    DrawRectangleRounded(replayBtn, 0.3f, 6, bg);
    DrawRectangleRoundedLines(replayBtn, 0.3f, 6, brd);
    
    // Ikon sederhana & Teks
    DrawText("REPLAY", (int)replayBtn.x + 25, (int)replayBtn.y + 9, 16, WHITE);
}