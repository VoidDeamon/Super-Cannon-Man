#pragma once
#include <ecm.h>
#include "cmp_physics.h"

class UIComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;
	std::weak_ptr<Entity> _gun;
	std::shared_ptr<Entity> _forceBarBackdrop;
	std::shared_ptr<Entity> _forceBar;
	std::shared_ptr<Entity> _currentBallSceen;
	std::shared_ptr<Entity> _currentBall;
	std::shared_ptr<Entity> _currentBallText;
public:
	void update(double dt) override;
	void render() override {}
	explicit UIComponent(Entity* p);
	UIComponent() = delete;
};