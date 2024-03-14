#include "cmp_camera.h"
#include "../engine.h"

using namespace std;
using namespace sf;

void CameraComponent::update(double dt) {
    _camera.setCenter(_player.lock()->getPosition());
    Engine::GetWindow().setView(_camera);
}

CameraComponent::CameraComponent(Entity* p) : Component(p), _player(_parent->scene->ents.find("player")[0]) {
    _camera.setCenter(_player.lock()->getPosition());
    _camera.setSize(Vector2f(600.0f, 300.0f));
}