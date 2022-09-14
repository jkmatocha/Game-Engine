// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(class Game* game)
:Actor(game)
,_pNext(nullptr)
,_f(0.0f)
,_g(0.0f)
,_h(0.0f)
,_bBlocked(false)
,_pSprite(nullptr)
,_tileState(EDefault)
,_bSelected(false) {
	_pSprite = new SpriteComponent(this);
	_updateTexture();
}

void Tile::SetTileState(TileState state)
{
	_tileState = state;
	_updateTexture();
}

void Tile::ToggleSelect()
{
	_bSelected = !_bSelected;
	_updateTexture();
}

void Tile::_updateTexture()
{
	std::string text;
	switch (_tileState) {
		case EStart:
			text = "assets/TileTan.png";
			break;
		case EBase:
			text = "assets/TileGreen.png";
			break;
		case EPath:
			if (_bSelected)
				text = "assets/TileGreySelected.png";
			else
				text = "assets/TileGrey.png";
			break;
		case EDefault:
		default:
			if (_bSelected)
				text = "assets/TileBrownSelected.png";
			else
				text = "assets/TileBrown.png";
			break;
	}
	_pSprite->SetTexture(GetGame()->GetTexture(text));
}
