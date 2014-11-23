#ifndef TIMER_H
#define TIMER_H

#include <allegro5/allegro.h>

class Timer
{
public:
	Timer(unsigned int interval);
	~Timer(void);
	void wait(void);
private:
	ALLEGRO_TIMER *allegroTimer;
	ALLEGRO_EVENT_QUEUE *eventQueue;


};

#endif
