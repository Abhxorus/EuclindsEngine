#pragma once
#include "Actor.h"
#include "Utilities/CVector2.h"

class A_Racer : public Actor {
public:
	A_Racer(const std::string& name);

	void update(float deltaTime) override;

	void setPlace(int newPlace);
	int getPlace() const;

	void setPosition(const EngineMathLib::CVector2& pos);
	EngineMathLib::CVector2 getPosition() const;

	void setSpeed(float s);
	float getSpeed() const;

	void setTarget(const EngineMathLib::CVector2& waypoint);
	EngineMathLib::CVector2 getTarget() const;

	// STEERING BEHAVIOR
	void enableSteering(bool enabled);
	void setArriveRadius(float radius);

protected:
	int place = 0;
	float speed = 100.f;
	EngineMathLib::CVector2 logicPosition;
	EngineMathLib::CVector2 logicTarget;
	bool steeringEnabled = true;
	float arriveRadius = 10.f;
};