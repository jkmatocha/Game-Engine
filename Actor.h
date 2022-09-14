// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor {
public:
	enum State {
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/setters
	const Vector2& GetPosition() const { return _position; }
	void SetPosition(const Vector2& pos) { _position = pos; }
	float GetScale() const { return _fScale; }
	void SetScale(float scale) { _fScale = scale; }
	float GetRotation() const { return _fRotation; }
	void SetRotation(float rotation) { _fRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(_fRotation), -Math::Sin(_fRotation)); }

	State GetState() const { return _state; }
	void SetState(State state) { _state = state; }

	class Game* GetGame() { return _pGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// Actor's state
	State _state;

	// Transform
	Vector2 _position;
	float _fScale;
	float _fRotation;

	std::vector<class Component*> _vecComponents;
	class Game* _pGame;
};
