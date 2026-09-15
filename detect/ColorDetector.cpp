#include "ColorDetector.h"
#include "Logger.h"

static constexpr ColorHSVRange mColorHSVRanges[] =
{
    { Color::Red,    0, 29, 27,100, 58,100 },
    { Color::Red,  280,360, 27,100, 58,100 },
 
    { Color::Blue, 180,279, 44,100, 60, 85 },
 
    { Color::Yellow, 30,129, 35,70, 85,100 },
 
    { Color::Green, 130,179, 32,85, 45,70 },
 
    { Color::Gray,   0,360, 35,50, 65,100 },
 
    { Color::Black,  0,360,  0,40,  0,34 },
 
    { Color::White,  0,360,  0,30, 90,100 }
};

ColorDetector::ColorDetector(ColorSensor& sensor,
                             Light& light)
    : mColorSensor(sensor),
      mLight(light)
{
}

Color ColorDetector::detect()
{
    ColorSensor::HSV hsv;
    mColorSensor.getHSV(hsv);

    Logger::printf("今だけ：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
    for (const auto& range : mColorHSVRanges)
    {
        if (hsv.h >= range.hMin && hsv.h <= range.hMax &&
            hsv.s >= range.sMin && hsv.s <= range.sMax &&
            hsv.v >= range.vMin && hsv.v <= range.vMax)
        {
            if(range.color == Color::Red)
            {
                Logger::printf("判定色:赤\n");
                Logger::printf("赤：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
                mLight.turnOnColor(Light::EColor::RED);
            }
            else if(range.color == Color::Yellow)
            {
                Logger::printf("判定色:黄\n");
                Logger::printf("黄：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
                mLight.turnOnColor(Light::EColor::YELLOW);
            }
            else if(range.color == Color::Blue)
            {
                Logger::printf("判定色:青\n");
                Logger::printf("青：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
                mLight.turnOnColor(Light::EColor::BLUE);
            }
            else if(range.color == Color::Green)
            {
                Logger::printf("判定色:緑\n");
                Logger::printf("緑：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
                mLight.turnOnColor(Light::EColor::GREEN);
            }
            return range.color;
        }
    }

    mLight.turnOnColor(Light::EColor::BLACK);
    return Color::Unknown;   // または適切なデフォルト
}