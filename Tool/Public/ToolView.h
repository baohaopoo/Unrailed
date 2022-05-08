
#pragma once

// CToolView 폼 뷰입니다.
#include "GameInstance.h"
#include "Tool_Defines.h"


class CToolView : public CFormView
{
	DECLARE_DYNCREATE(CToolView)

protected:
	CToolView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CToolView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device;
	CGameInstance*		m_pInstance = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	LEVEL				m_eCurrLevel; 
	CRect				m_rectCurHist;

public:
	virtual void OnInitialUpdate();


private:
	void SetUp_DefaultRenderState();
	
	void ViewInitialize();
	void Ready_Prototype_Component();

	void Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Texture_SeongUk();
	HRESULT Ready_Prototype_Texture_MinHee();
	HRESULT Ready_Prototype_Texture_JuhYeon();
	HRESULT Ready_Prototype_Texture_HyunWoo();

	void Ready_Prototype_GameObject();
	void OpenLevel();

public:
	CRenderer* Get_Renderer(void) { return m_pRenderer; }
	LPDIRECT3DDEVICE9 Get_Device(void) { return m_pGraphic_Device; }

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};