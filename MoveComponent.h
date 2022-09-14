// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"

class MoveComponent : public Component {
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return _fAngularSpeed; }
	float GetForwardSpeed() const { return _fForwardSpeed; }
	void SetAngularSpeed(float speed) { _fAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { _fForwardSpeed = speed; }
private:
	float _fAngularSpeed;
	float _fForwardSpeed;
};
