#include "raylib.h"
#include "raymath.h"
#include "arena.h"
#include <stdio.h>
#include <math.h>

#define ARENA_CAPACITY (1024 * 1024) // 1MB
#define MAX_TARGETS 5
#define MATRIX_ROWS 6
#define MATRIX_COLS 16

typedef struct {
    Vector2 pos;
    bool locked;
    float pulse;
} Target;

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 750;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "LANCELLOT AXE // WEAPON SYSTEM ENGINE - COMBAT HUD");
    SetTargetFPS(60);

    RenderTexture2D target_tex = LoadRenderTexture(screenWidth, screenHeight);

    uint8_t *raw_memory = MemAlloc(ARENA_CAPACITY);
    Arena arena;
    arena_init(&arena, raw_memory, ARENA_CAPACITY);

    // Cores de Estágio Tático
    Color COLOR_CYAN    = (Color){  0, 240, 255, 255 };
    Color COLOR_AMBER   = (Color){ 255, 170,   0, 255 };
    Color COLOR_RED     = (Color){ 255,  30,  70, 255 };
    Color COLOR_GREEN   = (Color){  50, 255, 120, 255 };
    Color COLOR_BG      = (Color){   8,  10,  15, 255 };

    float radar_angle = 0.0f;
    int cartridges = 6;
    float weapon_heat = 22.0f;
    bool overdrive_mode = false;

    // Alvos simulados para o Radar de Ataque
    Target targets[MAX_TARGETS] = {
        { { 880, 220 }, false, 0.0f },
        { { 950, 320 }, false, 0.0f },
        { { 810, 380 }, false, 0.0f },
        { { 1020, 250 }, false, 0.0f },
        { { 910, 440 }, false, 0.0f }
    };

    while (!WindowShouldClose()) {
        // --- Atualizações do Sistema de Armamento ---
        radar_angle += (overdrive_mode ? 4.5f : 2.0f);
        if (radar_angle >= 360.0f) radar_angle = 0.0f;

        // Atualização Térmica do Cano/Módulo
        if (weapon_heat > 22.0f) weapon_heat -= 0.05f;
        if (weapon_heat > 70.0f) overdrive_mode = true;
        if (weapon_heat < 40.0f && overdrive_mode) overdrive_mode = false;

        // Animação de trava nos alvos no radar
        for (int i = 0; i < MAX_TARGETS; i++) {
            targets[i].pulse += 0.05f;
            float angle_to_target = atan2f(targets[i].pos.y - 320, targets[i].pos.x - 900) * RAD2DEG;
            if (angle_to_target < 0) angle_to_target += 360.0f;
            if (fabsf(radar_angle - angle_to_target) < 15.0f) {
                targets[i].locked = true;
            }
        }

        // --- Controles de Disparo do Armamento ---
        // ESPAÇO: Disparo do Cartucho de Mana/Memória
        if (IsKeyPressed(KEY_SPACE)) {
            arena_alloc(&arena, 128 * 1024); // Aloca bloco na Arena
            if (cartridges > 0) cartridges--;
            weapon_heat += 12.5f;
        }

        // 'R': Recarga do Sistema / Purga de Memória
        if (IsKeyPressed(KEY_R)) {
            arena_reset(&arena);
            cartridges = 6;
            weapon_heat = 22.0f;
            for (int i = 0; i < MAX_TARGETS; i++) targets[i].locked = false;
        }

        Color main_theme = overdrive_mode ? COLOR_RED : (weapon_heat > 50.0f ? COLOR_AMBER : COLOR_CYAN);

        // --- RENDERIZAÇÃO NA TEXTURA ---
        BeginTextureMode(target_tex);
            ClearBackground(COLOR_BG);

            // 1. MOLDURA PERIMÉTRICA TÁTICA (HUD Bounding Box)
            DrawRectangleLinesEx((Rectangle){ 15, 15, screenWidth - 30, screenHeight - 30 }, 2, main_theme);
            DrawText("LANCELOT AXE // WEAPON FIRE-CONTROL INTERFACE", 30, 25, 14, main_theme);
            DrawText(overdrive_mode ? "STATE: [OVERHEAT / COMBAT ENGAGED]" : "STATE: [STANDBY / TARGET LOCKING]", 780, 25, 13, main_theme);

            // 2. PAINEL ESQUERDO - DIAGNÓSTICO DO NÚCLEO E ARENA
            DrawRectangle(30, 60, 480, 660, (Color){ 12, 16, 24, 230 });
            DrawRectangleLinesEx((Rectangle){ 30, 60, 480, 660 }, 1, main_theme);

            DrawText("MAGIC CORE TELEMETRY", 50, 80, 16, COLOR_AMBER);
            DrawLine(50, 102, 490, 102, main_theme);

            // Medidor de Uso da Arena
            size_t used = arena.offset;
            size_t total = arena.capacity;
            float usage_pct = (float)used / (float)total;

            DrawText("ARENA ALLOCATION BUFFER", 50, 120, 12, WHITE);
            DrawRectangle(50, 140, 440, 26, (Color){ 10, 20, 30, 255 });
            DrawRectangle(50, 140, (int)(440 * usage_pct), 26, main_theme);
            DrawRectangleLines(50, 140, 440, 26, COLOR_AMBER);

            char buf[64];
            snprintf(buf, sizeof(buf), "%zu / %zu BYTES (%.1f%%)", used, total, usage_pct * 100.0f);
            DrawText(buf, 55, 146, 12, BLACK);

            // MAPA DE MATRIZ DE MEMÓRIA EM TEMPO REAL (Memory Hex Grid)
            DrawText("ARENA MEMORY BLOCKS MAP", 50, 185, 12, WHITE);
            int block_size = 25;
            int total_blocks = MATRIX_ROWS * MATRIX_COLS;
            int allocated_blocks = (int)(total_blocks * usage_pct);

            for (int r = 0; r < MATRIX_ROWS; r++) {
                for (int c = 0; c < MATRIX_COLS; c++) {
                    int idx = r * MATRIX_COLS + c;
                    Color block_color = (idx < allocated_blocks) ? main_theme : (Color){ 20, 30, 42, 255 };
                    DrawRectangle(50 + (c * 27), 205 + (r * 27), block_size, block_size, block_color);
                    DrawRectangleLines(50 + (c * 27), 205 + (r * 27), block_size, block_size, (Color){ 0, 240, 255, 40 });
                }
            }

            // SISTEMA DE CARTUCHOS (CHAMBER SYSTEM)
            DrawText("CV-CARTRIDGE CHAMBER (CARTRIDGE SYSTEM)", 50, 380, 12, WHITE);
            for (int i = 0; i < 6; i++) {
                Color c = (i < cartridges) ? COLOR_AMBER : (Color){ 30, 35, 45, 255 };
                DrawRectangle(50 + (i * 73), 400, 65, 30, c);
                DrawRectangleLines(50 + (i * 73), 400, 65, 30, main_theme);
                if (i < cartridges) {
                    DrawText("LOADED", 58 + (i * 73), 410, 10, BLACK);
                } else {
                    DrawText("EMPTY", 60 + (i * 73), 410, 10, GRAY);
                }
            }

            // BARRA DE TEMPERATURA E ESTRESSE
            snprintf(buf, sizeof(buf), "BARREL TEMPERATURE: %.1f C", weapon_heat);
            DrawText(buf, 50, 450, 13, (weapon_heat > 60.0f) ? COLOR_RED : COLOR_GREEN);
            DrawRectangle(50, 470, 440, 15, (Color){ 10, 20, 30, 255 });
            DrawRectangle(50, 470, (int)(440 * (weapon_heat / 100.0f)), 15, (weapon_heat > 60.0f) ? COLOR_RED : COLOR_GREEN);
            DrawRectangleLines(50, 470, 440, 15, WHITE);

            // COMANDOS DE DISPARO DA ARMA
            DrawRectangle(50, 510, 440, 190, (Color){ 5, 10, 18, 255 });
            DrawRectangleLines(50, 510, 440, 190, main_theme);
            DrawText("[OPERATIONAL CONTROLS]", 65, 525, 12, COLOR_AMBER);
            DrawText("-> PRESS [SPACE] : ENGAGE / FIRE CV CARTRIDGE", 65, 555, 12, WHITE);
            DrawText("-> PRESS [R]     : RELOAD CHAMBER & RESET ARENA", 65, 585, 12, WHITE);
            DrawText("-> MOUSE CLICK   : LOCK TARGET ON TACTICAL RADAR", 65, 615, 12, WHITE);

            // 3. PAINEL DIREITO - RADAR DE COMBATE E TÁTICA DE MIRA
            Vector2 radar_center = { 900, 320 };
            float radar_radius = 210.0f;

            DrawCircle(radar_center.x, radar_center.y, radar_radius, (Color){ 10, 15, 22, 255 });
            DrawCircleLines(radar_center.x, radar_center.y, radar_radius, main_theme);
            DrawCircleLines(radar_center.x, radar_center.y, radar_radius * 0.66f, (Color){ 0, 240, 255, 40 });
            DrawCircleLines(radar_center.x, radar_center.y, radar_radius * 0.33f, (Color){ 0, 240, 255, 40 });

            // Eixo do Radar
            DrawLine(radar_center.x - radar_radius, radar_center.y, radar_center.x + radar_radius, radar_center.y, (Color){ 0, 240, 255, 40 });
            DrawLine(radar_center.x, radar_center.y - radar_radius, radar_center.x, radar_center.y + radar_radius, (Color){ 0, 240, 255, 40 });

            // Vetor de Varredura (Sweep Line)
            Vector2 sweep_end = {
                radar_center.x + cosf(radar_angle * DEG2RAD) * radar_radius,
                radar_center.y + sinf(radar_angle * DEG2RAD) * radar_radius
            };
            DrawLineEx(radar_center, sweep_end, 2.0f, main_theme);

            // Desenho dos Alvos Encontrados
            for (int i = 0; i < MAX_TARGETS; i++) {
                Color target_col = targets[i].locked ? COLOR_RED : COLOR_GREEN;
                DrawCircleV(targets[i].pos, 5, target_col);

                if (targets[i].locked) {
                    float pulse_size = 12.0f + sinf(targets[i].pulse * 4.0f) * 3.0f;
                    DrawRectangleLines(targets[i].pos.x - pulse_size/2, targets[i].pos.y - pulse_size/2, pulse_size, pulse_size, COLOR_RED);
                    DrawText("LOCK", targets[i].pos.x + 10, targets[i].pos.y - 5, 10, COLOR_RED);
                }
            }

            // Título do Radar
            DrawText("TACTICAL COMBAT RADAR", 780, 80, 16, COLOR_AMBER);
            DrawText("SYSTEM: AUTONOMOUS TARGET SCANNER", 780, 100, 11, main_theme);

        EndTextureMode();

        // --- RENDERIZAÇÃO FINAL COM SCANLINES CRT DE COMBATE ---
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(target_tex.texture, (Rectangle){ 0, 0, (float)target_tex.texture.width, (float)-target_tex.texture.height }, (Vector2){ 0, 0 }, WHITE);

            // Efeito Additive Bloom
            BeginBlendMode(BLEND_ADDITIVE);
                DrawTextureRec(target_tex.texture, (Rectangle){ 0, 0, (float)target_tex.texture.width, (float)-target_tex.texture.height }, (Vector2){ 0, 0 }, ColorAlpha(WHITE, 0.12f));
            EndBlendMode();

            // Scanlines Táticas estilo visor de combate
            for (int y = 0; y < screenHeight; y += 3) {
                DrawLine(0, y, screenWidth, y, (Color){ 0, 0, 0, 50 });
            }
        EndDrawing();
    }

    UnloadRenderTexture(target_tex);
    MemFree(raw_memory);
    CloseWindow();
    return 0;
}
