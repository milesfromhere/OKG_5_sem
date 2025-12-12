
// CBladeView.cpp: реализация класса CCBladeView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "CBlade.h"
#endif

#include "CBladeDoc.h"
#include "CBladeView.h"
#include "TriangleDrawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBladeView

IMPLEMENT_DYNCREATE(CCBladeView, CView)

BEGIN_MESSAGE_MAP(CCBladeView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_BLADE, &CCBladeView::OnBlade)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_SIZE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// Создание или уничтожение CCBladeView

CCBladeView::CCBladeView() noexcept
{
	TimerID = 0;
	IsRotating = FALSE;
	BladeVisible = FALSE;

}

CCBladeView::~CCBladeView()
{
    if (TimerID != 0)
    {
        if (::IsWindow(m_hWnd))
            KillTimer(TimerID);
        TimerID = 0;
    }
}

BOOL CCBladeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CCBladeView

void CCBladeView::OnDraw(CDC* pDC)
{
	CCBladeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (BladeVisible)
	{
		Blade.Draw(*pDC);
	}

    // Простой треугольник: основание 20, высота 90.5, по центру окна
    CRect client;
    GetClientRect(&client);
    const int base = 20;
    const int height = static_cast<int>(std::lround(90.5));
    int left = (client.Width() - base) / 2;
    int top = (client.Height() - height) / 2;
    CRect triRect(left, top, left + base, top + height);

    CPen penTriangle(PS_SOLID, 1, RGB(0, 0, 0));
    CBrush brushTriangle(RGB(200, 200, 0));
    CPen* oldPen = pDC->SelectObject(&penTriangle);
    CBrush* oldBrush = pDC->SelectObject(&brushTriangle);
    DrawTriangle(*pDC, triRect);
    pDC->SelectObject(oldPen);
    pDC->SelectObject(oldBrush);
}

void CCBladeView::OnBlade()
// Обработчик меню "Лопасть"
{
    CRect clientRect;
    GetClientRect(&clientRect);

    // Вычисляем размеры окна под лопасть
    CRectD sr = Blade.GetSpaceRect();
    CSizeD sz = sr.SizeD();
    int width = (int)(sz.cx * 25) + 100;   // Масштаб 25 пикселей на единицу
    int height = (int)(sz.cy * 25) + 100;

    // Устанавливаем область для рисования (с отступами)
    CRect winRect(50, 50, width - 50, height - 50);
    Blade.SetWinRect(winRect);

    // Фиксируем размер окна главного фрейма
    CFrameWnd* pFrame = GetParentFrame();
    if (pFrame != NULL)
    {
        // Вычисляем размер окна с учетом меню, borders и т.д.
        CRect frameRect;
        pFrame->GetWindowRect(&frameRect);

        CRect clientFrame;
        pFrame->GetClientRect(&clientFrame);

        int extraWidth = frameRect.Width() - clientFrame.Width();
        int extraHeight = frameRect.Height() - clientFrame.Height();

        // Устанавливаем фиксированный размер окна
        pFrame->SetWindowPos(NULL, 0, 0,
            width + extraWidth,
            height + extraHeight,
            SWP_NOMOVE | SWP_NOZORDER);

        // Запрещаем изменение размера окна
        LONG style = GetWindowLong(pFrame->m_hWnd, GWL_STYLE);
        style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
        SetWindowLong(pFrame->m_hWnd, GWL_STYLE, style);

        // Обновляем внешний вид окна
        pFrame->SetWindowPos(NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    BladeVisible = TRUE;
    IsRotating = FALSE;
    Invalidate();
}

//---------------------------------Скорость вращения---------------------------------------------

void CCBladeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (BladeVisible && !IsRotating)
    {
        TimerID = SetTimer(1, 500 / 360, NULL);
        IsRotating = TRUE;
    }

}

//------------------------------------------------------------------------------

void CCBladeView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    if (BladeVisible && IsRotating)
    {
        if (TimerID != 0)
        {
            KillTimer(TimerID);
            TimerID = 0;
        }
        IsRotating = FALSE;
    }

    CView::OnRButtonDblClk(nFlags, point);
}

//------------------------------------------------------------------------------

void CCBladeView::OnTimer(UINT_PTR nIDEvent)
// Обработчик таймера - выполняет вращение
{
    if (nIDEvent == TimerID && BladeVisible)
    {
        Blade.Rotate();
        Invalidate();
    }

    CView::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------

void CCBladeView::OnSize(UINT nType, int cx, int cy)
// Обработчик изменения размера окна
{
    CView::OnSize(nType, cx, cy);

    // Если лопасть видима, обновляем её область рисования
    if (BladeVisible && cx > 100 && cy > 100)
    {
        CRect winRect(50, 50, cx - 50, cy - 50);
        Blade.SetWinRect(winRect);
        Invalidate();
    }
}

void CCBladeView::OnDestroy()
{
    if (TimerID != 0)
    {
        KillTimer(TimerID);
        TimerID = 0;
    }
    IsRotating = FALSE;

    CView::OnDestroy();
}

// Печать CCBladeView

BOOL CCBladeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CCBladeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CCBladeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CCBladeView

#ifdef _DEBUG
void CCBladeView::AssertValid() const
{
	CView::AssertValid();
}

void CCBladeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCBladeDoc* CCBladeView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCBladeDoc)));
	return (CCBladeDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CCBladeView
