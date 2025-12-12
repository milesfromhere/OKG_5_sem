
// CBlade.h: основной файл заголовка для приложения CBlade
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CCBladeApp:
// Сведения о реализации этого класса: CBlade.cpp
//

class CCBladeApp : public CWinApp
{
public:
	CCBladeApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCBladeApp theApp;
