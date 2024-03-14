#pragma once
#include <ecm.h>
#include "cmp_physics.h"

class MenuComponent : public Component {
protected:
	std::shared_ptr<Entity> _menuBackdrop;
	std::shared_ptr<Entity> _menuText;
	std::shared_ptr<Entity> _optionsMenuBackdrop;
	std::shared_ptr<Entity> _optionsMenuResolutionText;
	std::shared_ptr<Entity> _optionsMenuText;
	int _selectedMenuOption;
	int _selectedSubMenuOption;
	std::shared_ptr<Entity> _selectionIndicator;
	enum state { WAITING, PRESSED };
	state _state;
	enum menuState { INACTIVE, ACTIVE };
	menuState _subMenuState;
	int _resolutionState;
	bool check;
	float cooldown;
public:
	void update(double dt) override;
	void render() override {}
	explicit MenuComponent(Entity* p);
	MenuComponent() = delete;
};