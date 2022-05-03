/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

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