
// CBladeView.h: интерфейс класса CCBladeView
//

#pragma once


class CCBladeView : public CView
{
protected: // создать только из сериализации
	CCBladeView() noexcept;
	DECLARE_DYNCREATE(CCBladeView)

// Атрибуты
public:
	CCBladeDoc* GetDocument() const;
	CBlade Blade;           // Объект лопасти
	UINT_PTR TimerID;       // ID таймера
	BOOL IsRotating;        // Флаг вращения
	BOOL BladeVisible;      // Флаг видимости лопасти

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CCBladeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBlade();                    
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // версия отладки в CBladeView.cpp
inline CCBladeDoc* CCBladeView::GetDocument() const
   { return reinterpret_cast<CCBladeDoc*>(m_pDocument); }
#endif

