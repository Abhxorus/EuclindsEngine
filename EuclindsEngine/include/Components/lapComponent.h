#pragma once
#include "Prerequisitos.h"

struct
	LapComponent {
	int   lap = 0;
	float currentLapTime = 0.f;
	float bestLapTime = std::numeric_limits<float>::infinity();
	float totalTime = 0.f;

	size_t checkpoint = 0;             // último waypoint alcanzado
	float  progressAlongSegment = 0.f; // 0..1 entre checkpoint y next
};