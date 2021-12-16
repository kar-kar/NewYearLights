#pragma once

#define PIXEL_COUNT 150
#define MATRIX_WIDTH 14
#define MATRIX_HEIGHT 11

class MyLayout
{
public:
    static uint16_t Map(uint16_t width, uint16_t height, uint16_t x, uint16_t y)
    {
        if (width != MATRIX_WIDTH || height != MATRIX_HEIGHT || x < 0 || x >= width || y < 0 || y >= height)
            return 150;

        switch (x)
        {
        case 0:
            return 10 - y;
        case 1:
            return 12 + y;
        case 2:
            return 34 - y;
        case 3:
            return 36 + y;
        case 4:
            return 150; //invisible
        case 5:
            return 61 - y;
        case 6:
            return 63 + y;
        case 7:
            return 85 - y;
        case 8:
            return 87 + y;
        case 9:
            return 150; //invisible
        case 10:
            return 112 - y;
        case 11:
            return 114 + y;
        case 12:
            return 136 - y;
        case 13:
            return 138 + y;
        }

        return 150;
    }
};

