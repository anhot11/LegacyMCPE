#include "ScreenSizeCalculator.h"

#include <math.h>

#include "minecraft/client/Options.h"

// who the fuck thought this was a good idea
ScreenSizeCalculator::ScreenSizeCalculator(Options* options, int width,
                                           int height, int forceScale /*=-1*/) {
    w = width;
    h = height;
    if (forceScale == -1) {
        scale = 1;

        int maxScale = options->guiScale;
        if (maxScale == 0) maxScale = 1000;
        while (scale < maxScale && w / (scale + 1) >= 320 &&
               h / (scale + 1) >= 240)  // ughh
        {
            scale++;
        }
    } else {
        scale = forceScale;
    }
    rawWidth = w / (double)scale;
    rawHeight = h / (double)scale;
    w = (int)ceil(rawWidth);
    h = (int)ceil(rawHeight);
}

int ScreenSizeCalculator::getWidth() { return w; }

int ScreenSizeCalculator::getHeight() { return h; }