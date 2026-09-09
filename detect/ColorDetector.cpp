#include "ColorDetector.h"
#include "Logger.h"

static constexpr ColorHSVRange mColorHSVRanges[] =
{
    { Color::Red,      0, 29,31,120,35,100 },
    { Color::Red,    280,360,31,120,35,100 },
    { Color::Blue,   200,279,41,100,35,100 },
    { Color::Yellow,  30,79, 31,100,35,100 },
    { Color::Green,   80,175,31,100,35,100 },
    { Color::Gray,     0,360, 0, 30,35, 89 },
    { Color::Black,    0,360, 0,100, 0, 30 },
    { Color::White,    0,360, 0, 30,90,100 }
};

ColorDetector::ColorDetector(ColorSensor& sensor)
    : mColorSensor(sensor)
{
}

Color ColorDetector::detect()
{
    ColorSensor::HSV hsv;
    mColorSensor.getHSV(hsv);

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
            }
            else if(range.color == Color::Yellow)
            {
                Logger::printf("判定色:黄\n");
                Logger::printf("黄：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
            }
            else if(range.color == Color::Blue)
            {
                Logger::printf("判定色:青\n");
                Logger::printf("青：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
            }
            else if(range.color == Color::Green)
            {
                Logger::printf("判定色:緑\n");
                Logger::printf("緑：H=%d,S=%d,V=%d\n",hsv.h,hsv.s,hsv.v);
            }
            return range.color;
        }
    }

    return Color::Unknown;   // または適切なデフォルト
}