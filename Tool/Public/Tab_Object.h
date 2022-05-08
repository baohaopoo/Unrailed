#pragma once


// CTab_Object ��ȭ �����Դϴ�.

#include "Tool_Defines.h"
#include "afxwin.h"
#include "GameObject.h"

BEGIN(Tool)

class CTab_Object : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Object)

public:
	CTab_Object(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTab_Object();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// control
	// =============================================================================
	CButton m_BtToolObjectSave;		// ObjectTool���� ����� save��ư
	CButton m_BtToolObjectLoad;		// ObjectTool���� ����� load��ư

	CButton m_BtMapObjectLoad;		// MapTool���� ����� load��ư

	CButton m_BtColor;		// �÷� ��ư
	CButton m_BtTexture;	// �ؽ��� ȭ��� ������ ��ư

	CButton m_BtApplyScale;
	CButton m_BtApplyRotation;
	CButton m_BtApplyTransform;

	CListBox m_lstBoxObjectFile;	// load�� �ʿ��� ������Ʈ ���� ���� ����Ʈ

	CEdit m_EditScaleX;		// ������ x,y,z
	CEdit m_EditScaleY;
	CEdit m_EditScaleZ;

	CEdit m_EditRotateX;	// ȸ�� x,y,z
	CEdit m_EditRotateY;
	CEdit m_EditRotateZ;

	CEdit m_EditTransformX; // �̵� x,y,z
	CEdit m_EditTransformY;
	CEdit m_EditTransformZ;

	CEdit m_EditTextureNumber;	// �ؽ��� ��ȣ

	CButton m_CheckMinus;
	CButton m_BtCheckMoveAllMinus;

	int m_nModeRadio;				// ��� ������ ���� ������ư �׷�
	int m_nSelectObjectRadio;		// ������ ������Ʈ �׷�

	UINT m_iObjectRotateYAngle;		// ��� ������Ʈ�� Y������ ������ ���� �����ϴ� Y��

	UINT m_iMoveAll_X;	// ��� ������Ʈ�� X,Y,Z�� ��ŭ �̵��ϱ� ���� ����
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