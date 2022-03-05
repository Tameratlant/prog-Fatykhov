

#include <iostream>
#include "Vector2.h"
using namespace std;







    int main() {
        setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
        //Тест конструктора
        Vector2 a (4, 3);

        //Тест вывода
        cout<<"Тест вывода " << a;
        
        //Тест ввода
        cin >> a;
        cout << a;

        //Тест вывода длинны
        cout << "Тест вывода длины " << a.Len() << "\n";
        cout << a.SquareLen() << "\n";


        //Тест +
        Vector2 b(4, 3);
        cout <<"Тест + "<< a + b << "\n";

        //Тест +=
        Vector2 c = a += b;
        cout <<"Тест += "<< c << "\n";

        //Тест скалярного и векторного умножения
        
        int d;
        d = a ^ b;
        cout <<"Тест скалярного и векторного умножения "<< a * b << "\n";
        cout << d<<"\n";

        //Тест *
        cout <<"Тест * "<< 3 * a << a * 3 << "\n";

        //Тест /
        cout << "Тест / "<< a / 3 << "\n";

        //Тест нормировки
        cout << "Тест нормировки " << a.Norm() << "\n";

        //Тест перпендикуляра
        cout << "Тест перпендикуляра "<< a << a.Perp()<< "\n";

        //Тест унарного -
        cout << "Тест унарного - " << -a<<"\n";

        //Тест rotate и getRotate
        cout << "Тест rotate и getRotate " << a.getRotate(1) << a.rotate(1) << " ";
        return 0;
    }
