// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* pGame)
	:_state(EActive)
	, _position(Vector2::Zero)
	, _fScale(1.0f)
	, _fRotation(0.0f)
	, _pGame(pGame) {
	_pGame->AddActor(this);
}

Actor::~Actor() {
	_pGame->RemoveActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!_vecComponents.empty()) {
		delete _vecComponents.back();
	}
}

void Actor::Update(float deltaTime) {
	if (_state == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : _vecComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime) {
}

void Actor::ProcessInput(const uint8_t* keyState) {
	if (_state == EActive) {
		// First process input for components
		for (auto comp : _vecComponents) {
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState) {
}

void Actor::AddComponent(Component* component) {
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = _vecComponents.begin();
	for (;
		iter != _vecComponents.end();
		++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// Inserts element before position of iterator
	_vecComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(_vecComponents.begin(), _vecComponents.end(), component);
	if (iter != _vecComponents.end()) {
        _vecComponents.erase(iter);
	}
}
