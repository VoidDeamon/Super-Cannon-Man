#include "scene_level1.h"
#include "../components/cmp_camera.h"
#include "../components/cmp_gun.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../system_physics.h"
#include "../components/cmp_UI.h"
#include "../components/cmp_menu.h"
#include <filesystem>
#include "Shlobj_core.h"
#include <windows.h>
#include "Fileapi.h"
#include <fstream>
#include "../components/cmp_electro_ball.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/level_1.txt", 40.0f);

    // Create player
    {
        player = makeEntity();
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(10.f, 15.f);
        player->addTag("player");
        player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
        auto doors = ls::findTiles(ls::DOOR);
        for (auto d : doors) {
            auto pos = ls::getTilePosition(d);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->addTag("door");
            e->setPosition(pos);

            auto s = e->addComponent<ShapeComponent>();
            s->setShape<sf::RectangleShape>(Vector2f(10.0f, 40.0f));
            s->getShape().setFillColor(Color::Red);
            s->getShape().setFillColor(Color::Green);
            s->getShape().setOrigin(5.f, 20.f);
            e->addComponent<PhysicsComponent>(false, Vector2f(10.f, 40.f));
        }
        auto generators = ls::findTiles(ls::GENERATOR);
        for (auto g : generators) {
            auto pos = ls::getTilePosition(g);
            pos += Vector2f(20.f, 20.f); //offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addTag("generator");
        }
    }

    //create gun
    //make sure to create gun component after generators etc so that components work
    {
        auto gun = makeEntity();
        gun->setPosition(player->getPosition());
        auto g = gun->addComponent<ShapeComponent>();
        g->setShape<sf::RectangleShape>(Vector2f(10.f, 20.f));
        g->getShape().setFillColor(Color::Blue);
        g->getShape().setOrigin(5.f, 9.f);
        gun->addTag("gun");

        gun->addComponent<GunComponent>();
    }

    auto camera = makeEntity();
    camera->addComponent<CameraComponent>();

    auto ui = makeEntity();
    ui->addComponent<UIComponent>();

    auto menu = makeEntity();
    menu->addComponent<MenuComponent>();

    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    if (Engine::getLoadingFromFile()) {
        Engine::setLoadingFromFile(false);
        char* appdata = getenv("APPDATA");
        string path = "/Super Cannon";
        path = "/Super Cannon/save.txt";
        ifstream file (appdata + path);
        string line;
        while (getline(file, line)) {
            std::vector<std::string> arguments;
            stringstream s_stream(line);
            while (s_stream.good())
            {
                string word;
                getline(s_stream, word, ' ');
                arguments.push_back(word);
            }
            if (arguments[0] == "player") {
                cout << arguments[0] << "-" << endl;
                ents.find("player")[0]->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->teleport(Vector2f(std::stof(arguments[1]), std::stof(arguments[2])));
                ents.find("player")[0]->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setVelocity(Vector2f(std::stof(arguments[3]), std::stof(arguments[4])));
            }
            if (arguments[0] == "gun") {
                cout << arguments[0] << "-" << endl;
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->setfireForceMod(std::stof(arguments[1]));
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->setCurrentBall(std::stoi(arguments[2]));
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->setState(arguments[3]);
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->setBallPoolCurrent(std::stoi(arguments[4]));
            }
            if (arguments[0] == "ball") {
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getBallPool()[std::stoi(arguments[1])]->GetCompatibleComponent<PhysicsComponent>()[0]->teleport(Vector2f(std::stof(arguments[2]), std::stof(arguments[3])));
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getBallPool()[std::stoi(arguments[1])]->GetCompatibleComponent<PhysicsComponent>()[0]->setVelocity(Vector2f(std::stof(arguments[4]), std::stof(arguments[5])));
                if (std::stoi(arguments[6]) == 0) {
                    ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getBallPool()[std::stoi(arguments[1])]->GetCompatibleComponent<PhysicsComponent>()[0]->makeInactive();
                }
                else {
                    ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getBallPool()[std::stoi(arguments[1])]->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
                }
            }
            if (arguments[0] == "electroball") {
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getElectroBall()->GetCompatibleComponent<PhysicsComponent>()[0]->teleport(Vector2f(std::stof(arguments[1]), std::stof(arguments[2])));
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getElectroBall()->GetCompatibleComponent<PhysicsComponent>()[0]->setVelocity(Vector2f(std::stof(arguments[3]), std::stof(arguments[4])));
                if (std::stoi(arguments[5]) == 0) {
                    ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getElectroBall()->GetCompatibleComponent<PhysicsComponent>()[0]->makeInactive();
                }
                else {
                    ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getElectroBall()->GetCompatibleComponent<PhysicsComponent>()[0]->makeActive();
                }
                ents.find("gun")[0]->GetCompatibleComponent<GunComponent>()[0]->getElectroBall()->GetCompatibleComponent<ElectroBallComponent>()[0]->setState(arguments[6]);
            }
        }
    }
    setLoaded(true);
}

void Level1Scene::UnLoad() {
    cout << "Scene 1 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&menu);
    }
    Physics::update(dt);
    Scene::Update(dt);
}

void Level1Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}