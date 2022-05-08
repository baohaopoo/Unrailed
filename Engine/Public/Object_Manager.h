#pragma once

#include "Base.h"

/*1. 내 게임에 사용될 객체(사본)들을 보관하고 관리한다. */
/*2. 원형객체들을 보관한다. */
/*3. 사본객체들의 갱신을 담당한다.(Tick함수호출) */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Find_Object(_uint iLevelIndex, const _tchar* pLayerTag, const string& _strObjectName);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pPrototype);
	HRESULT Delete_Layer(_uint iLevelIndex, const _tchar * pLayerTag);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	HRESULT Clear(_uint iLevelIndex);

public:
	// save, load 관련
	HRESULT Save_Data(const TCHAR* _strFilePath, CGameObject* _pGameObject);
	HRESULT Load_Data(const TCHAR* _strFilePath, _uint iLevelIndex, const _tchar* pLayerTag);

public:
	// Tool 관련 함수
	void Tick(_uint iNumLevel, _float fTimeDelta);				// 특정 레벨의 오브젝트만 Tick하는 함수
	void LateTick(_uint iNumLevel, _float fTimeDelta);			// 특정 레벨의 오브젝트만 Late_Tick하는 함수

public:
	map<const _tchar*, class CLayer*>& Find_LevelLayers(_uint iLevelIndex);

private:
	/* 원형객체들을 보관한다. */
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
		
	
private:
	/* 사본객체들을 레이어로구분(니 맘대로)하여 보관한다.  */
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;

private:
	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);

public:
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END