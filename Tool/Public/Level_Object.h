#pragma once

#include "Tool_Defines.h"
#include "Level.h"
#include "Pick_Manager.h"
#include "GameObject.h"

BEGIN(Tool)

class CLevel_Object final : public CLevel
{
public:
	enum MODE { MODE_CREATE, MODE_SELECT, MODE_CAMERA, MODE_END };

private:
	explicit CLevel_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Object() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Cube(const _tchar* pLayerTag);

private:
	// For Axis
	void SetRenderState();
	void DrawAxis();

private:
	// For Picking and AddObject;
	HRESULT CreateVIBuffer(VIBuffer _eType);		// ���ϴ� ����� ������Ʈ�� ���ӿ�����Ʈ�� VIBuffer�� �߰��ϴ� �Լ�
	HRESULT DeleteVIBuffer(void);					// �� �����ӿ��� ��ŷ�� ������Ʈ�� �����ϸ� �ش� ������Ʈ�� ���ӿ�����Ʈ�� VIBuffer���� �����ϴ� �Լ�
	CGameObject* SelectVIBuffer(void);				// �� �����ӿ��� ��ŷ�� ������Ʈ�� �����ϸ� �ش� ������Ʈ�� ��ȯ�ϴ� �Լ�
	void	UpgradePrecision(CPick_Manager::POLYGON& polygon);

public:
	static CLevel_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	// ���ڷ� ���� ����� ������Ʈ�� ���ڷ� ���� ��ŷ�� �������� Ư�� ���⿡ �����ϴ� �Լ�
	_float3 TriBevelMiddle(_float3& p1, const _float3& p2, const _float3& p3);
	_float3 TriBevelMiddle(CPick_Manager::POLYGON& polygon);

public:
	// ��� ���� �Լ�
	void changeMode(MODE _eModeType) { m_eCurrMode = _eModeType; }
	MODE GetCurrMode(void) { return m_eCurrMode; }
	CGameObject* GetSelectObject(void) { return m_pSelectObject; }

//public:
//	// Save & Load
//	HRESULT Save(const TCHAR* _strFilePath);
//	HRESULT Load(const TCHAR* _strFilePath);

private:
	bool	m_bLButtonClick = false;
	bool	m_bRButtonClick = false;

	MODE	m_eCurrMode;
	CGameObject* m_pSelectObject = nullptr;
};

END