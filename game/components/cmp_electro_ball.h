#pragma once
#include <ecm.h>

class ElectroBallComponent : public Component {
protected:
	enum state { SEARCHING, WAITING };
	state _state;
	std::vector<std::shared_ptr<Entity>> _generators;
public:
	void update(double dt) override;
	void render() override {}
	void inactivePrep();
	std::string getState();
	void setState(std::string state);
	explicit ElectroBallComponent(Entity* p);
	ElectroBallComponent() = delete;
};