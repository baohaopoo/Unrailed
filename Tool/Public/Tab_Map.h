#pragma once


// CTab_Map 대화 상자입니다.
#include "Tool_Defines.h"
#include "afxwin.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Tool)

class CTab_Map : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Map)

public:
	CTab_Map(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTab_Map();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_MAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
	enum OBJECT {
		OBJECT_ROCK1, OBJECT_ROCK2, OBJECT_ROCK3,

		OBJECT_TREE_DARK1, OBJECT_TREE_DARK2, OBJECT_TREE_DARK3, OBJECT_TREE_DARK4, OBJECT_TREE_LIGHT,

		OBJECT_MATERIAL_IRON, OBJECT_MATERIAL_WOOD,

		OBJECT_PLATFORM_START, OBJECT_PLATFORM_FINISH,

		OBJECT_TRAIN_HEAD, OBJECT_TRAIN_MATERIAL, OBJECT_TRAIN_RAIL, OBJECT_TRAIN_WATER, OBJECT_TRAIN_DYNAMITE,

		OBJECT_PICKAX, OBJECT_AXE, OBJECT_BUCKET,

		OBJECT_CAMEL,

		OBJECT_RAIL_STRAIGHT, OBJECT_RAIL_CURVE,

		OBJECT_UNBREAKABLE_ROCK1, OBJECT_UNBREAKABLE_ROCK2, OBJECT_UNBREAKABLE_ROCK3,

		OBJECT_MAPCUBE, OBJECT_RAIL_END,

		OBJECT_TREE_DARK_HIT1, OBJECT_TREE_DARK_HIT2, OBJECT_TREE_LIGHT_HIT1, OBJECT_TREE_LIGHT_HIT2,

		OBJECT_ROCK_HIT1, OBJECT_ROCK_HIT2, 

		OBJECT_END
	};

public:
	// control
	// ==========================================
	CButton m_BtApplyScale;
	CButton m_BtApplyRotation;
	CButton m_BtApplyTransform;

	CEdit m_EditMapSizeX;
	CEdit m_EditMapSizeZ;
	CEdit m_EditScaleX;
	CEdit m_EditScaleY;
	CEdit m_EditScaleZ;
	CEdit m_EditRotateX;
	CEdit m_EditRotateY;
	CEdit m_EditRotateZ;
	CEdit m_EditTransformX;
	CEdit m_EditTransformY;
	CEdit m_EditTransformZ;
	// ==========================================

	// general
	// ==========================================
private:
	int m_iTerrainSizeX;
	int m_iTerrainSizeZ;
	UINT m_iTextureNumber;

private:
	afx_msg void ApplySelectObjectScale();
	afx_msg void ApplySelectObjectRotation();
	afx_msg void ApplySelectObjectTransform();
	mutable CGameObject* m_pSelectObject = nullptr;

public:
	void Set_SelectObject(CGameObject* _pObject) { m_pSelectObject = _pObject; }
	UINT Get_TextureNumber(void) const { return m_iTextureNumber; }
	const vector<const TCHAR*>& Get_vObjectFile(void) const { return m_vObjectFile; }
	// ==========================================

private:
	// value
	// ==========================================
	vector<const TCHAR*> m_vObjectFile;
	int m_nModeRadio;				// 모드 선택을 위한 라디오버튼 그룹
	// ==========================================
	
	afx_msg void ChangeTerrainSize();
	afx_msg void SaveMapData();
	afx_msg void LoadMapData();

	virtual BOOL OnInitDialog();
	CEdit m_ObjectScaleX;
	CEdit m_ObjectScaleY;
	CEdit m_ObjectScaleZ;
	CEdit m_ObjectRotationY;

public:
	afx_msg void RadioCtrl(UINT id);

public:
	const CTransform::TRANSFORMDESC& Get_TransDesc(void) const { return m_TransDesc; }
private:
	CTransform::TRANSFORMDESC m_TransDesc;
	afx_msg void ApplyObjectTransformation();

public:
	_float3 m_vScale = _float3(1.f, 1.f, 1.f);
	_float m_fRotateY = 0.f;

public:
	CListBox m_ListBoxMapObject;
	void AddStringToListBox(string strName);
	size_t GetSelectListBoxSel(void) { return m_ListBoxMapObject.GetCurSel(); }

public:
	afx_msg void ApplySelectObjectTextureNumber();
	CEdit m_EditTextureNumber;
};

END