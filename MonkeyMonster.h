#pragma once

#include <vector>
#include "Actor.h"

class Game;
class MoveComponent;
class MeshComponent;

class MonkeyMonster : public Actor
{
public:
	MonkeyMonster(::Game* game);
	MonkeyMonster(const MonkeyMonster&) = delete;
	~MonkeyMonster() = default;
	void UpdateActor(float deltaTime) override;
	void FixCollisions();

	bool Update(float dt);
protected:
	float rotationSpeed_;
	float rotationAngle_;
	MoveComponent* mMoveComp;
	class BoxComponent* mBoxComp;
	std::vector<MeshComponent*> meshComponents_;
};
