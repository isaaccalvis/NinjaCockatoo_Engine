#ifndef __ModuleInGame_H__
#define __ModuleInGame_H__

#include <list>
#include "Module.h"
#include "Timer.h"

class ModuleInGame : public Module
{
public:
	ModuleInGame(Application* app, bool start_enabled = true);

	update_status Update(float dt);
	bool CleanUp();

	void StartGame();
	void StopGame();
	void PauseGame();
	void ContinueGame();
	void TickGame();

public:
	bool onGame = false;
	bool runingGame = false;
	Timer timer;
	unsigned int logicUpdates = 0;

	float cameraSpeed = 0.2f;
	float cameraRotSpeed = 0.8f;
	float bulletImpulse = 15.0f;
};

#endif