#pragma once
#ifndef VECTOR_IMPL__
#define VECTOR_IMPL__
#include <iostream>

struct Vector2 final
{
    float x = 0, y = 0;
public:
    Vector2(float x , float y );

    //Вывести длину
    float  Len() const;

    //Вывести квадрат длины
    float SquareLen() const;

    //Вывести нормированный вектор
    Vector2 Norm() const;

    //Оператор + 
    Vector2 operator+(const Vector2& other);

    //Оператор - 
    Vector2 operator-(const Vector2& other);

    //Оператор / на скаляр
    Vector2 operator/(float a);

    //Оператор * с двух сторон
    Vector2 operator*(float a);

    friend Vector2 operator*(const float a, Vector2 b);

    //Скалярное произведение
    float operator*(Vector2& other) const;

    //Векторниое произведение 
    float operator^(Vector2& other) const;

    //Унарный -
    Vector2 operator-() const;

    //Унарный +
    Vector2 operator+() const;

    //Перпендикуляр
    Vector2 Perp() const;

    //Оператор вывода
    friend std::ostream& operator << (std::ostream& os, const Vector2& out);

    //Оператор ввода
    friend std::istream& operator >> (std::istream& is, Vector2& in);


    //Операторы += и -=
    Vector2 operator+=(Vector2 const & other);

    Vector2 operator-=(Vector2 const& other);

    Vector2 rotate(const float angle);

    Vector2 getRotate(const float angle);
};

#endif
