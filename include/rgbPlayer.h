// (c) Kostyantyn Matviyevskyy aka kostyamat and Stepko
// 27.01.2022
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
#ifndef _RGBP_H
#define _RGBP_H

#include "config.h"
#include "main.h"
#include "lamp.h"

#define MULTIPLIC 256

class RGBPlayer {
    private:
        uint8_t frameWidth, frameHeight, frames, frame = 0; 
        uint16_t maxSize; 
        int16_t corrX, corrY;
        uint16_t resizeX, resizeY;
        bool firstStart = true;
        uint8_t* frameBuf = nullptr;
        uint32_t timer = millis();
        bool done = false;

        File rgbFile;

        void load_PGM(uint8_t *data) {
            LOG(println, F("RGBPlayer: Start. PROGMEM mode."));
            frameWidth = pgm_read_byte(data);
            frameHeight = pgm_read_byte(data + 1);
            frames = pgm_read_byte(data + 2);
            LOG(printf_P, PSTR("RGBPlayer: Image loaded. It has %d frames. Image size %dX%d.\n"), frames, frameWidth, frameHeight);

            calc();
        }

        void load_FILE_332(String filename) {
            LOG(println, F("RGBPlayer: Start. File rgb332 mode."));
            File rgbFile = LittleFS.open(filename, "r");
            if (rgbFile and rgbFile.isFile() and rgbFile.size() >= (3 + WIDTH*HEIGHT)) {
                rgbFile.read(&frameWidth, 1);
                rgbFile.read(&frameHeight, 1);
                rgbFile.read(&frames, 1);
                LOG(printf_P, PSTR("RGBPlayer: File %s loaded. It has %d frames. Image size %dX%d.\n"), filename, frames, frameWidth, frameHeight);
            
                calc();
            } else {
                LOG(println, F("File not found or wrong format!"));
            }
        }

        void calc() {
            maxSize = max(WIDTH, HEIGHT);
            resizeX = ((float)frameWidth / maxSize) * MULTIPLIC;
            resizeY = ((float)frameHeight / maxSize) * MULTIPLIC;
            corrX = ((maxSize - WIDTH) / 2) * MULTIPLIC;
            corrY = ((maxSize - HEIGHT) / 2) * MULTIPLIC;
            frameBuf = new uint8_t[frameWidth * frameHeight];
        }

        void getFromPGM_332(uint8_t *data, int16_t frame) {
            for (uint16_t i = 0; i < frameWidth * frameHeight; i++) {
                uint32_t index = (frameWidth * frameHeight) * frame + i;
                frameBuf[i] = pgm_read_byte_far(data + index + 3);
            }
        }

        void getFromFile_332(uint8_t frame) {
            uint32_t index = (frameWidth * frameHeight) * frame + 3;
            rgbFile.seek(index, SeekSet);

            for(uint16_t i = 0; i < frameWidth * frameHeight; i++) {
                uint8_t data;
                rgbFile.read(&data, 1);
                frameBuf[i] = data;
            }
        }

        CRGB rgb332_To_CRGB(uint8_t value){
            CRGB color;
            color.r = value & 0xe0; // mask out the 3 bits of red at the start of the byte
            color.r |= (color.r >> 3); // extend limited 0-224 range to 0-252
            color.r |= (color.r >> 3); // extend limited 0-252 range to 0-255
            color.g = value & 0x1c; // mask out the 3 bits of green in the middle of the byte
            color.g |= (color.g << 3) | (color.r >> 3); // extend limited 0-34 range to 0-255
            color.b = value & 0x03; // mask out the 2 bits of blue at the end of the byte
            color.b |= color.b << 2; // extend 0-3 range to 0-15
            color.b |= color.b << 4; // extend 0-15 range to 0-255
            return color;
        }

        void drawFrame () {
            for (uint16_t y = 0; y < (maxSize * MULTIPLIC); y+= resizeY) {
                for (uint16_t x = 0; x < (maxSize * MULTIPLIC); x+= resizeX) {
                    uint16_t index = ((x / MULTIPLIC * resizeX) / MULTIPLIC) + ((y/MULTIPLIC * resizeY) / MULTIPLIC) * frameWidth;
                    EffectMath::getPixel(((x - corrX) /MULTIPLIC), (HEIGHT- 1) - (y - corrY) / MULTIPLIC) = rgb332_To_CRGB(frameBuf[index]);
                }
            }
        }
    
    public:

        void play332_PGM(uint8_t *data, uint8_t frameDelay) {
            // load
            if (firstStart) {
                load_PGM(data);
                firstStart = false;
            }

            if ((millis() - timer >= frameDelay) and done) {
                drawFrame();
                done = false;
                timer = millis();
                frame++;
                if (frame >= frames)
                    frame = 0;
            } else if ((millis() - timer < frameDelay) and !done) {
                getFromPGM_332(data, frame);
                done = true;
            }
        }

        

        void play332_File(String filename, uint8_t frameDelay) {
            // load
            if (firstStart) {
                load_FILE_332(filename);
                firstStart = false;
            }

            if ((millis() - timer >= frameDelay) and done) {
                drawFrame();
                FastLED.show();
                done = false;
                timer = millis();
                frame++;
                if (frame >= frames)
                    frame = 0;
            } else if ((millis() - timer < frameDelay) and !done) {
                getFromFile_332(frame);
                done = true;
            }
        }

        ~RGBPlayer() {
            if (rgbFile and rgbFile.isFile()) {
                rgbFile.close();
                LOG(println, F("RGBPlayer: Stop. File closed."));
            }
            delete [] frameBuf;
            LOG(println, F("RGBPlayer: Stop and destroyed."));
        }
};


#endif