#ifndef INPUT_H
#define INPUT_H

#include <atomic>

struct GamepadStatus
{
	std::atomic<bool> connected;
	std::atomic<char> LTrigger;
	std::atomic<char> RTrigger;
	std::atomic<bool> A;
	std::atomic<bool> B;
	std::atomic<bool> X;
	std::atomic<bool> Y;
	std::atomic<bool> RB;
	std::atomic<bool> LB;
	std::atomic<bool> DPADUp;
	std::atomic<bool> DPADDown;
	std::atomic<bool> DPADLeft;
	std::atomic<bool> DPADRight;
	std::atomic<bool> ThumbL;
	std::atomic<bool> ThumbR;
	std::atomic<bool> Start;
	std::atomic<bool> Back;
	std::atomic<short> ThumbLX;
	std::atomic<short> ThumbLY;
	std::atomic<short> ThumbRX;
	std::atomic<short> ThumbRY;
};

extern GamepadStatus controllerStatus;

void initializeControls();

void deinitializeControls();

#endif
