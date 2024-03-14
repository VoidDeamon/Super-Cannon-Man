#pragma once
#include "cmp_menu.h"
#include "../engine.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "cmp_electro_ball.h"
#include <SFML/Graphics.hpp>
#include "cmp_gun.h"
#include "cmp_text.h"
#include "../game.h"
#include <filesystem>
#include "Shlobj_core.h"
#include <windows.h>
#include "Fileapi.h"
#include <iostream>
#include <fstream>
#include "cmp_player_physics.h"


using namespace std;
using namespace sf;

void MenuComponent::update(double dt) {
    cooldown += 1.0f;
    if (Engine::isPaused()) {
        _menuBackdrop->setPosition(Engine::GetWindow().getView().getCenter());
        _menuText->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(100.0f, 75.0f));
        switch (_subMenuState) {
        case INACTIVE:
            switch (_state) {
            case WAITING:
                if (Keyboard::isKeyPressed(Keyboard::S)) {
                    _selectedMenuOption += 1;
                    if (_selectedMenuOption > 2) {
                        _selectedMenuOption = 0;
                    }
                    _state = PRESSED;
                }
                if (Keyboard::isKeyPressed(Keyboard::W)) {
                    _selectedMenuOption -= 1;
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
            switch (_selectedMenuOption) {
            case 0:
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(40.0f, -60.0f));
                if (Keyboard::isKeyPressed(Keyboard::Enter) && cooldown > 100.0f) {
                    _subMenuState = ACTIVE;
                    cooldown = 0.0f;
                }
                break;
            case 1:
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(40.0f, -15.0f));
                if (Keyboard::isKeyPressed(Keyboard::Enter) && cooldown > 100.0f) {
                    cooldown = 0.0f;
                    char* appdata = getenv("APPDATA");
                    string path = "/Super Cannon";
                    CreateDirectory(string(appdata + path).c_str(), NULL);
                    path = "/Super Cannon/save.txt";
                    ofstream file;
                    file.open(appdata + path, ios::out);
                    auto player = _parent->scene->ents.find("player")[0];
                    file << "player " << player->getPosition().x << " " << player->getPosition().y << " " << player->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->getVelocity().x << " " << player->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->getVelocity().y << endl;
                    auto gun = _parent->scene->ents.find("gun")[0];
                    file << "gun " << gun->GetCompatibleComponent<GunComponent>()[0]->getfireForceMod() << " " << gun->GetCompatibleComponent<GunComponent>()[0]->getCurrentBall() << " " << gun->GetCompatibleComponent<GunComponent>()[0]->getState() << " " << gun->GetCompatibleComponent<GunComponent>()[0]->getBallPoolCurrent() << endl;
                    auto ballPool = gun->GetCompatibleComponent<GunComponent>()[0]->getBallPool();
                    int ballnumber = 0;
                    for (auto& b : ballPool) {
                        file << "ball " << ballnumber << " " << b->getPosition().x << " " << b->getPosition().y << " " << b->GetCompatibleComponent<PhysicsComponent>()[0]->getVelocity().x << " " << b->GetCompatibleComponent<PhysicsComponent>()[0]->getVelocity().y << " " << b->GetCompatibleComponent<PhysicsComponent>()[0]->getActive() << endl;
                        ballnumber++;
                    }
                    auto electroBall = gun->GetCompatibleComponent<GunComponent>()[0]->getElectroBall();
                    file << "electroball " << electroBall->getPosition().x << " " << electroBall->getPosition().y << " " << electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->getVelocity().x << " " << electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->getVelocity().y << " " << electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->getActive() << " " << electroBall->GetCompatibleComponent<ElectroBallComponent>()[0]->getState() << endl;
                    file.close();
                }
                break;
            case 2:
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(40.0f, 30.0f));
                if (Keyboard::isKeyPressed(Keyboard::Enter) && cooldown > 100.0f) {
                    cooldown = 0.0f;
                    Engine::SetPauseState(false);
                    Engine::ChangeScene((Scene*)&menu);
                }
                break;
            }
            break;
        case ACTIVE:
            _optionsMenuBackdrop->setPosition(Engine::GetWindow().getView().getCenter());
            _optionsMenuResolutionText->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(50.0f, 95.0f));
            _optionsMenuText->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(50.0f, 55.0f));
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
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(100.0f, 85.0f));
                if (Keyboard::isKeyPressed(Keyboard::Q) && check) {
                    _resolutionState += 1;
                    if (_resolutionState > 7) {
                        _resolutionState = 0;
                    }
                    check = false;
                }
                if (Keyboard::isKeyPressed(Keyboard::E) && check) {
                    _resolutionState -= 1;
                    if (_resolutionState < 0) {
                        _resolutionState = 7;
                    }
                    check = false;
                }
                if (!Keyboard::isKeyPressed(Keyboard::Q) && !Keyboard::isKeyPressed(Keyboard::E)) {
                    check = true;
                }
                break;
            case 1:
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(100.0f, 40.0f));
                if (Keyboard::isKeyPressed(Keyboard::Enter) && cooldown > 100.0f) {
                    Engine::GetWindow().setSize(Vector2u(resx,resy));
                    char* appdata = getenv("APPDATA");
                    string path = "/Super Cannon";
                    CreateDirectory(string(appdata + path).c_str(), NULL);
                    path = "/Super Cannon/preference.txt";
                    ofstream file;
                    file.open(appdata + path, ios::out);
                    file << resx << " " << resy << endl;
                    file.close();
                    cooldown = 0.0f;
                }
                break;
            case 2:
                _selectionIndicator->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(100.0f, -5.0f));
                if (Keyboard::isKeyPressed(Keyboard::Enter) && cooldown > 100.0f) {
                    _subMenuState = INACTIVE;
                    _optionsMenuBackdrop->setPosition(Vector2f(-2000.0f, 0.0f));
                    _optionsMenuResolutionText->setPosition(Vector2f(-2000.0f, 0.0f));
                    _optionsMenuText->setPosition(Vector2f(-2000.0f, 0.0f));
                    cooldown = 0.0f;
                }
                break;
            }
            break;
        }
    }
    else {
        _menuBackdrop->setPosition(Vector2f(-2000.0f, 0.0f));
        _menuText->setPosition(Vector2f(-2000.0f, 0.0f));
        _selectionIndicator->setPosition(Vector2f(-2000.0f, 0.0f));
        _optionsMenuBackdrop->setPosition(Vector2f(-2000.0f, 0.0f));
        _optionsMenuResolutionText->setPosition(Vector2f(-2000.0f, 0.0f));
        _optionsMenuText->setPosition(Vector2f(-2000.0f, 0.0f));
    }
}

