#ifndef __COLOR_PALETTE_H
#define __COLOR_PALETTE_H
// переместил в platformio.ini
// #ifdef ESP8266
// #define FASTLED_USE_PROGMEM             (1)
// #endif

#include "FastLED.h"
#include "colorutils.h"
#include "LList.h"

DECLARE_GRADIENT_PALETTE(MBVioletColors_gp);

//////////////////////////////

DECLARE_GRADIENT_PALETTE(ib_jul01_gp);
DECLARE_GRADIENT_PALETTE(es_vintage_57_gp);
DECLARE_GRADIENT_PALETTE(es_vintage_01_gp);
DECLARE_GRADIENT_PALETTE(es_rivendell_15_gp);
DECLARE_GRADIENT_PALETTE(rgi_15_gp);
DECLARE_GRADIENT_PALETTE(retro2_16_gp);
DECLARE_GRADIENT_PALETTE(Analogous_1_gp);
DECLARE_GRADIENT_PALETTE(es_pinksplash_08_gp);
DECLARE_GRADIENT_PALETTE(es_pinksplash_07_gp);
DECLARE_GRADIENT_PALETTE(Coral_reef_gp);
DECLARE_GRADIENT_PALETTE(es_ocean_breeze_068_gp);
DECLARE_GRADIENT_PALETTE(es_ocean_breeze_036_gp);
DECLARE_GRADIENT_PALETTE(departure_gp);
DECLARE_GRADIENT_PALETTE(es_landscape_64_gp);
DECLARE_GRADIENT_PALETTE(es_landscape_33_gp);
DECLARE_GRADIENT_PALETTE(rainbowsherbet_gp);
DECLARE_GRADIENT_PALETTE(gr65_hult_gp);
DECLARE_GRADIENT_PALETTE(gr64_hult_gp);
DECLARE_GRADIENT_PALETTE(GMT_drywet_gp);
DECLARE_GRADIENT_PALETTE(ib15_gp);
DECLARE_GRADIENT_PALETTE(Fuschia_7_gp);
DECLARE_GRADIENT_PALETTE(es_emerald_dragon_08_gp);
DECLARE_GRADIENT_PALETTE(lava_gp);
DECLARE_GRADIENT_PALETTE(fire_gp);
DECLARE_GRADIENT_PALETTE(Colorfull_gp);
DECLARE_GRADIENT_PALETTE(Magenta_Evening_gp);
DECLARE_GRADIENT_PALETTE(Pink_Purple_gp);
DECLARE_GRADIENT_PALETTE(Sunset_Real_gp);
DECLARE_GRADIENT_PALETTE(es_autumn_19_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Blue_Magenta_White_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Magenta_Red_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Red_Magenta_Yellow_gp);
DECLARE_GRADIENT_PALETTE(Blue_Cyan_Yellow_gp);

DECLARE_GRADIENT_PALETTE(purple_gp);
DECLARE_GRADIENT_PALETTE(outrun_gp);
//DECLARE_GRADIENT_PALETTE(greenblue_gp);
DECLARE_GRADIENT_PALETTE(redyellow_gp);

class GradientPalette{
    CRGBPalette32 pl;
    int sh;
    int mn;
    int mx;
    public:
    GradientPalette(CRGBPalette32 pallete, uint8_t shift = 0, uint8_t min = 0, uint8_t max = 0) {
        pl = pallete; sh = shift; mn = min; mx = max;
    }
    CRGB GetColor(uint8_t idx, uint8_t br) {
        if (mn && idx < mn) idx = mn;
        if (mx && idx > mx) idx = mx;
        return ColorFromPalette(pl, (uint8_t)idx + sh, br);
    }
};

class GradientPaletteList{
    LList<GradientPalette*> palletes;
    public:
    GradientPaletteList(): palletes(){

    }
    ~GradientPaletteList(){
        while (palletes.size()) {
            GradientPalette *pl = palletes.shift();
            delete pl;
        }
    }

    GradientPalette *operator[](int i){ return palletes[i]; }

    int size(){ return palletes.size(); }
    void del(int idx){ palletes.remove(idx); }
    int add(CRGBPalette32 pallete, int shift, uint8_t min = 0, uint8_t max = 0) {
        palletes.add(new GradientPalette(pallete, shift, min, max));
        return size();
    }
    int add(int idx, CRGBPalette32 pallete, int shift, uint8_t min = 0, uint8_t max = 0) {
        GradientPalette *pl = palletes.get(idx);
        if (pl) {
            delete pl;
            palletes.set(idx, new GradientPalette(pallete, shift, min, max));
        }
        return size();
    }
};

#endif
