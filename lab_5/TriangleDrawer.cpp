#include "pch.h"
#include "TriangleDrawer.h"
#include <cmath>

// Рисует треугольник по трём точкам, рассчитанным от заданного прямоугольника.
// Основание 20, высота 90.5 (округление до пикселя), центрируем внутри bounds.
void DrawTriangle(CDC& dc, const CRect& bounds)
{
    const int base = 20;
    const int height = static_cast<int>(std::lround(90.5));

    // Координаты трёх вершин, вычисленные от границ bounds
    POINT pts[3];

    // Центрируем по горизонтали
    int centerX = bounds.left + bounds.Width() / 2;
    // Верхняя точка — отступ сверху так, чтобы вся высота поместилась
    int topY = bounds.top + (bounds.Height() - height) / 2;
    int baseY = topY + height;

    // Верхняя вершина
    pts[0].x = centerX;
    pts[0].y = topY;
    // Левая нижняя вершина
    pts[1].x = centerX - base / 2;  
    pts[1].y = baseY;
    // Правая нижняя вершина
    pts[2].x = centerX + base / 2;
    pts[2].y = baseY;

    //x*cos + x0*sin
    //y*cos + y0*sin
    dc.Polygon(pts, 3);
}

