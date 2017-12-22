#include "utilityfunction.h"

float logisticscore(float scaler, float input, float negation, float norma)
{
	float score = 1 / (1 + (std::pow((2.718 * scaler), ((input)-negation)*norma)));

	score = fmin(fmax(score, 0), 1);
	
	return score;
}

float linearandquadratic(float addition, float input, float power)
{
	float score;

	score = std::pow((input + addition), power);
	score = fmin(fmax(score, 0), 1);

	return score;

}
