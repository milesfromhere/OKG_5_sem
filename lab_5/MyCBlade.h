#pragma once
#include "pch.h"

class CBlade
{
private:
    CMatrix Points;          // Точки контура лопасти
    double angle;            // Текущий угол поворота
    double delta_angle;      // Шаг вращения
    CRectD SpaceRect;        // Область в мировых координатах
    CRect WinRect;           // Область в окне
    bool clockwise;          // true = по часовой, false = против

    // Параметры лопасти
    double h;                // Высота треугольника
    double phi;              // Угол при вершине (градусы)
    double R;                // Радиус описанной окружности

    void CalculatePoints();  // Вычисляет координаты точек лопасти

public:
    CBlade();                                    // Конструктор по умолчанию
    CBlade(double height, double angle_phi, double radius);  // Конструктор с параметрами
    void SetWinRect(CRect& rw);                 // Установить область окна
    void Draw(CDC& dc);                         // Нарисовать лопасть
    void Rotate();                              // Повернуть на delta_angle
    void SetDirection(bool cw);                 // Установить направление вращения
    void ToggleDirection();                     // Изменить направление вращения
    CRectD GetSpaceRect() { return SpaceRect; }
};