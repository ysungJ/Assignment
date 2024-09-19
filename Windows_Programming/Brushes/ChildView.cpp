
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Brushes.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
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
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.

 // 비트맵 이미지를 로드합니다. (파일 경로 및 이름에 맞게 수정 필요)
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP2); // IDB_BITMAP_PATTERN은 비트맵 리소스의 ID입니다.

	// 패턴 브러시를 생성합니다.
	CBrush brush;
	brush.CreatePatternBrush(&bitmap);

	// 클라이언트 영역의 크기를 얻습니다.
	CRect rect;
	GetClientRect(&rect);

	// 패턴 브러시를 사용하여 클라이언트 영역을 채웁니다.
	dc.FillRect(&rect, &brush);
}

