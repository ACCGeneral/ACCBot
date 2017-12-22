#include "usefulFunctions.h"

float invert(float num, float max)
{
	float newnum;

	newnum = num;
	newnum -= max;
	newnum = std::fabs(newnum);

	return newnum;
}

float clamp(float num)
{
	float clampnum = std::fmin(1, num);
	clampnum = std::fmax(0, clampnum);

	return clampnum;
}

float setclamp(float num, float min, float max)
{
	float clampnum = std::fmin(max, num);
	clampnum = std::fmax(min, clampnum);

	return clampnum;
}


float scoringfixer(float num1, float num2)
{
	if (num1 >= 1 || num2 >= 1)
	{
		return num1 * num2;
	}
	if (num1 == 0 || num2 == 0)
	{
		return num1 * num2;
	}
	else
	{
		float minnum = std::fmin(num1, num2);

		if (minnum == num1)
		{
			float compen = 2 - num2;

			float returnvalue = num1 * num2;

			returnvalue *= compen;

			return returnvalue;
		}
		else if (minnum == num2)
		{
			float compen = 2 - num1;

			float returnvalue = num1 * num2;

			returnvalue *= compen;

			return returnvalue;
		}

	}
	return 0.0f;
}

int getfromproduction(std::vector<buildinfo> buildorder, BWAPI::UnitType type)
{
	int num = 0;

	for (int i = 0; i < buildorder.size(); i++)
	{
		if (buildorder[i].mytype == type)
		{
			num++;
		}
	}

	return num;

}