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

class Tile : public Actor {
public:
	friend class Grid;
	enum TileState {
		EDefault,
		EPath,
		EStart,
		EBase
	};
	
	Tile(class Game* game);
	
	void SetTileState(TileState state);
	TileState GetTileState() const { return _tileState; }
	const Tile* GetNext() const { return _pNext; }
    
    void ToggleSelect();
    
private:
	// For pathfinding
	std::vector<Tile*> _vecAdjacent;
	float _f;
	float _g;
	float _h;
	bool _bInOpenSet;
	bool _bInClosedSet;
	bool _bBlocked;
	
    // Start here
	void _updateTexture();
	class SpriteComponent* _pSprite;
	TileState _tileState;
    Tile* _pNext;
    bool _bSelected;
};
