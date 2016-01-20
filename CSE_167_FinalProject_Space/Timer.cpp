#include "GLee.h"

#include "Timer.h"
using namespace std;

Timer::Timer(float maxTimeStep /* = 0.03333f */)
	: maxTimeStep(maxTimeStep)
	, previous(clock() / (float)CLOCKS_PER_SEC)
{}

float Timer::getTimePassed() const
{
	float currentTime = clock() / (float)CLOCKS_PER_SEC;
	float timePassed = currentTime - previous;
	previous = currentTime;

	// Clamp to the max time step
	timePassed = min(timePassed, maxTimeStep);

	return timePassed;
}
