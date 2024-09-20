
// WokerThread1View.cpp: CWokerThread1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WokerThread1.h"
#endif

#include "WokerThread1Doc.h"
#include "WokerThread1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWokerThread1View

IMPLEMENT_DYNCREATE(CWokerThread1View, CView)

BEGIN_MESSAGE_MAP(CWokerThread1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CWokerThread1View 생성/소멸

CWokerThread1View::CWokerThread1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWokerThread1View::~CWokerThread1View()
{
}

BOOL CWokerThread1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWokerThread1View 그리기

void CWokerThread1View::OnDraw(CDC* /*pDC*/)
{
	CWokerThread1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CWokerThread1View 인쇄

BOOL CWokerThread1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWokerThread1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWokerThread1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWokerThread1View 진단

#ifdef _DEBUG
void CWokerThread1View::AssertValid() const
{
	CView::AssertValid();
}

void CWokerThread1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWokerThread1Doc* CWokerThread1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWokerThread1Doc)));
	return (CWokerThread1Doc*)m_pDocument;
}
#endif //_DEBUG

struct ThreadParams {
	CWnd* pWnd;
	CPoint point;
	int val;
};

// CWokerThread1View 메시지 처리기
UINT Calclt(LPVOID pParam) {
	ThreadParams* pParams = (ThreadParams*)pParam;
	CWnd* pWnd = pParams->pWnd;
	CPoint point = pParams->point;
	int val = pParams->val;
	delete pParams; // 메모리 해제

	int result = 0;
	for (int i = 0; i < val; i++) {
		result += i;
		Sleep(10);
	}

	CString str;
	str.Format(_T("계산 결과 = %d"), result);

	CClientDC dc(pWnd);
	dc.TextOut(point.x, point.y, str);
	return 0;

}

void CWokerThread1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int val = 600;

	ThreadParams* pParams = new ThreadParams;
	pParams->pWnd = this;
	pParams->point = point;
	pParams->val = val;

	AfxBeginThread(Calclt, (LPVOID)pParams);

	CView::OnLButtonDown(nFlags, point);
}

