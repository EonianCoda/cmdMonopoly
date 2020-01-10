#include "Game.h"



Game::Game()
{
}


Game::~Game()
{
}

void Game::Start()
{
	UI::getInstance()->ShowScreen(Display::Screen::MainMenu);
	
	while (1) {
		GameSystem::getInstance()->MouseInput();
		Sleep(100);
	}
}
