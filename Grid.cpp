// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Grid.h"
#include "Tile.h"
#include "Enemy.h"
#include <algorithm>
#include <queue>
#include "SDL/SDL_image.h"
using namespace std;

Grid::Grid(class Game* game)
:Actor(game)
,_pSelectedTile(nullptr) {
	// 7 rows, 16 columns
    _vecTiles.resize(NUM_ROWS);
	for (size_t i = 0; i < _vecTiles.size(); i++) {
		_vecTiles[i].resize(NUM_COLUMNS);
	}
	
	// Create tiles
	for (size_t i = 0; i < NUM_ROWS; i++) {
		for (size_t j = 0; j < NUM_COLUMNS; j++) {
            _vecTiles[i][j] = new Tile(GetGame());
            _vecTiles[i][j]->SetPosition(Vector2(TILE_SIZE/2.0f + j * TILE_SIZE, START_Y + i * TILE_SIZE));
		}
	}
	
	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);
	
	// Set up adjacency lists
	for (size_t i = 0; i < NUM_ROWS; i++) {
		for (size_t j = 0; j < NUM_COLUMNS; j++) {
			if (i > 0) {
                _vecTiles[i][j]->_vecAdjacent.push_back(_vecTiles[i-1][j]);
			}
			if (i < NUM_ROWS - 1) {
                _vecTiles[i][j]->_vecAdjacent.push_back(_vecTiles[i+1][j]);
			}
			if (j > 0) {
                _vecTiles[i][j]->_vecAdjacent.push_back(_vecTiles[i][j-1]);
			}
			if (j < NUM_COLUMNS - 1) {
                _vecTiles[i][j]->_vecAdjacent.push_back(_vecTiles[i][j+1]);
			}
		}
	}
	
    // Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	_updatePathTiles(GetStartTile());
	
	_fNextEnemy = ENEMY_TIME;
}

void Grid::ActorInput(const uint8_t *keyState) {
    // Process mouse
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);
    if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT) {
        _processClick(x, y);
    }
}

void Grid::_selectTile(size_t row, size_t col)
{
	// Make sure it's a valid selection
	Tile::TileState tstate = _vecTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) {
		// Deselect previous one
		if (_pSelectedTile)
		{
            _pSelectedTile->ToggleSelect();
		}
        _pSelectedTile = _vecTiles[row][col];
        _pSelectedTile->ToggleSelect();
	}
}

void Grid::_processClick(int x, int y) {
	y -= static_cast<int>(START_Y - TILE_SIZE / 2);
	if (y >= 0) {
		x /= static_cast<int>(TILE_SIZE);
		y /= static_cast<int>(TILE_SIZE);
		if (x >= 0 && x < static_cast<int>(NUM_COLUMNS) && y >= 0 && y < static_cast<int>(NUM_ROWS)) {
			_selectTile(y, x);
            _blockPath();
		}
	}
}

// Implement A* pathfinding.
// Use the concept of an openSet vector and open/closed booleans to
// find the closest neighbor to the current node.
// Closest neighbor has the lowest f-function.
// That node becomes the next current node in the path.
bool Grid::FindPath(Tile* start, Tile* goal)
{
    // TODO:
    // Initialize entire _vecTiles grid to have _g of 0.0f,
    // _bInOpenSet to false, _bInClosedSet to false.

	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLUMNS; j++)
		{
			_vecTiles[i][j]->_g = 0.0f;
			_vecTiles[i][j]->_bInClosedSet = false;
			_vecTiles[i][j]->_bInOpenSet = false;
		}
	}
    
    // Declare a vector of Tile pointers to be the open set.
    vector<Tile*> openSet;
    
    // Set current node to start, and add it to the closed set (just set the boolean).
    Tile* pCurrent = start;
	pCurrent->_bInClosedSet = true;
    
    // TODO:
    // Loop until current is equal to goal.
    do {
        // TODO:
        // For every Tile* in current's adjacency list.
        for (Tile* neighbor : pCurrent->_vecAdjacent) {
            // TODO:
            // If this Tile is blocked, just continue.
			if (neighbor->_bBlocked)
			{
				continue;
			}
            
            // TODO:
            // Only check nodes that aren't in the closed set and aren't in the open set.
			if (!neighbor->_bInClosedSet)
			{
				if (!neighbor->_bInOpenSet)
				{
					// TODO:
					// Set next of this neighbor to current.
					neighbor->_pNext = pCurrrent;

					//h
					neighbor->_h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// Compute g(x) to be the next's g plus the cost of traversing
					// this edge (just the TILESIZE).
					neighbor->_g = pCurrent->_g + TILE_SIZE;
					// Compute the f(x) to be _h + _g.
					neighbor->_f = neighbor->_g + neighbor->_h;
					// Add this Tile to the open set.
					openSet.push_back(neighbor);

					neighbor->_bInOpenSet = true;

				}
			}
            
        }
        
        // If open set is empty, all possible paths are exhausted.
        if (openSet.empty()) {
            break;
        }
        
        // Find lowest cost node in open set (using a lambda function).
        auto iter = std::min_element(openSet.begin(), openSet.end(),
                                     [](Tile* a, Tile* b)
									 {
                                         return a->_f < b->_f;
                                     });
        
        // TODO:
        // iter is the node with the lowest _f in the open set, make
        // this the new current and move it from open to closed.
		pCurrent = *iter;
		openSet.erase(iter);

		pCurrent->_bInOpenSet = false;
		pCurrent->_bInClosedSet = true;
		
        
        
    } while (pCurrent != goal);
    
    // Did we find a path?
    return (pCurrent == goal) ? true : false;
}

// Sets state of each tile in Grid, making
// ones on path look different from others.
void Grid::_updatePathTiles(class Tile* start) {
	// Reset all tiles to normal (except for start/end)
	for (size_t i = 0; i < NUM_ROWS; i++) {
		for (size_t j = 0; j < NUM_COLUMNS; j++) {
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15)) {
				_vecTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	
	Tile* t = start->_pNext;
	while (t != nullptr && t != GetEndTile()) {
		t->SetTileState(Tile::EPath);
		t = t->_pNext;
	}
}

void Grid::_blockPath() {
	if (_pSelectedTile && !_pSelectedTile->_bBlocked)
	{
        _pSelectedTile->_bBlocked = true;
        FindPath(GetEndTile(), GetStartTile());
		_updatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile() {
	return _vecTiles[3][0];
}

Tile* Grid::GetEndTile() {
	return _vecTiles[3][15];
}

void Grid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	
	// Is it time to spawn a new enemy?
	_fNextEnemy -= deltaTime;
	if (_fNextEnemy <= 0.0f) {
		new Enemy(GetGame());
		_fNextEnemy += ENEMY_TIME;
	}
}
