#include <SFML\Graphics.hpp>
//#include<wingdi.h>
//#include<windows.h>
#include "Vector2.h"

sf::RenderWindow window(sf::VideoMode(800, 600), "My window");


void zalupa() {
    sf::Vector2i MousePosition;
    while (1) {
        MousePosition = sf::Mouse::getPosition(window);
        Vector2 MP = { (float)MousePosition.x, (float)MousePosition.y };
        sf::CircleShape circle(50.f);

        circle.setOutlineThickness(15.f); // устанавливаем толщину контура круга
        circle.setOutlineColor(sf::Color(80, 220, 50)); // устанавливаем цвет контура
        circle.move((int)MP.x, (int)MP.y); // перемещаем наш круг для корректного отображения в окне
        window.draw(circle);
    }
}

int main()
{
    

    //sf::Vector2i MousePosition = sf::Mouse::getPosition(window);
    //Vector2 MP = { (float)MousePosition.x, (float)MousePosition.y };
    //sf::CircleShape circle(50.f);

    //circle.setOutlineThickness(15.f); // устанавливаем толщину контура круга
    //circle.setOutlineColor(sf::Color(80, 220, 50)); // устанавливаем цвет контура
    //circle.move(15, 15); // перемещаем наш круг для корректного отображения в окне
    
    while (window.isOpen())
    {
        //color = getpixel(10, 20);
  //https://stackoverflow.com/questions/51256740/how-to-get-the-color-of-a-pixel-in-an-image-in-c-sfml

        sf::Image track;
        //sf::Color color = track.getPixel(0, 0);
        window.clear();
        zalupa();
        window.display();
    }

    return 0;
}
