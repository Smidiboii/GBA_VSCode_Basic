#include "Intellisense.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define SCREEN_BUFFER ((unsigned short*)0x06000000)

#define CIRCLE_RADIUS 20

void draw_circle(int x0, int y0, int radius, unsigned short color) {
    int x, y;
    for (x = -radius; x <= radius; x++) {
        for (y = -radius; y <= radius; y++) {
            if (x * x + y * y <= radius * radius) {
                int xx = x0 + x;
                int yy = y0 + y;
                if (xx >= 0 && xx < SCREEN_WIDTH && yy >= 0 && yy < SCREEN_HEIGHT) {
                    SCREEN_BUFFER[xx + yy * SCREEN_WIDTH] = color;
                }
            }
        }
    }
}

int main() {
    *(unsigned int*)0x04000000 = 0x0403; // Set GBA rendering context to MODE 3 Bitmap Rendering

    int t = 0;
    unsigned short color = 0xF800; // Initial color (red)
    while (1) {
        draw_circle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CIRCLE_RADIUS, color);

        // Change color for the next frame
        color += 0x001F; // Increment the color (blue component)
        if (color > 0xFFFF) {
            color = 0xF800; // Reset to red if overflow
        }

        // Wait for VBlank
        while (*(volatile unsigned short*)0x04000004 & 1); // Wait for VBlank
        while (!(*(volatile unsigned short*)0x04000004 & 1)); // Wait until VBlank ends
    }

    return 0;
}