MenuComponent::MenuComponent(Entity* p) : Component(p) {
    {
        _menuBackdrop = _parent->scene->makeEntity();
        auto s = _menuBackdrop->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(250.f, 200.f));
        s->getShape().setOrigin(125.0f, 100.0f);
        s->getShape().setFillColor(Color::Color(128.0f, 128.0f, 128.0f, 255.0f));
        s->getShape().setOutlineThickness(-10.0f);
        s->getShape().setOutlineColor(Color::Color(105.0f, 105.0f, 105.0f, 255.0f));
    }
    {
        _menuText = _parent->scene->makeEntity();
        _menuText->addComponent<TextComponent>();
        _menuText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _menuText->GetCompatibleComponent<TextComponent>()[0]->SetText("Options\n\nSave\n\nExit");
    }
    _selectedMenuOption = 0;
    _state = WAITING;
    {
        _optionsMenuBackdrop = _parent->scene->makeEntity();
        auto s = _optionsMenuBackdrop->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(300.f, 250.f));
        s->getShape().setOrigin(150.0f, 125.0f);
        s->getShape().setFillColor(Color::Color(128.0f, 128.0f, 128.0f, 255.0f));
        s->getShape().setOutlineThickness(-10.0f);
        s->getShape().setOutlineColor(Color::Color(105.0f, 105.0f, 105.0f, 255.0f));
    }
    {
        _optionsMenuResolutionText = _parent->scene->makeEntity();
        _optionsMenuResolutionText->addComponent<TextComponent>();
        _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _optionsMenuResolutionText->GetCompatibleComponent<TextComponent>()[0]->SetText("Q- " + std::to_string(Engine::GetWindow().getSize().x) + "x" + std::to_string(Engine::GetWindow().getSize().y) + " -E");
    }
    {
        _optionsMenuText = _parent->scene->makeEntity();
        _optionsMenuText->addComponent<TextComponent>();
        _optionsMenuText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
        _optionsMenuText->GetCompatibleComponent<TextComponent>()[0]->SetText("Apply\n\nBack");
    }
    _subMenuState = INACTIVE;
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
    {
        _selectionIndicator = _parent->scene->makeEntity();
        auto s = _selectionIndicator->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(40.f, 10.f));
        s->getShape().setOrigin(20.0f, 5.0f);
        s->getShape().setFillColor(Color::White);
    }
    check = true;
    cooldown = 100.0f;
}