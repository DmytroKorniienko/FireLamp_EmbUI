// https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-102618

#ifndef __4MATRIX_XY_H
#define __4MATRIX_XY_H


const uint8_t mWH = WIDTH /2;

uint32_t matrix4_XY(uint16_t x, uint16_t y)
{
    if (x >= mWH) // 1st and 2nd matrixes 8x8
    {
        x = x - mWH;    // shift of 0:0 coordinate
        if (y % 2 == 0) // even rows
        {
            return y * mWH + x;
        }
        else // odd rows
        {
            return y * mWH + mWH - x - 1;
        }
    }
    else // 3rd and 4th matrixes 8x8
    {
        if (y % 2 == 0) // even rows
        {
            return (mWH * mWH * 2) + (HEIGHT - y - 1) * mWH + x;
        }
        else // odd rows
        {
            return (mWH * mWH * 2) + (HEIGHT - y - 1) * mWH + mWH - x - 1;
        }
    }
}

#endif