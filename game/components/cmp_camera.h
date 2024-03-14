#pragma once
#include <ecm.h>
#include <SFML/Graphics/View.hpp>

using namespace sf;

class CameraComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;
	View _camera;
public:
	void update(double dt) override;
	void render() override {}
	explicit CameraComponent(Entity* p);
	CameraComponent() = delete;
};