#pragma once
#include "cmp_mainMenu.h"
#include "../engine.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "cmp_electro_ball.h"
#include <SFML/Graphics.hpp>
#include "cmp_gun.h"
#include "cmp_text.h"
#include "../game.h"
#include "cmp_mainMenu.h"
#include <filesystem>
#include "Shlobj_core.h"
#include <windows.h>
#include "Fileapi.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

void MainMenuComponent::update(double dt) {
    _selectionCooldown += dt;
    switch (_optionsState) {
    case INACTIVE:
        switch (_state) {
        case WAITING:
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                _selectedMenuOption += 1;
                if (_selectedMenuOption > 3) {
                    _selectedMenuOption = 0;
                }
                _state = PRESSED;
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                _selectedMenuOption -= 1;
                if (_selectedMenuOption < 0) {
                    _selectedMenuOption = 3;
                }
                _state = PRESSED;
            }
            break;
        case PRESSED:
            if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W)) {
            }
            else {
                _state = WAITING;
            }
            break;
        }
        switch (_selectedMenuOption) {
        case 0:
            _selectionIndicator->setPosition(Vector2f(_x8Scale, _y8Scale * 2.0f + 5.0f));
            if (sf::Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                Engine::ChangeScene(&level1);
            }
            break;
        case 1:
            _selectionIndicator->setPosition(Vector2f(_x8Scale, _y8Scale * 3.0f + 5.0f));
            if (sf::Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                Engine::setLoadingFromFile(true);
                Engine::ChangeScene(&level1);
            }
            break;
        case 2:
            _selectionIndicator->setPosition(Vector2f(_x8Scale, _y8Scale * 4.0f + 5.0f));
            if (sf::Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                _optionsState = ACTIVE;
            }
            break;
        case 3:
            _selectionIndicator->setPosition(Vector2f(_x8Scale, _y8Scale * 5.0f + 5.0f));
            if (sf::Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                Engine::GetWindow().close();
            }
            break;
        }
        break;
    case ACTIVE:
        _optionsMenuResolutionText->setVisible(true);
        _optionsMenuApplyText->setVisible(true);
        _optionsMenuBackText->setVisible(true);
        switch (_state) {
        case WAITING:
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                _selectedSubMenuOption += 1;
                if (_selectedSubMenuOption > 2) {
                    _selectedSubMenuOption = 0;
                }
                _state = PRESSED;
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                _selectedSubMenuOption -= 1;
                if (_selectedMenuOption < 0) {
                    _selectedMenuOption = 2;
                }
                _state = PRESSED;
            }
            break;
        case PRESSED:
            if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::W)) {

            }
            else {
                _state = WAITING;
            }
            break;
        }
        int resx;
        int resy;
        switch (_resolutionState) {
        case 0:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1024x576 -E");
            resx = 1024;
            resy = 576;
            break;
        case 1:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1152x648 -E");
            resx = 1152;
            resy = 648;
            break;
        case 2:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1280x720 -E");
            resx = 1280;
            resy = 720;
            break;
        case 3:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1366x768 -E");
            resx = 1366;
            resy = 768;
            break;
        case 4:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1600x900 -E");
            resx = 1600;
            resy = 900;
            break;
        case 5:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 1920x1080 -E");
            resx = 1920;
            resy = 1080;
            break;
        case 6:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 2560x1440 -E");
            resx = 2560;
            resy = 1440;
            break;
        case 7:
            _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- 3840x2160 -E");
            resx = 3480;
            resy = 2160;
            break;
        }
        switch (_selectedSubMenuOption) {
        case 0:
            _selectionIndicator->setPosition(Vector2f(_x8Scale * 4, _y8Scale * 3));
            if (Keyboard::isKeyPressed(Keyboard::Q) && _selectionCooldown > 0.5f) {
                _resolutionState += 1;
                if (_resolutionState > 7) {
                    _resolutionState = 0;
                }
                _selectionCooldown = 0.0f;
            }
            if (Keyboard::isKeyPressed(Keyboard::E) && _selectionCooldown > 0.5f) {
                _resolutionState -= 1;
                if (_resolutionState < 0) {
                    _resolutionState = 7;
                }
                _selectionCooldown = 0.0f;
            }
            break;
        case 1:
            _selectionIndicator->setPosition(Vector2f(_x8Scale * 4, _y8Scale * 4));
            if (Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                Engine::GetWindow().setSize(Vector2u(resx, resy));
                _selectionCooldown = 0.0f;

                char* appdata = getenv("APPDATA");
                string path = "/Super Cannon";
                CreateDirectory(string(appdata + path).c_str(), NULL);
                path = "/Super Cannon/preference.txt";
                ofstream file;
                file.open(appdata + path, ios::out);
                file << resx << " " << resy << endl;
                file.close();
            }
            break;
        case 2:
            _selectionIndicator->setPosition(Vector2f(_x8Scale * 4, _y8Scale * 5));
            if (Keyboard::isKeyPressed(Keyboard::Enter) && _selectionCooldown > 0.5f) {
                _optionsState = INACTIVE;
                _selectedSubMenuOption = 0;
                _selectionCooldown = 0.0f;
            }
            break;
        }
        break;
    }
}

MainMenuComponent::MainMenuComponent(Entity* p) : Component(p) {
    _selectedMenuOption = 0;
    {
        _selectionIndicator = _parent->scene->makeEntity();
        auto s = _selectionIndicator->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(90.f, 30.f));
        s->getShape().setFillColor(Color::White);
    }
    _state = WAITING;
    _x8Scale = Engine::GetWindow().getSize().x / 8.0f;
    _y8Scale = Engine::GetWindow().getSize().y / 8.0f;
    _selectionCooldown = 0.0f;
    _optionsState = INACTIVE;
    {
        _optionsMenuResolutionText = _parent->scene->makeEntity();
        _optionsMenuResolutionText->addComponent<TextComponent>();
        _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- " + std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) + " -E");
        _optionsMenuResolutionText->setPosition(Vector2f(_x8Scale * 5, _y8Scale * 3));
        _optionsMenuResolutionText->setVisible(false);
    }
    {
        _optionsMenuApplyText = _parent->scene->makeEntity();
        _optionsMenuApplyText->addComponent<TextComponent>();
        _optionsMenuApplyText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _optionsMenuApplyText->GetCompatibleComponent<TextComponent>()[0]->SetText("Apply");
        _optionsMenuApplyText->setPosition(Vector2f(_x8Scale * 5, _y8Scale * 4));
        _optionsMenuApplyText->setVisible(false);
    }
    {
        _optionsMenuBackText = _parent->scene->makeEntity();
        _optionsMenuBackText->addComponent<TextComponent>();
        _optionsMenuBackText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _optionsMenuBackText->GetCompatibleComponent<TextComponent>()[0]->SetText("Back");
        _optionsMenuBackText->setPosition(Vector2f(_x8Scale * 5, _y8Scale * 5));
        _optionsMenuBackText->setVisible(false);
    }
    _optionsState = INACTIVE;
    _selectedSubMenuOption = 0;
    if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1024x576") {
        _resolutionState = 0;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1152x648") {
        _resolutionState = 1;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1280x720") {
        _resolutionState = 2;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1366x768") {
        _resolutionState = 3;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1600x900") {
        _resolutionState = 4;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "1920x1080") {
        _resolutionState = 5;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "2560x1440") {
        _resolutionState = 6;
    }
    else if (std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) == "3840x2160") {
        _resolutionState = 7;
    }
}