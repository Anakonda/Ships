#include "timer.h"

Timer::Timer(unsigned int interval)
{
	this->allegroTimer = al_create_timer(interval / 1000.0);
	this->eventQueue = al_create_event_queue();
	al_register_event_source(this->eventQueue, al_get_timer_event_source(this->allegroTimer));
	al_start_timer(this->allegroTimer);
}

Timer::~Timer(void)
{
	al_destroy_timer(this->allegroTimer);
	al_destroy_event_queue(this->eventQueue);
}

void Timer::wait(void)
{
	while(true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(this->eventQueue, &event);

		switch(event.type)
		{
			case ALLEGRO_EVENT_TIMER:
			{
				return;
			}
		}
	}
}
