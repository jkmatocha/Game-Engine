// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "NavComponent.h"
#include "Tile.h"

NavComponent::NavComponent(class Actor* owner, int updateOrder)
:MoveComponent(owner, updateOrder)
,_pNextNode(nullptr) {
	
}

void NavComponent::Update(float deltaTime) {
	if (_pNextNode) {
		// If we're at the next node, advance along path
		Vector2 diff = _pOwner->GetPosition() - _pNextNode->GetPosition();
		if (Math::NearZero(diff.Length(), 2.0f))
		{
            _pNextNode = _pNextNode->GetNext();
			TurnTo(_pNextNode->GetPosition());
		}
	}
	
	MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start) {
	_pNextNode = start->GetNext();
	TurnTo(_pNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos) {
	// Vector from me to pos
	Vector2 dir = pos - _pOwner->GetPosition();
	// New angle is just atan2 of this dir vector
	// (Negate y because +y is down on screen)
	float angle = Math::Atan2(-dir.y, dir.x);
    _pOwner->SetRotation(angle);
}
