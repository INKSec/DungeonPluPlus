#ifndef DUNGEONPLUSPLUS_GAMESCENE_H
#define DUNGEONPLUSPLUS_GAMESCENE_H

#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Button.h"
#include "Room.h"

namespace game {

    class GameScene {
    public:
        GameScene();
        void display(sf::RenderWindow &window, const Room &room);
        void explorationView(sf::RenderWindow &window);
        void combatView();
        void setBackground(const std::string &path);
        void setHUD(const std::string &path);
        void setFinished(const bool &_finished);
    private:
        std::vector<gui::Button> buttons;
        sf::Texture background;
        sf::Texture hud;
        sf::Font font;
        std::unique_ptr<Room> activeRoom;
        bool finished = false;
    };

}


#endif //DUNGEONPLUSPLUS_GAMESCENE_H
