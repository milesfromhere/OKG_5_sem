
// ChildView.cpp: реализация класса CChildView
//

#include "stdafx.h"
#include "laba_3.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define MARGIN_CYCLE 10
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_TESTS_F1, &CChildView::OnTestsF1)
	ON_COMMAND(ID_TESTS_F2, &CChildView::OnTestsF2)
	ON_COMMAND(ID_TESTS_F3, &CChildView::OnTestsF3)
END_MESSAGE_MAP()



// Обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	/*CPaintDC - это класс, который предоставляет контекст устройства для рисования в окне.
	dc(this) - создает объект CPaintDC, связанный с окном this (текущее окно).*/

	CPaintDC dc(this); // контекст устройства для рисования 
	dc.SetMapMode(MM_ANISOTROPIC);
	if (Index == 1)
	{
		Graph.Draw(dc, 1, 1);
	}
}

double CChildView::MyF1(double x)
{
	double y = sin(x) / x;
	return y;
}

double CChildView::MyF2(double x)
{
	double y = sqrt(abs(x)) * sin(x);
	return y;
}

void CChildView::OnTestsF1()	// MM_TEXT
{
	double xL = -3 * pi;
	double xH = -xL;
	double dx = pi / 36;		//шаг
	int N = (xH - xL) / dx;
	X.RedimMatrix(N + 1);	// Изменяет размер матрицы с уничтожением данных
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF1(X(i));
	}
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));  // тип линии, толщина, Цвет графика
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));  // тип линии, толщина, Цвет координатных осей
	RW.SetRect(200, 200, 600, 600);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	this->Invalidate();
}

void CChildView::OnTestsF2()
{
	double Xl = -4 * pi;
	double Xh = -Xl;
	double dX = pi / 36;
	int N = (Xh - Xl) / dX;
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);
	for (int i = 0; i <= N; i++)
	{
		X(i) = Xl + i * dX;
		Y(i) = MyF2(X(i));
	}
	PenLine.Set(PS_DASH, 1, RGB(255, 0, 0));
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
	RW.SetRect(200, 200, 600, 600);
	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);
	Index = 1;
	this->Invalidate();
}

void CChildView::OnTestsF3()
{
	Invalidate();
	CPaintDC dc(this);

	// Установка параметров пера для отображения фигуры
	CPen penFigure(PS_SOLID, 3, RGB(255, 0, 0));
	dc.SelectObject(&penFigure);

	// Установка параметров пера для отображения окружности
	CPen penCircle(PS_SOLID, 2, RGB(0, 0, 255));

	// Расчет размеров квадрата
	CRect rect;
	GetClientRect(&rect);
	const int squareSize = min(rect.Width() - MARGIN_CYCLE * 2, rect.Height() - MARGIN_CYCLE * 2);

	// Расчет координат точек восьмиугольника
	const double radius = squareSize / 2.0;
	const double angle = 45.0; // Угол между двумя соседними сторонами восьмиугольника
	const double centerX = rect.left + squareSize / 2.0 + MARGIN_CYCLE; // Координаты центра квадрата
	const double centerY = rect.top + squareSize / 2.0 + MARGIN_CYCLE;
	const double circleRadius = radius; // Радиус окружности, в которую вписан восьмиугольник
	const double circleLeft = centerX - circleRadius;
	const double circleTop = centerY - circleRadius;
	const double circleRight = centerX + circleRadius;
	const double circleBottom = centerY + circleRadius;

	// Отображение окружности
	dc.SelectObject(&penCircle);
	dc.Ellipse(circleLeft, circleTop, circleRight, circleBottom);

	// Отображение восьмиугольника
	const int sides = 8;
	const double angleRadians = angle * 3.14159 / 180.0;
	const double startX = centerX + radius;
	const double startY = centerY;
	double currentX = startX;
	double currentY = startY;

	for (int i = 0; i < sides; i++)
	{
		double nextX = centerX + radius * cos(angleRadians * i);
		double nextY = centerY - radius * sin(angleRadians * i);

		// Установка стиля линии для каждой стороны восьмиугольника
		if (i % 2 == 0)
			dc.SelectObject(&penFigure);
		else
			dc.SelectObject(&penFigure);

		dc.MoveTo((int)currentX, (int)currentY);
		dc.LineTo((int)nextX, (int)nextY);

		currentX = nextX;
		currentY = nextY;
	}

	// Отображение последней стороны восьмиугольника
	dc.SelectObject(&penFigure);
	dc.MoveTo((int)currentX, (int)currentY);
	dc.LineTo((int)startX, (int)startY);
}

//
//void CChildView::OnTestsF3()
//{
//	Invalidate();
//
//	CPaintDC dc(this);
//
//	int N = 8;  // Восьмиугольник
//	double R = 10; // Радиус окружности
//	X.RedimMatrix(N + 1);
//	Y.RedimMatrix(N + 1);
//
//	for (int i = 0; i < N; i++)
//	{
//		double angle = 2 * pi * i / N; // Угол в радианах
//		X(i) = R * cos(angle);
//		Y(i) = R * sin(angle);
//	}
//
//	X(N) = X(0);
//	Y(N) = Y(0);
//
//	PenLine.Set(PS_SOLID, 3, RGB(255, 0, 0));
//	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
//	RW.SetRect(200, 200, 600, 600);
//	Graph.SetParams(X - 0.01, Y, RW);
//	Graph.SetPenLine(PenLine);
//	Graph.SetPenAxis(PenAxis);
//	Graph.Draw(dc, 0, 1);
//
//	N = 1800;
//	X.RedimMatrix(N);
//	Y.RedimMatrix(N);
//
//	double radius = 10.0;
//
//	for (int i = 0; i < N; i++)
//	{
//		double angle = 2.0 * 3.14 * i / N;
//		X(i) = radius * cos(angle);
//		Y(i) = radius * sin(angle);
//	}
//
//	PenLine.Set(PS_SOLID, 2, RGB(0, 0, 255));
//	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));
//	RW.SetRect(200, 200, 600, 600);
//	Graph.SetParams(X - 0.01, Y, RW);
//	Graph.SetPenLine(PenLine);
//	Graph.SetPenAxis(PenAxis);
//	Graph.Draw(dc, 0, 0);
//}