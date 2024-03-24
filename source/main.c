#include "Intellisense.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define SCREEN_BUFFER ((unsigned short *)0x06000000)

#define CIRCLE_RADIUS 20

float cos(float angle)
{
    while (angle < 0)
    {
        angle += 2 * 3.14159265358979323846;
    }
    while (angle >= 2 * 3.14159265358979323846)
    {
        angle -= 2 * 3.14159265358979323846;
    }

    float result = 1.0f;
    float term = 1.0f;
    float angle_squared = angle * angle;
    int sign = -1;
    for (int i = 2; i <= 20; i += 2)
    {
        term *= angle_squared / ((float)(i * (i - 1)));
        result += sign * term;
        sign = -sign;
    }

    return result;
}

float sin(float angle)
{
    while (angle < 0)
    {
        angle += 2 * 3.14159265358979323846;
    }
    while (angle >= 2 * 3.14159265358979323846)
    {
        angle -= 2 * 3.14159265358979323846;
    }
    float result = angle;
    float term = angle;
    float angle_squared = angle * angle;
    int sign = -1;
    for (int i = 3; i <= 21; i += 2)
    {
        term *= angle_squared / ((float)((i - 1) * i));
        result += sign * term;
        sign = -sign;
    }

    return result;
}

void draw_circle(int x0, int y0, int radius, unsigned short color)
{
    int x, y;
    for (x = -radius; x <= radius; x++)
    {
        for (y = -radius; y <= radius; y++)
        {
            if (x * x + y * y <= radius * radius)
            {
                int xx = x0 + x;
                int yy = y0 + y;
                if (xx >= 0 && xx < SCREEN_WIDTH && yy >= 0 && yy < SCREEN_HEIGHT)
                {
                    SCREEN_BUFFER[xx + yy * SCREEN_WIDTH] = color;
                }
            }
        }
    }
}



int main() {
    *(volatile unsigned int*)0x04000000 = 0x0403; // Set GBA rendering context to MODE 3 Bitmap Rendering

    unsigned short color = 0xF800; // Initial color (red)
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    float angle = 0.0f;
    float angularSpeed = 0.01f; // Adjust speed for desired animation 
    unsigned short colorIncrement = 0x001; // Increment for color transition 

    while (1) {
        // Fill screen buffer with black color
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
            SCREEN_BUFFER[i] = 0x0000; // Fill screen buffer with black color
        }

        // Calculate new position of the circle
        int x = centerX + (int)(CIRCLE_RADIUS * cos(angle));
        int y = centerY + (int)(CIRCLE_RADIUS * sin(angle));

        // Draw circle at new position
        draw_circle(x, y, CIRCLE_RADIUS, color);

        // Increment angle for next frame
        angle += angularSpeed;
        if (angle >= 2 * 3.14159265358979323846) {
            angle -= 2 * 3.14159265358979323846; // Reset angle if it exceeds full circle
        }

        // Change color for the next frame with slower transition
        color += colorIncrement; // Increment the color
        if (color > 0xFFFF || color < 0xF800) {
            colorIncrement = -colorIncrement; // Reverse the increment direction at color boundaries
        }

        // Wait for VBlank
        while (*(volatile unsigned short*)0x04000004 & 1); // Wait for VBlank
        while (!(*(volatile unsigned short*)0x04000004 & 1)); // Wait until VBlank ends
    }
    
    return 0;
}
