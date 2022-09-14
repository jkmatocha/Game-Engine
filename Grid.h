// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <vector>

class Grid : public Actor {
public:
	Grid(class Game* game);
	
    void ActorInput(const uint8_t *keyState) override;
    
	// Use A* to find a path
	bool FindPath(class Tile* start, class Tile* goal);
    
	// Get start/end tile
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;
private:
	// Select a specific tile
	void _selectTile(size_t row, size_t col);
	
	// Update textures for tiles on path
	void _updatePathTiles(class Tile* start);
	
    // Try to block path
    void _blockPath();
    
    // Handle a mouse click at the x/y screen locations
    void _processClick(int x, int y);
    
	// Currently selected tile
	class Tile* _pSelectedTile;
	
	// 2D vector of tiles in grid
	std::vector<std::vector<class Tile*>> _vecTiles;
	
	// Time until next enemy
	float _fNextEnemy;
	
	// Rows/columns in grid
	const size_t NUM_ROWS = 7;
	const size_t NUM_COLUMNS = 16;
	// Start y position of top left corner
	const float START_Y = 192.0f;
	// Width/height of each tile
	const float TILE_SIZE = 64.0f;
	// Time between enemies
	const float ENEMY_TIME = 1.5f;
};
