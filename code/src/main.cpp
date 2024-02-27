
#ifdef _WIN32
	#include <windows.h>
#endif

#include <my-game/game.hpp>

int main(int /*argc*/, char* /*argv*/[]) {
#ifdef _WIN32
	SetConsoleTitle("My game");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif

	my_game::Game engine;
	engine.setFPS(60);
	engine.setTPS(66);
	engine.run();

	return 0;
}
