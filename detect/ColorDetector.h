#pragma once

#include "ColorSensor.h"
#include "Light.h"

using namespace spikeapi;

enum class Color
{
    None,
    Black,
    White,
    Gray,
    Green,
    Yellow,
    Red,
    Blue,
    Unknown
};

struct ColorHSVRange
{
    Color color;

    int hMin;
    int hMax;

    int sMin;
    int sMax;

    int vMin;
    int vMax;
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
};