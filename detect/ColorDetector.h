#pragma once

#include "ColorSensor.h"
#include "Light.h"

using namespace spikeapi;



enum class Color
{ 
    Black = 0,  //  黒
    White = 1,  //  白
    Gray = 2,   //  灰色
    Green = 3,  //  緑
    Yellow = 4,//  黄
    Red = 5,    //  赤
    Blue = 6,   //  青
    Unknown = 7, //  未知の色
    None = 8   //  未検出
};

struct ColorHSVReference
{
    Color color;

    int h;
    int s;
    int v;

    double hWeight;
    double sWeight;
    double vWeight;
};

class ColorDetector
{
public:
    ColorDetector(ColorSensor& sensor,
                  Light& light);

    Color detect();

private:
    ColorSensor& mColorSensor;
    Light& mLight;

    double calculateDistance(
        const ColorSensor::HSV& hsv,
        const ColorHSVReference& reference);

    void setLight(Color color);
};