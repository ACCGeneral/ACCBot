#pragma once

#include <iostream>
#include "buildinfo.h"

float invert(float num, float max);

float scoringfixer(float num1, float num2);

float clamp(float num);

float setclamp(float num, float min, float max);

int getfromproduction(std::vector<buildinfo> buildorder, BWAPI::UnitType type);