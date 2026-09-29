#pragma once

class FoliageColor {
    // 4J Stu - We don't want to use this any more
    // private:
    //	static std::vector<int> pixels;
    //
    // public:
    //	static void init(std::vector<int>& pixels);
    //     static int get(double temp, double rain);

public:
    static int getEvergreenColor();
    static int getBirchColor();
    static int getDefaultColor();
};