#pragma once
#include "cmp_UI.h"
#include "../engine.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "cmp_electro_ball.h"
#include <SFML/Graphics.hpp>
#include "cmp_gun.h"
#include "cmp_text.h"

using namespace std;
using namespace sf;

void UIComponent::update(double dt) {
    if (!Engine::isPaused()) {
        _currentBallSceen->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(300.0f, -70.0f));
        _currentBall->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(255.0f, -110.0f));
        _currentBallText->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(220.0f, -85.0f));
        switch (_gun.lock()->GetCompatibleComponent<GunComponent>()[0]->getCurrentBall()) {
        case 0:
        {
            _currentBall->GetCompatibleComponent<ShapeComponent>()[0]->getShape().setFillColor(Color::White);
            _currentBallText->GetCompatibleComponent<TextComponent>()[0]->SetText("Standard-Shot\nUsed to fight enemies");
        }
        break;
        case 1:
        {
            _currentBall->GetCompatibleComponent<ShapeComponent>()[0]->getShape().setFillColor(Color::Cyan);
            _currentBallText->GetCompatibleComponent<TextComponent>()[0]->SetText("Electro-Ball\nUsed to activate generators");
        }
        }
        _forceBarBackdrop->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(200.0f, -50.0f));
        auto mod = _gun.lock()->GetCompatibleComponent<GunComponent>()[0]->getfireForceMod();
        auto locationMod = 190.0f * mod;
        _forceBar->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(210.0f, 150.0f - locationMod));
        _forceBar->GetCompatibleComponent<ShapeComponent>()[0]->getShape().setScale(Vector2f(1.f, 1.f * mod));
    }
}

UIComponent::UIComponent(Entity* p) : Component(p), _player(_parent->scene->ents.find("player")[0]), _gun(_parent->scene->ents.find("gun")[0]) {
    {
        _currentBallSceen = _parent->scene->makeEntity();
        _currentBallSceen->setPosition(Engine::GetWindow().getView().getCenter() - Vector2f(290.0f, -85.0f));
        auto s = _currentBallSceen->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(440.f, 80.f));
        s->getShape().setFillColor(Color::Color(150.0f, 75.0f, 0.0f, 255.0f));
    }
    {
        _forceBarBackdrop = _parent->scene->makeEntity();
        _forceBarBackdrop->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(200.0f, -50.0f));
        auto s = _forceBarBackdrop->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(100.f, 200.f));
        s->getShape().setFillColor(Color::Color(150.0f, 75.0f, 0.0f, 255.0f));
    }
    {
        _forceBar = _parent->scene->makeEntity();
        _forceBar->setPosition(Engine::GetWindow().getView().getCenter() + Vector2f(210.0f, -40.0f));
        auto s = _forceBar->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(80.f, 190.f));
        s->getShape().setFillColor(Color::Blue);
    }
    {
        _currentBall = _parent->scene->makeEntity();
        _currentBall->setPosition(Vector2f(-1000.0f, 0.0f));
        auto s = _currentBall->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(25.f);
        s->getShape().setFillColor(Color::White);
        s->getShape().setOrigin(25.f, 25.f);
    }
    {
        _currentBallText = _parent->scene->makeEntity();
        _currentBallText->addComponent<TextComponent>();
        _currentBallText->GetCompatibleComponent<TextComponent>()[0]->SetFontSize(20.0f);
    }
}