#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "Pick_Manager.h"
#include "Layer.h"
#include "Collision_Manager.h"
#include "Frustum_Manager.h"

/* 클라이언트에 보여줘야할 인터페이스를 보관하고. 보여준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For. Tool */
	void Tick_Engine(_uint iNumLevel, _float fTimeDelta);

public: /* For.GameInstance */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, LPDIRECT3DDEVICE9* ppGraphic_Device);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();	
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */	
	void Render_Begin(void);
	void Render_End();

public: /* For.Input_Device */
	bool Get_DIKeyState(_uchar byKeyID);
	_char Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	_long Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState);

public: /* For.Timer_Manager */
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);
	HRESULT ChangeLevel(_uint iLevelIndex);	
	HRESULT SetLevel(_uint iNumLevels, class CLevel* pLevel);	
	CLevel* GetCurrLevel(void);

public: /* For.Object_Manager */
	CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pObject);
	HRESULT Delete_Layer(_uint iLevelIndex, const _tchar * pLayerTag);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject* Find_Object(_uint iLevelIndex, const _tchar* pLayerTag, const string& _strObjectName);
	map<const _tchar*, class CLayer*>& Find_LevelLayers(_uint iLevelIndex);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	 
public: /* For.Pick_Manager */
	CPick_Manager::POLYGON* Picking(_uint _iNumLevel);
	CGameObject* Picking_Layer(_uint _iNumLevel, const _tchar* _tagLayer); 
	CGameObject* Picking_Object(_uint _iNumLevel);
	_float3 TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3);
	_float3 TriBevelMiddle(CPick_Manager::POLYGON & polygon);
	HRESULT InitPickManager(HWND hWnd, _uint _iWinCX, _uint _iWinCY, _uint _iStartWinPosX = 0, _uint _iStartWinPosY = 0);
	void MatchPrecisionPoint(_float3& _fPoint);
	float Pick(D3DXVECTOR3& _p1, D3DXVECTOR3& _p2, D3DXVECTOR3& _p3); 
	void Tick(void);

public:	/* For.Collision_Manager */
	HRESULT AddCollisionLayers(_uint _iNumLevel, const TCHAR* _pLayerTag1, const TCHAR* _pLayerTag2);
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);

public:	/* For.Frustum_Manager */
	bool Check_InOut(const _float4x4& Object_WorldMatrix);


private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CPick_Manager*				m_pPick_Manager = nullptr;
	CCollision_Manager*			m_pCollision_Manager = nullptr;
	CFrustum_Manager*			m_pFrustum_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END