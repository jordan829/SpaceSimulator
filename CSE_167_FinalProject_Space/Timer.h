#include <time.h>
#include <algorithm>
#include "ParticleEffect.h"

class Timer
{
public:
	Timer(float maxTimeStep = 0.03333f);
	float getTimePassed() const;

private:
	float maxTimeStep;
	mutable float previous;
};