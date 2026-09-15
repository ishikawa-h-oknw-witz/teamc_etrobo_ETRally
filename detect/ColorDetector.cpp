#include "ColorDetector.h"
#include "Logger.h"
 
#include <cmath>
 
static constexpr ColorHSVReference mColorReferences[] =
{
    // 色        H    S    V
    { Color::Red,     352,  95, 82 },
    { Color::Yellow, 52,  66, 95 },
    { Color::Green, 150,  81, 46 },
    { Color::Blue,  212, 97, 46 },
 
    // 無彩色
    { Color::Black,   210,  29, 9 },
    { Color::Gray,    207,  29, 66 },
    { Color::White,   193,  27, 99 }
};
 
 
// Hの距離を計算
// 例：359度と1度 → 2度
static double hueDistance(int h1, int h2)
{
    int diff = std::abs(h1 - h2);
 
    if (diff > 180)
    {
        diff = 360 - diff;
    }
 
    return static_cast<double>(diff);
}
 
 
ColorDetector::ColorDetector(ColorSensor& sensor,
                             Light& light)
    : mColorSensor(sensor),
      mLight(light)
{
}
 
 
// HSV同士の距離を計算
double ColorDetector::calculateDistance(
    const ColorSensor::HSV& hsv,
    const ColorHSVReference& reference)
{
    double dh = hueDistance(hsv.h, reference.h);
    double ds = std::abs(hsv.s - reference.s);
    double dv = std::abs(hsv.v - reference.v);
 
    // 正規化
    dh /= 180.0;
    ds /= 100.0;
    dv /= 100.0;
 
    double hWeight = 1.0;
    double sWeight = 0.5;
    double vWeight = 0.5;
 
    // 緑・黄色は色相を特に重視
    if (reference.color == Color::Green ||
        reference.color == Color::Yellow)
    {
        hWeight = 2.0;
        sWeight = 0.5;
        vWeight = 0.3;
    }
 
    return std::sqrt(
        hWeight * dh * dh +
        sWeight * ds * ds +
        vWeight * dv * dv
    );
}
 
 
void ColorDetector::setLight(Color color)
{
    switch (color)
    {
    case Color::Red:
        mLight.turnOnColor(Light::EColor::RED);
        break;
 
    case Color::Yellow:
        mLight.turnOnColor(Light::EColor::YELLOW);
        break;
 
    case Color::Blue:
        mLight.turnOnColor(Light::EColor::BLUE);
        break;
 
    case Color::Green:
        mLight.turnOnColor(Light::EColor::GREEN);
        break;
 
    default:
        mLight.turnOnColor(Light::EColor::BLACK);
        break;
    }
}
 
 
Color ColorDetector::detect()
{
    ColorSensor::HSV hsv;
    mColorSensor.getHSV(hsv);
 
    // Logger::printf(
    //     "測定値: H=%d,S=%d,V=%d\n",
    //     hsv.h,
    //     hsv.s,
    //     hsv.v
    // );
 
 
    double minDistance = 999999.0;
    Color nearestColor = Color::Unknown;
 
 
    for (const auto& reference : mColorReferences)
    {
        double distance = calculateDistance(hsv, reference);
 
        // Logger::printf(
        //     "色=%d 距離=%d\n",
        //     static_cast<int>(reference.color),
        //     static_cast<int>(distance)
        // );
 
 
        if (distance < minDistance)
        {
            minDistance = distance;
            nearestColor = reference.color;
        }
    }
 
 
    Logger::printf(
        "判定色=%d 距離=%d\n",
        static_cast<int>(nearestColor),
        static_cast<int>(minDistance)
    );
 
 
    setLight(nearestColor);
 
    return nearestColor;
}