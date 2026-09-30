#pragma once
#include "CourseParameter.h"

#ifdef COURSE_LEFT

#include "L-CourseParameter.h"
constexpr int COURSE_DIRECTION = 1;

#elif defined(COURSE_RIGHT)

#include "R-CourseParameter.h"
constexpr int COURSE_DIRECTION = -1;

#endif