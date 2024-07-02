
// MainToolView.h: CMainToolView 클래스의 인터페이스
//

#pragma once

class CMainToolDoc;
class Inference;

class CMainToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMainToolView() noexcept;
	DECLARE_DYNCREATE(CMainToolView)

// 특성입니다.
public:
	CMainToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMainToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	virtual BOOL OnIdle(LONG lCount); 
	
	

#endif


private:
	bool runOnGPU = true;


protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MainToolView.cpp의 디버그 버전
inline CMainToolDoc* CMainToolView::GetDocument() const
   { return reinterpret_cast<CMainToolDoc*>(m_pDocument); }
#endif

