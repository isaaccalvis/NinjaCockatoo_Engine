#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

#include "physfs/include/physfs.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

// TODO : AQUESTA LIB POTSER NO S'HA D'INCLOURE AQUI
#pragma comment (lib, "physfs/libx86/physfs.lib")

enum main_states
{	
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	PHYSFS_init(argv[0]);
	char full[_MAX_PATH];
	_fullpath(full, ".\\", _MAX_PATH);
	PHYSFS_addToSearchPath(full, 1);
	PHYSFS_setWriteDir(full);

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG_IDE("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			App->input->LoadPCGSeed(argc, argv);
			break;

		case MAIN_START:

			LOG_IDE("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG_IDE("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG_IDE("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{

			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG_IDE("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG_IDE("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG_IDE("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	return main_return;
}