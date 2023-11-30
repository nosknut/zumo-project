#ifndef DISPLAY_h
#define DISPLAY_h

#include <U8glib.h>
#include "CarLink/ChargeState.h"

struct Display
{
    int sckPin = 10;
    int mosiPin = 9;
    int csPin = 12;
    int dcPin = 11;
    int resetPin = 13;

    // Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
    // https://github.com/olikraus/u8glib/wiki/thelloworld
    U8GLIB_SSD1306_128X64 u8g = U8GLIB_SSD1306_128X64(
        sckPin,
        mosiPin,
        csPin,
        dcPin,
        resetPin);

    void setup(void)
    {
        if (u8g.getMode() == U8G_MODE_R3G3B2)
        {
            u8g.setColorIndex(255); // white
        }
        else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
        {
            u8g.setColorIndex(3); // max intensity
        }
        else if (u8g.getMode() == U8G_MODE_BW)
        {
            u8g.setColorIndex(1); // pixel on
        }
        else if (u8g.getMode() == U8G_MODE_HICOLOR)
        {
            u8g.setHiColorByRGB(255, 255, 255);
        }

        u8g.setFont(u8g_font_unifont);
    }

    void draw(ChargeState &chargeState)
    {
        int lineHeight = 12;
        int line = 1;

        if (chargeState.charging)
        {
            u8g.drawStr(4, lineHeight * line++, "Charging ...");
            u8g.drawStr(4, lineHeight * line++, ("Car " + String(chargeState.carId)).c_str());
            u8g.drawStr(4, lineHeight * line++, (String(chargeState.chargeLevel) + " / " + String(chargeState.targetChargeLevel) + "%").c_str());
            u8g.drawStr(4, lineHeight * line++, (String(chargeState.accountBalance) + "$").c_str());

            if (chargeState.allowDebt)
            {
                u8g.drawStr(4, lineHeight * line++, "Debt approved");
            }
            else
            {
                u8g.drawStr(4, lineHeight * line++, "Debt declined");
            }
        }
        else
        {
            u8g.drawStr(4, lineHeight * line++, "Not Charging");
        }
    }

    void update(ChargeState &chargeState)
    {
        draw(chargeState);
        u8g.firstPage();
        do
        {
            draw(chargeState);
        } while (u8g.nextPage());
    }
};

#endif