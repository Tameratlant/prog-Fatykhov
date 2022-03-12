#include <SFML/Graphics.hpp>
#define NUM_OF_ITERATIONS 10  //"глубина" фрактала. Больше 10 работает очень долго
sf::RenderWindow window(sf::VideoMode(900, 900), "SFML works!");

struct Point {
    float x, y;
};

void print(int x1, int y1, int x2, int y2, int x3, int y3)
{
    sf::ConvexShape polygon;
    polygon.setPointCount(3);
    polygon.setPoint(0, sf::Vector2f(x1, y1));
    polygon.setPoint(1, sf::Vector2f(x2, y2));
    polygon.setPoint(2, sf::Vector2f(x3, y3));


    polygon.setOutlineColor(sf::Color::Red);
    polygon.setOutlineThickness(1);
    window.draw(polygon);
}

void fract(int x1, int y1, int x2, int y2, int x3, int y3, int n)
{
    
    int i = 0;
    print(x1, y1, x2, y2, x3, y3);

    if (n == NUM_OF_ITERATIONS) return;

    print(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);
    print(x2, y2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2);
    print(x3, y3, (x3 + x2) / 2, (y3 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);


    fract(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2, n+1);
    fract(x2, y2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2, n+1);
    fract(x3, y3, (x3 + x2) / 2, (y3 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2, n+1);

    n++;
}

int main()
{
    Point a, b, c;                                  //Вершины треугольника
    a = { 450, 100 };
    b = { 900, 779 };
    c = { 0, 779 };
    int x1 = 450, y1 = 100, x2 = 900, y2 = 779, x3 = 0, y3 = 779;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        fract(a.x, a.y, b.x, b.y, c.x, c.y, 0);
        window.display();
    }
    return 0;
}
