#include "stdafx.h"
#include <vector>

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}


void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}


CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	
	cz.cy = fabs(top - bottom);	
	return cz;
}


CMatrix SpaceToWindow(CRectD& RS, CRect& RW)

{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	
	int dwx = sz.cx;	     
	int dwy = sz.cy;	     
	CSizeD szd = RS.SizeD();

	double dsx = szd.cx;
	double dsy = szd.cy;

	double kx = (double)dwx / dsx;
	double ky = (double)dwy / dsy;

	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}


void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)  //MFC

{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
	dc.SetMapMode(MM_ANISOTROPIC);
}

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)	

{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();

	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.SetRectD(x_min, y_max, x_max, y_min);		
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);	
	K = SpaceToWindow(RS, RW);			
}


void CPlot2D::SetWindowRect(CRect& RWX)	
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);			
}


void CPlot2D::GetWindowCoords(double xs, double ys, int& xw, int& yw)		

{
	CMatrix V(3), W(3);
	V(2) = 1;
	V(0) = xs;
	V(1) = ys;
	W = K * V;
	xw = (int)W(0);
	yw = (int)W(1);
}


void CPlot2D::SetPenLine(CMyPen& PLine)	
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}


void CPlot2D::SetPenAxis(CMyPen& PAxis)	
{
	PenAxis.PenStyle = PAxis.PenStyle;
	PenAxis.PenWidth = PAxis.PenWidth;
	PenAxis.PenColor = PAxis.PenColor;
}


void CPlot2D::Draw(CDC& dc, int Ind1, int Ind2)	

