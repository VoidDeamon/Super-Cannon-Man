#pragma once
#include <ecm.h>
#include "cmp_physics.h"

class MainMenuComponent : public Component {
protected:
	int _selectedMenuOption;
	std::shared_ptr<Entity> _selectionIndicator;
	std::shared_ptr<Entity> _optionsMenuResolutionText;
	std::shared_ptr<Entity> _optionsMenuApplyText;
	std::shared_ptr<Entity> _optionsMenuBackText;
	int _selectedSubMenuOption;
	enum state { WAITING, PRESSED };
	state _state;
	enum optionsState { INACTIVE, ACTIVE };
	optionsState _optionsState;
	float _x8Scale;
	float _y8Scale;
	float _selectionCooldown;
	int _resolutionState;
public:
	void update(double dt) override;
	void render() override {}
	explicit MainMenuComponent(Entity* p);
	MainMenuComponent() = delete;
};