#include "pch.h"

//------------------------------------------------------------------------------

CBlade::CBlade()
{
    angle = 0;
    delta_angle = 5;  

    h = 90.5;       
    phi = 20.0;       
    R = 1.0;         
    clockwise = true; 

    SpaceRect.SetRectD(-8, 8, 8, -8);

    CalculatePoints();
}

//------------------------------------------------------------------------------

CBlade::CBlade(double height, double angle_phi, double radius)
{
    angle = 0;
    delta_angle = 5;

    h = height;
    phi = angle_phi;
    R = radius;

    SpaceRect.SetRectD(-8, 8, 8, -8);

    CalculatePoints();
}

//------------------------------------------------------------------------------

void CBlade::CalculatePoints()
{
    Points.RedimMatrix(3, 6); 

    // ��������� �������� ���� ��� ������� � ��������
    double half_phi = (phi / 2.0) * M_PI / 180.0;

    // ��������� ������ ��������� ������������
    double base_width = 2.0 * h * tan(half_phi);

    // ����� ����������� (������� ���������� �����)
    // ����� 0 - ����������� �������
    Points(0, 0) = -R;
    Points(1, 0) = 0;
    Points(2, 0) = 1;

    // ����� 1 - ������� ����� ���������
    Points(0, 1) = -R - h;
    Points(1, 1) = base_width / 2.0;
    Points(2, 1) = 1;

    // ����� 2 - ������ ����� ���������
    Points(0, 2) = -R - h;
    Points(1, 2) = -base_width / 2.0;
    Points(2, 2) = 1;

    // ������ ����������� (������� ���������� ������)
    // ����� 3 - ����������� �������
    Points(0, 3) = R;
    Points(1, 3) = 0;
    Points(2, 3) = 1;

    // ����� 4 - ������� ����� ���������
    Points(0, 4) = R + h;
    Points(1, 4) = base_width / 2.0;
    Points(2, 4) = 1;

    // ����� 5 - ������ ����� ���������
    Points(0, 5) = R + h;
    Points(1, 5) = -base_width / 2.0;
    Points(2, 5) = 1;
}

//------------------------------------------------------------------------------

void CBlade::SetWinRect(CRect& rw)
// ������������� ������� ���� ��� ���������
// rw - ������������� � ����
{
    WinRect = rw;
}

//------------------------------------------------------------------------------

void CBlade::Draw(CDC& dc)
// ������ ������� � ���� (��� ������������)
// dc - �������� ����������
{
    // ������� �������� ������ ������ ���������
    CMatrix MR = CreateRotate2D(angle);

    // ������������ �����
    CMatrix RotatedPoints = MR * Points;

    // ������� �������������� �� ������� ��������� � �������
    CMatrix MSW = SpaceToWindow(SpaceRect, WinRect);

    // ����������� � ������� ����������
    CMatrix WinPoints = MSW * RotatedPoints;

    // ������� ���� � ����� ��� ������������� (�������)
    //CPen penRed(PS_SOLID, 2, RGB(255, 0, 0));
    //CBrush brushRed(RGB(255, 100, 100));
    //CPen* oldPen = dc.SelectObject(&penRed);
    //CBrush* oldBrush = dc.SelectObject(&brushRed);

CPen penRed(PS_SOLID, 2, RGB(0, 0, 255));
CBrush brushRed(RGB(100, 100, 255));
CPen* oldPen = dc.SelectObject(&penRed);
CBrush* oldBrush = dc.SelectObject(&brushRed);

    // ������ ����� �����������
    POINT pts1[3];
    for (int i = 0; i < 3; i++)
    {
        pts1[i].x = (int)WinPoints(0, i);
        pts1[i].y = (int)WinPoints(1, i);
    }
    dc.Polygon(pts1, 3);

    // ������ ������ �����������
    POINT pts2[3];
    for (int i = 0; i < 3; i++)
    {
        pts2[i].x = (int)WinPoints(0, i + 3);
        pts2[i].y = (int)WinPoints(1, i + 3);
    }
    dc.Polygon(pts2, 3);

    // ������ ����������� ���������� (�������)
    CPen penGreen(PS_SOLID, 2, RGB(0, 200, 0));
    CBrush brushGreen(RGB(100, 255, 100));
    dc.SelectObject(&penGreen);
    dc.SelectObject(&brushGreen);

    // ����������� ����� ���������� � ������� ����������
    CMatrix center(3, 1);
    center(0, 0) = 0;
    center(1, 0) = 0;
    center(2, 0) = 1;
    CMatrix centerWin = MSW * center;

    int cx = (int)centerWin(0, 0);
    int cy = (int)centerWin(1, 0);

    // ��������� ������ � ��������
    double scale = (WinRect.Width()) / (SpaceRect.right - SpaceRect.left);
    int radius_pixels = (int)(R * scale);

    dc.Ellipse(cx - radius_pixels, cy - radius_pixels,
        cx + radius_pixels, cy + radius_pixels);

    // ��������������� ������ ������� GDI
    dc.SelectObject(oldPen);
    dc.SelectObject(oldBrush);
}

//------------------------------------------------------------------------------

void CBlade::Rotate()
// ������������ ������� �� delta_angle ��������
{
    if (clockwise)
        angle -= delta_angle;   // �� �������
    else
        angle += delta_angle;   // ������ �������

    if (angle >= 360)
        angle -= 360;
    else if (angle < 0)
        angle += 360;
}

//------------------------------------------------------------------------------

void CBlade::SetDirection(bool cw)
// ������������� ����������� �������� ����������
{
    clockwise = cw;
}

//------------------------------------------------------------------------------

void CBlade::ToggleDirection()
// �������� ����������� �������� ����������
{
    clockwise = !clockwise;
}

//------------------------------------------------------------------------------