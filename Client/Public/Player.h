#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	enum HOLDING_STATE { HS_NONE, HS_AXE, HS_PICKAXE, HS_BUCKET, HS_WOOD, HS_IRON, HS_RAIL1, HS_RAIL2, HS_RAIL3, HS_DYNMITE, HS_WATERBUCKET, HS_END };
	/*
	0. ��� ���� ����
	1. ��ġ
	2. ���
	3. �絿��
	4. �ڿ� - ����
	5. �ڿ� - ö
	6. ���� 1��
	7. ���� 2��
	8. ���� 3��
	*/

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	
private:
	int materialNum = 0;	//���(material wood/ material iron) 
	_float waterTime = 0.f;
private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_OnTerrain();
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);

public:
	HOLDING_STATE Get_HoldingState(void) const { return m_eHoldingState; }

private:
	void PushOut_Collider(_float _fDeltaTime);
	LPD3DXFONT font;

private:
	void PlayerMove(_float fTimeDelta);	//PlayerControl
	void Meteo(_float fTimeDelta);
	void Rain(_float fTimeDelta);
	void addArm(_float fTimeDelta);
	void pressReturn(_float fTimeDelta);
	void pressSpace(_float fTimeDelta);
	void RailTrainBomb(_float fTimeDelta);
	void createTile();
	void GetDynamite();
public:
	bool MissionBtnUpdate();
	bool m_getrail = false;
	bool takeout = false;
	_float waterbucketgo = 0.f;
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void PickingTool(_tchar* LayerName);
	void DropTool(_tchar* LayerName);
	bool GetUseTool() { return useTool; }
	void SetUseTool(bool use) { useTool = use; }

	//�浹��
	const _tchar* GetTypeName(OBJECT_TYPE obj);
	const _tchar* GetProtoName(OBJECT_TYPE obj);
	bool m_holdAxe = false;

private:

	DIRECTION	m_eDir;
   	bool m_isCol = false;
	int GrabObject = 0;
	float ObjectPlayerDis = 1.f;
	float timecontrol = 0.f;
	bool useTool = false;
	_float Time = 0.f;
	_float deltaTime = 0.f;
	int getTreeCnt = 0;
	_float raingo = 0.f;
	//Ȧ��üũ��
	bool m_hold = false;
	CGameObject* holdObj = nullptr;
	_float firetime = 0.f;

	//key��
	bool m_bPressEnter = false;
	bool m_bPressSpace = false;
	bool m_bPressButton = false;

	HOLDING_STATE m_eHoldingState = HS_RAIL3;

	LPD3DXFONT font2;

	// �κ����� ���� ����
	bool m_bVisible = false;
	_float m_fVisibleRailDeltaTime = 0.f;
	
private:
	// ���� ����
	void CreateRail(_float3 _vPos);		// ������ �����ϴ� �Լ�
	void WithDraw_LastRail();			// ������ ������ ȸ���ϴ� �Լ�
	void ShowRailPos(_float fTimeDelta);				// ������ ��� ��ġ���� �����ִ� �Լ�

private:
	bool colRailTrain = false;	// ���ϸ���� �Ҹ� �ѹ��� ������ ���� ����
	bool fireEnd = false;		// �� ���� �Ҹ� �ѹ��� ������ ���� ����
};

END