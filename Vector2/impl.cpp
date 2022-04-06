#include "Vector2.h"
#include <math.h>

Vector2::Vector2(float x = 0, float y = 0) : x(x), y(y)
{}

//Вывести длину
float Vector2::Len() const
{
    return sqrt(x * x + y * y);
}

//Вывести квадрат длины
float Vector2::SquareLen() const
{
    return x * x + y * y;
}

//Вывести нормированный вектор
Vector2 Vector2::Norm() const
{
    Vector2 result;
    result.x = x / sqrt(x * x + y * y);
    result.y = y / sqrt(x * x + y * y);
    return result;
}

//Оператор + 
Vector2 Vector2::operator+(const Vector2& other)
{
    Vector2 result;
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
}
//Оператор - 
Vector2 Vector2::operator-(const Vector2& other)
{
    Vector2 result;
    result.x = x - other.x;
    result.y = y - other.y;
    return result;
}


//Оператор / на скаляр
Vector2 Vector2::operator/(const float a)
{
    Vector2 result;
    result.x = x / a;
    result.y = y / a;
    return result;
}

//Оператор * с двух сторон
Vector2 Vector2::operator*(const float a)
{
    Vector2 result;
    result.x = x * a;
    result.y = y * a;
    return result;
}
Vector2 operator*(const float a, Vector2 b)         //было friend
{
    Vector2 result;
    result.x = a * b.x;
    result.y = a * b.y;
    return result;
}

//Скалярное произведение
float Vector2::operator*(Vector2& other) const
{
    return(x * other.x + y * other.y) / sqrt(x * x + y * y);
}

//Векторниое произведение 
float Vector2::operator^(Vector2& other) const
{
    Vector2 result;
    result.x = x * other.y;
    result.y = y * other.x;
    return result.Len();
}

//Унарный -
Vector2 Vector2::operator-() const
{
    Vector2 result;
    result.x = -x;
    result.y = -y;
    return result;
}

//Унарный +
Vector2 Vector2::operator+() const
{
    Vector2 result;
    return result;
}

//Перпендикуляр
Vector2 Vector2::Perp() const
{
    Vector2 result;
    result.x = -x;
    result.y = y;
    return result;
}

//Оператор вывода
std::ostream& operator << (std::ostream& os, const Vector2& out)
{
    os << out.x << " " << out.y << "\n";
    return os;
}
//Оператор ввода
std::istream& operator >> (std::istream& is, Vector2& in)
{
    is >> in.x >> in.y;
    return is;
}

//Операторы += и -=
Vector2 Vector2::operator+=(Vector2 const&  other)
{
    Vector2 result;
    result.x = other.x + x;
    result.y = other.y + y;
    return result;
}
Vector2 Vector2::operator-=(Vector2 const&  other)
{
    Vector2 result;
    result.x = other.x - x;
    result.y = other.y - y;
    return result;
}

Vector2 Vector2::rotate(const float angle)
{
    x = x * cos(angle) - y * sin(angle);
    y = x * sin(angle) - y * cos(angle);
    Vector2 result(0, 0);
    result.x = x;
    result.y = y;
    return result;
}

Vector2 Vector2::getRotate(const float angle)
{
    Vector2 result(0, 0);
    result.x = x * cos(angle) - y * sin(angle);
    result.y = x * sin(angle) - y * cos(angle);
    return result;
}



