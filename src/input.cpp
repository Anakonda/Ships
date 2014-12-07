
#include "input.h"
#include "timer.h"

#include <allegro5/allegro.h>
#include <thread>

#include <iostream>

std::thread thread;
bool controlsClosing;

GamepadStatus controllerStatus;

void controlsLoop(void)
{
	#ifdef _WIN32
	al_set_config_value(al_get_system_config(), "joystick", "driver", "xinput");
	#endif // _WIN32
	al_install_joystick();

	int numJoysticks = al_get_num_joysticks();

	if(numJoysticks > 0)
	{
		controllerStatus.connected = true;
		ALLEGRO_JOYSTICK *joystick = al_get_joystick(0);
		Timer timer(10);
		while(!controlsClosing)
		{
			timer.wait();
			ALLEGRO_JOYSTICK_STATE joystickState;
			al_get_joystick_state(joystick, &joystickState);
			controllerStatus.ThumbLX = joystickState.stick[0].axis[0] * 32767;
			controllerStatus.ThumbLY = -joystickState.stick[0].axis[1] * 32767;
			controllerStatus.ThumbRX = joystickState.stick[1].axis[0] * 32767;
			controllerStatus.ThumbRY = -joystickState.stick[1].axis[1] * 32767;
			controllerStatus.A = joystickState.button[0] == 32767;
		}
	}
	al_uninstall_joystick();
}

void initializeControls()
{
	controlsClosing = false;
	thread = std::thread(controlsLoop);
}

void deinitializeControls(void)
{
	controlsClosing = true;
	thread.join();
}