{
	double xs, ys;		
	int xw, yw;				

	if (Ind1 == 1) dc.Rectangle(RW);				

	if (Ind2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0)
		{
			xs =  0;  ys = RS.top;				
			GetWindowCoords(xs, ys, xw, yw);	
			dc.MoveTo(xw, yw);					

			xs = 0;  ys = RS.bottom;			
			GetWindowCoords(xs, ys, xw, yw);		
			dc.LineTo(xw, yw);						
		}
		
		if (RS.top * RS.bottom < 0)
		{
			xs = RS.left;  ys = 0;			
			GetWindowCoords(xs, ys, xw, yw);		
			dc.MoveTo(xw, yw);						

			xs = RS.right;  ys = 0;			
			GetWindowCoords(xs, ys, xw, yw);		
			dc.LineTo(xw, yw);						
		}
		dc.SelectObject(pOldPen);
	}

	xs = X(0); ys = Y(0);
	GetWindowCoords(xs, ys, xw, yw);		

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo(xw, yw);							
	for (int i = 1; i < X.rows(); i++)
	{
		xs = X(i); ys = Y(i);
		GetWindowCoords(xs, ys, xw, yw);			
		dc.LineTo(xw, yw);
	}
	dc.SelectObject(pOldPen);

	if (RS.top * RS.bottom < 0)
	{
		double arrow = (RS.right - RS.left) * 0.02;
		double xa = RS.right;
		double ya = 0.0;
		int xMain, yMain, x1, y1, x2, y2;
		GetWindowCoords(xa, ya, xMain, yMain);
		GetWindowCoords(xa - arrow, ya + arrow * 0.4, x1, y1);
		GetWindowCoords(xa - arrow, ya - arrow * 0.4, x2, y2);
		dc.MoveTo(x1, y1);
		dc.LineTo(xMain, yMain);
		dc.LineTo(x2, y2);
	}
	if (RS.left * RS.right < 0)
	{
		double arrow = (RS.top - RS.bottom) * 0.02; 
		double xa = 0.0;
		double ya = RS.top;
		int xMain, yMain, x1, y1, x2, y2;
		GetWindowCoords(xa, ya, xMain, yMain);
		GetWindowCoords(xa - arrow * 0.4, ya - arrow, x1, y1);
		GetWindowCoords(xa + arrow * 0.4, ya - arrow, x2, y2);
		dc.MoveTo(x1, y1);
		dc.LineTo(xMain, yMain);
		dc.LineTo(x2, y2);
	}

	int oldBkMode = dc.SetBkMode(TRANSPARENT);
	CFont labelFont;
	labelFont.CreatePointFont(80, _T("Segoe UI"));
	CFont* pOldFont = dc.SelectObject(&labelFont);

	// ������� �� X (5 ������)
	const int tickCount = 5;
	for (int i = 0; i <= tickCount; ++i)
	{
		double xt = RS.left + (RS.right - RS.left) * i / tickCount;
		int tx, ty;
		GetWindowCoords(xt, 0.0, tx, ty);
		dc.MoveTo(tx, ty - 4);
		dc.LineTo(tx, ty + 4);
		CString lbl;
		lbl.Format(_T("%.0f"), xt);
		dc.TextOutW(tx - 8, ty + 6, lbl);
	}

	// ������� �� Y (5 ������)
	for (int i = 0; i <= tickCount; ++i)
	{
		double yt = RS.bottom + (RS.top - RS.bottom) * i / tickCount;
		int tx, ty;
		GetWindowCoords(0.0, yt, tx, ty);
		dc.MoveTo(tx - 4, ty);
		dc.LineTo(tx + 4, ty);
		CString lbl;
		lbl.Format(_T("%.1f"), yt);
		dc.TextOutW(tx + 6, ty - 6, lbl);
	}

	std::vector<int> vertices;
	for (int i = 1; i < X.rows() - 1; ++i)
	{
		double dy1 = Y(i) - Y(i - 1);
		double dy2 = Y(i + 1) - Y(i);
		if ((dy1 > 0 && dy2 < 0) || (dy1 < 0 && dy2 > 0))
			vertices.push_back(i);
	}

	CPen vertexPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldVertexPen = dc.SelectObject(&vertexPen);
	for (int idx : vertices)
	{
		xs = X(idx);
		ys = Y(idx);
		GetWindowCoords(xs, ys, xw, yw);
		CRect marker(xw - 3, yw - 3, xw + 3, yw + 3);
		dc.Ellipse(marker);
		CString label;
		label.Format(_T("(%.1f; %.2f)"), xs, ys);
		dc.TextOutW(xw + 6, yw - 12, label);
	}
	dc.SelectObject(pOldVertexPen);

	if (RS.left <= 0 && RS.right >= 0 && RS.bottom <= 0 && RS.top >= 0)
	{
		int x0, y0;
		GetWindowCoords(0.0, 0.0, x0, y0);
		CRect zeroMarker(x0 - 4, y0 - 4, x0 + 4, y0 + 4);
		dc.Ellipse(zeroMarker);
		dc.TextOutW(x0 + 8, y0 - 16, _T("0"));
	}

	dc.SelectObject(pOldFont);
	dc.SetBkMode(oldBkMode);
}


void CPlot2D::Draw1(CDC& dc, int Ind1, int Ind2)	
{

	CRect IRS(RS.left, RS.top, RS.right, RS.bottom);
	if (Ind1 == 1)dc.Rectangle(IRS);           
	if (Ind2 == 1)                  
	{//***
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);

		if (RS.left * RS.right < 0)       
		{
			dc.MoveTo(0, (int)RS.top);    
			dc.LineTo(0, (int)RS.bottom); 
		}

		if (RS.top * RS.bottom < 0)       
		{
			dc.MoveTo((int)RS.left, 0);      
			dc.LineTo((int)RS.right, 0);     
		}
		dc.SelectObject(pOldPen);
	}//***

	CPen MyPen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
	CPen* pOldPen = dc.SelectObject(&MyPen);
	dc.MoveTo((int)X(0), (int)Y(0));
	for (int i = 1; i < X.rows(); i++)  dc.LineTo((int)X(i), (int)Y(i));
	dc.SelectObject(pOldPen);
}


void CPlot2D::GetRS(CRectD& RS)	

{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}



