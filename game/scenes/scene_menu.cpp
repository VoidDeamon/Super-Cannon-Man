#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../components/cmp_mainMenu.h"

using namespace std;
using namespace sf;

void MenuScene::Load() {
    cout << "Menu Load \n";
    Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());
    {
        auto y8Scale = Engine::GetWindow().getSize().y / 8.0f;
        auto x8Scale = Engine::GetWindow().getSize().x / 8.0f;
        {
            auto title = makeEntity();
            auto t = title->addComponent<TextComponent>("Super Cannon");
            t->SetFontSize(60.0f);
            title->setPosition(Vector2f(x8Scale, y8Scale));
        }
        {
            auto gameStart = makeEntity();
            auto t = gameStart->addComponent<TextComponent>("Start Game");
            t->SetFontSize(30.0f);
            gameStart->setPosition(Vector2f(x8Scale * 2.0f, y8Scale * 2.0f));
        }
        {
            auto gameLoad = makeEntity();
            auto t = gameLoad->addComponent<TextComponent>("Load Game");
            t->SetFontSize(30.0f);
            gameLoad->setPosition(Vector2f(x8Scale * 2.0f, y8Scale * 3.0f));
        }
        {
            auto options = makeEntity();
            auto t = options->addComponent<TextComponent>("Options");
            t->SetFontSize(30.0f);
            options->setPosition(Vector2f(x8Scale * 2.0f, y8Scale * 4.0f));
        }
        {
            auto gameQuit = makeEntity();
            auto t = gameQuit->addComponent<TextComponent>("Quit Game");
            t->SetFontSize(30.0f);
            gameQuit->setPosition(Vector2f(x8Scale * 2.0f, y8Scale * 5.0f));
        }
        auto selecter = makeEntity();
        selecter->addComponent<MainMenuComponent>();
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    Scene::Update(dt);
}