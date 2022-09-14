// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Enemy.h"

Game::Game()
:_pWindow(nullptr)
,_pRenderer(nullptr)
,_bIsRunning(true) {
	
}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	_pWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 4)", 100, 100, 1024, 768, 0);
	if (!_pWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	_pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_pRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	_loadData();

	_nTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop() {
	while (_bIsRunning) {
		_processInput();
		_updateGame();
		_generateOutput();
	}
}

void Game::_processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				_bIsRunning = false;
				break;
		}
	}
	
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE]) {
		_bIsRunning = false;
	}

	for (auto actor : _vecActors) {
		actor->ProcessInput(keyState);
	}
}

void Game::_updateGame() {
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), _nTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - _nTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	_nTicksCount = SDL_GetTicks();

	// Update all actors
	for (auto actor : _vecActors) {
		actor->Update(deltaTime);
	}
	
	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : _vecActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from _vecActors)
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::_generateOutput()
{
	SDL_SetRenderDrawColor(_pRenderer, 34, 139, 34, 255);
	SDL_RenderClear(_pRenderer);
	
	// Draw all sprite components
	for (auto sprite : _vecSprites) {
		sprite->Draw(_pRenderer);
	}

	SDL_RenderPresent(_pRenderer);
}

void Game::_loadData() {
    // Create the Grid.
	_pGrid = new Grid(this);
}

void Game::_unloadData() {
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!_vecActors.empty()) {
		delete _vecActors.back();
	}

	// Destroy textures
	for (auto i : _mapTextures) {
		SDL_DestroyTexture(i.second);
	}
    _mapTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	
    // Is the texture already in the map?
	auto iter = _mapTextures.find(fileName);
	if (iter != _mapTextures.end()) {
		tex = iter->second;
	}
	else {
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(_pRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

        _mapTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown() {
	_unloadData();
	IMG_Quit();
	SDL_DestroyRenderer(_pRenderer);
	SDL_DestroyWindow(_pWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
    _vecActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	// Is it in actors?
	auto iter = std::find(_vecActors.begin(), _vecActors.end(), actor);
	if (iter != _vecActors.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, _vecActors.end() - 1);
        _vecActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = _vecSprites.begin();
	for ( ;
		iter != _vecSprites.end();
		++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// Inserts element before position of iterator
    _vecSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// (We can't swap because it ruins ordering)
	auto iter = std::find(_vecSprites.begin(), _vecSprites.end(), sprite);
    _vecSprites.erase(iter);
}

