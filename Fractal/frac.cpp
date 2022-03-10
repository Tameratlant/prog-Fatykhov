#include <SFML/Graphics.hpp>

sf::RenderWindow window(sf::VideoMode(900, 900), "SFML works!");

void print(int x1, int y1, int x2, int y2, int x3, int y3)
//Сделай вектор полигонов и вызывай их
{
    sf::ConvexShape polygon;
    polygon.setPointCount(3);
    polygon.setPoint(0, sf::Vector2f(x1, y1));
    polygon.setPoint(1, sf::Vector2f(x2, y2));
    polygon.setPoint(2, sf::Vector2f(x3, y3));


    polygon.setOutlineColor(sf::Color::Red);
    polygon.setOutlineThickness(1);
    //polygon.setPosition(10, 20);
    window.draw(polygon);
    polygon.~();
    //print(x1, y1, x2, y2, x3, y3);
    

}

void fract(int x1, int y1, int x2, int y2, int x3, int y3)
{
    int i = 0;
    print(x1, y1, x2, y2, x3, y3);
    
        

        /*print(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);
        print(x2, y2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2);
        print(x3, y3, (x3 + x2) / 2, (y3 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);

        print(x1, y1, (x1 + x2) / 4, (y1 + y2) / 4, (x1 + x3) / 4, (y1 + y3) / 4);
        print(x2, y2, (x1 + x2) / 4, (y1 + y2) / 4, (x2 + x3) / 4, (y2 + y3) / 4);
        print(x3, y3, (x3 + x2) / 4, (y3 + y2) / 4, (x1 + x3) / 4, (y1 + y3) / 4);*/
        
    
    //print(x1, y1, x2, y2, x3, y3);
    //print(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);
    //fract(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);
    //int x11 = (x2 + x3) / 2;
    //int x12 = (x1 + x3) / 2;
    //int x13 = (x1 + x2) / 2;
    //int y11 = (y2 + y3) / 2;
    //int y12 = (y1 + y3) / 2;
    //int y13 = (y2 + y1) / 2;

    ///*int x21 = (x2 + x3) / 2;
    //int x22 = (x1 + x3) / 2;
    //int x23 = (x1 + x2) / 2;
    //int y21 = (y2 + y3) / 2;
    //int y22 = (y1 + y3) / 2;
    //int y23 = (y2 + y1) / 2;

    //int x31 = (x2 + x3) / 2;
    //int x32 = (x1 + x3) / 2;
    //int x33 = (x1 + x2) / 2;
    //int y31 = (y2 + y3) / 2;
    //int y32 = (y1 + y3) / 2;
    //int y33 = (y2 + y1) / 2;*/



    //print(x11, y11, x12, y12, x13, y13);

    //print((x1 + x3) / 2, (y1 + y3) / 2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2);
    //
    //print (
    //    abs(x1 - x2) / 4 + x1,
    //    abs(y1 - y2) / 4 + y1,
    //    abs(x2 - x3) / 4 + x3,
    //    abs(y2 - y3) / 4,
    //    abs(x1 - x2) / 4 * 3,
    //    abs(y1 - y2) / 4
    //    );

    //print(
    //    (x1 + x2) / 4 * 3,
    //    (y1 = y2) / 4 * 3,
    //    (x1 + x2) / 4,
    //    (y2 + y3) / 4,
    //    (x3 + x2) / 4 * 3,
    //    (y1 + y2) / 4
    //);

    //print(
    //    ((x1 + x2) / 2 + x1) / 2,
    //    ((y1 + y2) / 2 + y1) / 2,
    //    ((x1 + x2) / 2 + (x1 + x3) / 2) / 2,
    //    ((y1 + y2) / 2 + (y1 + y3) / 2) / 2,
    //    ((x1 + x3) / 2 + x1) / 2,
    //    ((y1 + y3) / 2 + y1) / 2
    //);
}

int main()
{
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
        print(x1, y1, x2, y2, x3, y3);



        //print((x1 + x3) / 2, (y1 + y3) / 2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2);
        
        fract(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);
        fract(x2, y2, (x1 + x2) / 2, (y1 + y2) / 2, (x2 + x3) / 2, (y2 + y3) / 2);
        fract(x3, y3, (x3 + x2) / 2, (y3 + y2) / 2, (x1 + x3) / 2, (y1 + y3) / 2);

        window.display();
    }

    return 0;
}
