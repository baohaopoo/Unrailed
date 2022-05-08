#pragma once


// CTab_Object 대화 상자입니다.

#include "Tool_Defines.h"
#include "afxwin.h"
#include "GameObject.h"

BEGIN(Tool)

class CTab_Object : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Object)

public:
	CTab_Object(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab_Object();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// control
	// =============================================================================
	CButton m_BtToolObjectSave;		// ObjectTool에서 사용할 save버튼
	CButton m_BtToolObjectLoad;		// ObjectTool에서 사용할 load버튼

	CButton m_BtMapObjectLoad;		// MapTool에서 사용할 load버튼

	CButton m_BtColor;		// 컬러 버튼
	CButton m_BtTexture;	// 텍스쳐 화면과 연동된 버튼

	CButton m_BtApplyScale;
	CButton m_BtApplyRotation;
	CButton m_BtApplyTransform;

	CListBox m_lstBoxObjectFile;	// load에 필요한 오브젝트 저장 파일 리스트

	CEdit m_EditScaleX;		// 스케일 x,y,z
	CEdit m_EditScaleY;
	CEdit m_EditScaleZ;

	CEdit m_EditRotateX;	// 회전 x,y,z
	CEdit m_EditRotateY;
	CEdit m_EditRotateZ;

	CEdit m_EditTransformX; // 이동 x,y,z
	CEdit m_EditTransformY;
	CEdit m_EditTransformZ;

	CEdit m_EditTextureNumber;	// 텍스쳐 번호

	CButton m_CheckMinus;
	CButton m_BtCheckMoveAllMinus;

	int m_nModeRadio;				// 모드 선택을 위한 라디오버튼 그룹
	int m_nSelectObjectRadio;		// 선택한 오브젝트 그룹

	UINT m_iObjectRotateYAngle;		// 모든 오브젝트를 Y축으로 돌리기 위해 존재하는 Y값

	UINT m_iMoveAll_X;	// 모든 오브젝트를 X,Y,Z값 만큼 이동하기 위해 존재
	UINT m_iMoveAll_Y;
	UINT m_iMoveAll_Z;

	afx_msg void RadioCtrl(UINT id);
	afx_msg void Click_CubeButton();
	// =============================================================================
	
	// general
	// =============================================================================
	mutable CGameObject* m_pSelectObject = nullptr;

	void Set_SelectObject(CGameObject* _pObject) { m_pSelectObject = _pObject; }
	void Change_Mode(_int iType) { m_nModeRadio = iType; }

	int m_iTextureNumber;
	int Get_TextureNumber(void) const { return m_iTextureNumber; }
	// =============================================================================

	// value
	// =============================================================================

	// =============================================================================

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void ClickScaleApply();
	afx_msg void ClickRotationApply();
	afx_msg void ClickTransformApply();

	afx_msg void EveryObjectRotateY();
	afx_msg void LoadToolObjectData();
	afx_msg void SaveToolObjectData();

	afx_msg void ClickTextureNumberApply();
	afx_msg void ChangeSelectObjectTexture();
	afx_msg void MoveEveryObject();
	afx_msg void ObjectReversal_Y();
};

END