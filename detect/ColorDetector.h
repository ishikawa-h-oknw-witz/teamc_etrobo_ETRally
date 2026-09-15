#pragma once
 
#include "ColorSensor.h"
#include "Light.h"
 
using namespace spikeapi;
 
enum class Color
{
    None,   // 0: 未検出
    Black,  // 1: 黒
    White,  // 2: 白
    Gray,   // 3: 灰色
    Green,  // 4: 緑
    Yellow,// 5: 黄
    Red,    // 6: 赤
    Blue,   // 7: 青
    Unknown // 8: 未知の色
};
 
struct ColorHSVReference
{
    Color color;
 
    int h;
    int s;
    int v;
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