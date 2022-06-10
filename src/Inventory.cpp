#include "Inventory.h"

#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40
#define BUTTON_COLOR sf::Color::White
#define EQUIPPED_COLOR sf::Color::Green
#define BUTTON_TEXT_COLOR sf::Color::Black
#define BUTTON_FONT_SIZE 14
#define INVENTORY_CAPACITY 8

gui::Inventory::Inventory(sf::RenderWindow &_window, std::shared_ptr<game::Player> &_player) : window{_window}, player{_player} {
    auto startingWeapon {game::ItemFactory::generateWeapon(game::ItemFactory::weaponType::Dagger)};
    putItem(startingWeapon);
    player->equipWeapon(startingWeapon);
}

void gui::Inventory::draw(GameScene &gameScene) {
    sf::Font font;
    font.loadFromFile("../fonts/Arial.ttf");
    sf::Texture texture;
    texture.loadFromFile("../images/dark_bg.jpg");
    sf::Sprite bg;
    bg.setTexture(texture);
    bg.setPosition((static_cast<float>(window.getSize().x) * 0.38f), (static_cast<float>(window.getSize().y) * 0.025f));
    bg.setScale((static_cast<float>(window.getSize().x) * 0.24f) / (static_cast<float>(texture.getSize().x)),
                (static_cast<float>(window.getSize().y) * 0.65f) / (static_cast<float>(texture.getSize().y)));
    window.draw(bg);
    float offset {0};
    itemButtons.clear();
    itemTexts.clear();
    sf::Text itemText;
    for(const auto &i : items) {
        itemText.setString(i->getName());
        itemText.setFont(font);
        itemText.setCharacterSize(24);
        itemText.setPosition(bg.getPosition().x + 20, bg.getPosition().y + offset);
        itemText.setFillColor(player->getEquippedWeapon() == i ? sf::Color::Green : sf::Color::Red);
        itemTexts.push_back(itemText);
        itemButtons.emplace_back(
                "Use",
                bg.getPosition().x + bg.getGlobalBounds().width - 120,
                bg.getPosition().y + 20 + offset,
                BUTTON_WIDTH, BUTTON_HEIGHT, font, BUTTON_FONT_SIZE,
                BUTTON_COLOR, BUTTON_TEXT_COLOR,
                [this, &i, &gameScene](){
                    if(i->itemType == game::Item::ItemType::Weapon) {
                        player->equipWeapon(std::dynamic_pointer_cast<game::Weapon>(i));
                        std::cout << "Equipped Weapon." << std::endl;
                    } else if (i->itemType == game::Item::ItemType::Consumable) {
                        std::cout << "Consumed Item." << std::endl;
                        items.erase(std::remove(items.begin(), items.end(), i), items.end());
                    }
                    gameScene.display(gameScene.getDungeonLayout().getCurrentRoom());
                });
        itemButtons.emplace_back(
                "Drop",
                bg.getPosition().x + bg.getGlobalBounds().width - 60,
                bg.getPosition().y + 20 + offset,
                BUTTON_WIDTH, BUTTON_HEIGHT, font, BUTTON_FONT_SIZE,
                BUTTON_COLOR, BUTTON_TEXT_COLOR,
                [this, &i, &gameScene](){
                    std::cout << "Dropped item." << std::endl;
                    items.erase(std::remove(items.begin(), items.end(), i), items.end());
                    gameScene.display(gameScene.getDungeonLayout().getCurrentRoom());
                });
        offset += 62;
    }
    for(auto &t : itemTexts) {
        window.draw(t);
    }
    for(auto &b : itemButtons) {
        b.render(window);
    }
}

void gui::Inventory::putItem(const std::shared_ptr<game::Item> &_item) {
    if(items.size() < INVENTORY_CAPACITY) {
        items.emplace_back(_item);
    } else {
        std::cout << "Inventory full!" << std::endl;
    }
}

std::vector<std::shared_ptr<game::Item>> gui::Inventory::getItems() const {
    return items;
}

std::vector<gui::Button> gui::Inventory::getButtons() const {
    return itemButtons;
}