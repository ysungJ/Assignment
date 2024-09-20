
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "ClientDC.h"
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
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
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
	POSITION pos = m_PointList.GetHeadPosition();

	while (pos != nullptr) {
		CPoint point = m_PointList.GetNext(pos);
	
		dc.Ellipse(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
	}
	
}


//왼쪽 마우스버튼클릭
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	dc.Ellipse(point.x - 20, point.y - 20, point.x + 20, point.y + 20);

	// CList에 좌표 추가
	m_PointList.AddTail(point);

	// 윈도우 다시 그리기 요청
	CWnd::Invalidate();

	CWnd::OnLButtonDown(nFlags, point);

}



//오른쪽 마우스버튼 클릭
void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);

	dc.Rectangle(point.x - 20, point.y - 20, point.x + 20, point.y + 20);

	// CList에 좌표 추가
	m_PointList.AddTail(point);

	// 윈도우 다시 그리기 요청
	CWnd::Invalidate();

	CWnd::OnRButtonDown(nFlags, point);


}

