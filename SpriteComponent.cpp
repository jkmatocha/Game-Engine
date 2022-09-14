// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,_pTexture(nullptr)
	,_nDrawOrder(drawOrder)
	,_nTexWidth(0)
	,_nTexHeight(0) {
	_pOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
    _pOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (_pTexture) {
		SDL_Rect r;
		// Scale the width/height by owner's scale
		r.w = static_cast<int>(_nTexWidth * _pOwner->GetScale());
		r.h = static_cast<int>(_nTexHeight * _pOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(_pOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(_pOwner->GetPosition().y - r.h / 2);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
			_pTexture,
			nullptr,
			&r,
			-Math::ToDegrees(_pOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	_pTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &_nTexWidth, &_nTexHeight);
}
