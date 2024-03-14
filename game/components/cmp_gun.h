#pragma once
#include <ecm.h>
#include "cmp_physics.h"
#include <SFML/Audio.hpp>

class GunComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;
	void fire(double dt, sf::Vector2f offset);
	float _fireForceMod;
	float _swapCooldown;
	float _fireCooldown;
	std::vector<std::string> _balls;
	int _currentBall;
	enum state { WAITING, FIRING };
	state _state;
	std::vector<std::shared_ptr<Entity>> _ballPool;
	int _ballPoolCurrent;
	int _ballCount = 10;
	std::shared_ptr<Entity> getBall();
	std::shared_ptr<Entity> _electroBall;
	sf::SoundBuffer buffer;
	sf::Sound sound;
public:
	void update(double dt) override;
	void render() override {}
	float getfireForceMod();
	void setfireForceMod(float fireForce);
	int getCurrentBall();
	void setCurrentBall(int currentBall);
	std::string getState();
	void setState(std::string state);
	int getBallPoolCurrent();
	void setBallPoolCurrent(int current);
	std::vector<std::shared_ptr<Entity>> getBallPool();
	std::shared_ptr<Entity> getElectroBall();
	explicit GunComponent(Entity* p);
	GunComponent() = delete;
};