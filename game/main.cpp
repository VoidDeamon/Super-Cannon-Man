#include "../engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include <filesystem>
#include "Shlobj_core.h"
#include <windows.h>
#include "Fileapi.h"
#include <iostream>
#include <fstream>

using namespace std;

MenuScene menu;
Level1Scene level1;

int main() {
    char* appdata = getenv("APPDATA");
    string path = "/Super Cannon";
    path = "/Super Cannon/preference.txt";
    ifstream file(appdata + path);
    string line;
    int resx = NULL;
    int resy = NULL;
    while (getline(file, line)) {
        std::vector<std::string> arguments;
        stringstream s_stream(line);
        while (s_stream.good())
        {
            string word;
            getline(s_stream, word, ' ');
            arguments.push_back(word);
        }
        resx = std::stoi(arguments[0]);
        resy = std::stoi(arguments[1]);
    }
    if (resx == NULL || resy == NULL) {
        Engine::Start(1920, 1080, "Super_Cannon", &menu);
    }
    else {
        Engine::Start(resx, resy, "Super_Cannon", &menu);
    }
}