#pragma once
#include "cmp_gun.h"
#include "../engine.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include "cmp_electro_ball.h"
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

void GunComponent::fire(double dt, Vector2f offset)
{
    switch (_currentBall) {
        case 0:
        {
            sound.play();

            auto ball = getBall();
            ball->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
            ball->GetCompatibleComponent<PhysicsComponent>()[0]->teleport(_parent->getPosition() + offset);
            ball->GetCompatibleComponent<PhysicsComponent>()[0]->setVelocity(Vector2f(0.0f, 0.0f));
            ball->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(sf::rotate(Vector2f(0, 50.0f * _fireForceMod), _parent->getRotation()));
        }
        break;
        case 1:
        {
            if (!_electroBall->isVisible()) {
                sound.play();
                _electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
                _electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->teleport(_parent->getPosition() + offset);
                _electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->setVelocity(Vector2f(0.0f, 0.0f));
                _electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->impulse(sf::rotate(Vector2f(0, 50.0f * _fireForceMod), _parent->getRotation()));
            }
            else {
                //ensure the generators know the ball is no longer active to avoid bugs
                _electroBall->GetCompatibleComponent<ElectroBallComponent>()[0]->inactivePrep();
                //deactivate the ball
                _electroBall->GetCompatibleComponent<PhysicsComponent>()[0]->makeInactive();
            }
        }
        break;
    default:
        break;
    }
}

void GunComponent::update(double dt) {
    if (!Engine::isPaused()) {
        Vector2i pixelPos = Mouse::getPosition(Engine::GetWindow());
        Vector2f mousePos = Engine::GetWindow().mapPixelToCoords(pixelPos);
        auto playerPos = _player.lock()->getPosition();
        if (mousePos.x - playerPos.x < 0.0f) {
            _parent->setPosition(Vector2f(playerPos.x - 10.0f, playerPos.y));
        }
        else {
            _parent->setPosition(Vector2f(playerPos.x + 10.0f, playerPos.y));
        }
        auto offset = normalize(mousePos - _parent->getPosition()) * 10.0f;
        auto gunAngle = -atan2(mousePos.x - _parent->getPosition().x, mousePos.y - _parent->getPosition().y) * 180.0f / 3.14159f;
        _parent->setRotation(gunAngle);

        _swapCooldown += dt;
        if (_swapCooldown > 0.25f) {
            if (Keyboard::isKeyPressed(Keyboard::E)) {
                _currentBall += 1;
                if (_currentBall >= _balls.size()) {
                    _currentBall = 0;
                }
                _swapCooldown = 0.0f;
            }
            if (Keyboard::isKeyPressed(Keyboard::Q)) {
                _currentBall -= 1;
                if (_currentBall < 0) {
                    _currentBall = _balls.size() - 1.0f;
                }
                _swapCooldown = 0.0f;
            }
        }

        _fireCooldown += dt;
        if (_fireCooldown > 0.5f) {
            switch (_state) {
            case WAITING:
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    _state = FIRING;
                }
                break;
            case FIRING:
                if (!Mouse::isButtonPressed(Mouse::Left)) {
                    fire(dt, offset);
                    _state = WAITING;
                    _fireForceMod = 0.0f;
                    _fireCooldown = 0.0f;
                }
                else {
                    if (_fireForceMod < 1.0f) {
                        _fireForceMod += dt;
                        if (_fireForceMod > 1.0f) {
                            _fireForceMod = 1.0f;
                        }
                    }
                }
                break;
            }
        }
    }
    else {
        _state = WAITING;
        _fireForceMod = 0.0f;
    }
}

std::shared_ptr<Entity> GunComponent::getBall() {
    ++_ballPoolCurrent;
    if (_ballPoolCurrent == _ballCount) {
        _ballPoolCurrent = 0;
    }
    return _ballPool[_ballPoolCurrent];
}

float GunComponent::getfireForceMod() {
    if (_fireForceMod > 1.0f) {
        return 1.0f;
    }
    else {
        return _fireForceMod;
    }
}

void GunComponent::setfireForceMod(float fireForce) {
    _fireForceMod = fireForce;
}

int GunComponent::getCurrentBall() {
    return _currentBall;
}

void GunComponent::setCurrentBall(int currentBall) {
    _currentBall = currentBall;
}

string GunComponent::getState() {
    string state = "";
    switch (_state)
    {
    case WAITING:
        state = "WAITING";
        break;
    case FIRING:
        state = "FIRING";
        break;
    default:
        break;
    }
    return state;
}

void GunComponent::setState(string state) {
    if (state == "WAITING") {
        _state = WAITING;
    }
    else {
        _state = FIRING;
    }
}

int GunComponent::getBallPoolCurrent() {
    return _ballPoolCurrent;
}

void GunComponent::setBallPoolCurrent(int current) {
    _ballPoolCurrent = current;
}

std::vector<std::shared_ptr<Entity>> GunComponent::getBallPool() {
    return _ballPool;
}

std::shared_ptr<Entity> GunComponent::getElectroBall() {
    return _electroBall;
}

GunComponent::GunComponent(Entity* p) : Component(p), _player(_parent->scene->ents.find("player")[0]) {
    _fireForceMod = 0.0f;
    _swapCooldown = 1.0f;
    _fireCooldown = 1.0f;
    _balls.push_back("STANDARD");
    _balls.push_back("ELECTRO");
    _currentBall = 0;
    _state = WAITING;
    _ballPoolCurrent = 0;
    for (int i = 0; i < _ballCount; i++) {
        auto ball = _parent->scene->makeEntity();
        ball->setPosition(Vector2f(-1000.0f, 0.0f));
        auto t = ball->addComponent<ShapeComponent>();
        t->setShape<sf::CircleShape>(4.f);
        t->getShape().setFillColor(Color::White);
        t->getShape().setOrigin(4.f, 4.f);
        auto p = ball->addComponent<PhysicsComponent>(true, Vector2f(7.f, 7.f));
        p->setRestitution(.5f);
        p->setFriction(.005f);
        p->makeInactive();
        _ballPool.push_back(ball);
    }
    {
        _electroBall = _parent->scene->makeEntity();
        _electroBall->setPosition(Vector2f(-1000.0f, 0.0f));
        auto t = _electroBall->addComponent<ShapeComponent>();
        t->setShape<sf::CircleShape>(4.f);
        t->getShape().setFillColor(Color::Cyan);
        t->getShape().setOrigin(4.f, 4.f);
        auto p = _electroBall->addComponent<PhysicsComponent>(true, Vector2f(7.f, 7.f));
        p->setRestitution(.5f);
        p->setFriction(.005f);
        p->makeInactive();
        _electroBall->addComponent<ElectroBallComponent>();
    }
    if (!buffer.loadFromFile("res/sounds/laser4.ogg")) {
        cout << "ERROR: SOUND NOT LOADED" << endl;
    }
    sound.setBuffer(buffer);
    sound.setVolume(20);
}