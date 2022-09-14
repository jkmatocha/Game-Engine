// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,_fAngularSpeed(0.0f)
,_fForwardSpeed(0.0f) {
	
}

void MoveComponent::Update(float deltaTime) {
	if (!Math::NearZero(_fAngularSpeed)) {
		float rot = _pOwner->GetRotation();
		rot += _fAngularSpeed * deltaTime;
        _pOwner->SetRotation(rot);
	}
	
	if (!Math::NearZero(_fForwardSpeed)) {
		Vector2 pos = _pOwner->GetPosition();
		pos += _pOwner->GetForward() * _fForwardSpeed * deltaTime;
        _pOwner->SetPosition(pos);
	}
}
