#include <SDL.h>
#include <spdlog/spdlog.h>

#include "src/sandbox/ExSideGame.h"
#include "src/time/DeltaTimer.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
	spdlog::set_level(spdlog::level::debug);

	ExSideGame* game = new ExSideGame();

	bool isGameInit = game->init();

	if (!isGameInit)
	{
		spdlog::critical("Game engine could not been initialize!");
		return EXIT_FAILURE;
	}

	while (game->isRunning())
	{
		game->events();
		game->update(DeltaTimer::getInstance()->getDeltaTime());
		game->draw();
		DeltaTimer::getInstance()->tick();
	}

	game->dispose();

	return EXIT_SUCCESS;
}
