#pragma once
#include "cmp_electro_ball.h"
#include "../engine.h"
#include "ecm.h"
#include "cmp_physics.h"

using namespace std;
using namespace sf;

void ElectroBallComponent::update(double dt) {
    if (_parent->isVisible()) {
        switch (_state)
        {
        case SEARCHING:
            for (auto& gs : _generators) {
                if (length(gs->getPosition() - _parent->getPosition()) < 40.0f) {
                    _state = WAITING;
                }
            }
            if (_state == WAITING) {
                for (auto& d : _parent->scene->ents.find("door")) {
                    d->GetCompatibleComponent<PhysicsComponent>()[0]->makeInactive();
                }
            }
            break;
        case WAITING:
            bool isWithinRange = false;
            for (auto& gs : _generators) {
                if (length(gs->getPosition() - _parent->getPosition()) < 40.0f) {
                    isWithinRange = true;
                }
            }
            if (!isWithinRange) {
                _state = SEARCHING;
                for (auto& d : _parent->scene->ents.find("door")) {
                    d->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
                }
            }
            break;
        }
    }
}

string ElectroBallComponent::getState() {
    string state = "";
    switch (_state)
    {
    case SEARCHING:
        state = "SEARCHING";
        break;
    case WAITING:
        state = "WAITING";
        break;
    default:
        break;
    }
    return state;
}

void ElectroBallComponent::setState(string state) {
    if (state == "WAITING") {
        _state = WAITING;
    }
    else {
        _state = SEARCHING;
    }
}

void ElectroBallComponent::inactivePrep() {
    for (auto& d : _parent->scene->ents.find("door")) {
        d->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
    }
    _state = SEARCHING;
}

ElectroBallComponent::ElectroBallComponent(Entity* p) : Component(p) {
    _state = SEARCHING;
    for (auto& gs : _parent->scene->ents.find("generator")) {
        _generators.push_back(gs);
    }
}