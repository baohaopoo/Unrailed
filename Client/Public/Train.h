#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

class CTrain abstract : public CGameObject {
protected:
	explicit CTrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain(const CTrain& rhs);
	virtual ~CTrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	/* 움직인다.(상태를 표현한다.) */
	CTransform*			m_pTransformCom = nullptr;

	/* 어떤모양으로 그릴건지?! (모델) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;

protected:
	virtual HRESULT SetUp_TrainComponents(void* pArg);
	HRESULT SetUp_VIBuffer(const TCHAR* strFileName);

	// 기차의 이동과 관련변 변수 및 함수들이다.
	// ===========================================================================================
protected:
	int		m_iRailNumber = 0;	// 현재 몇번째 레일을 따라가고 있는지에대한 변수
	bool	m_bMove = false;
	bool	m_bArrive = false;
	_float	m_fStartTrain = 0.f;
	_float  m_fStartDelay = 0.f;
	_float	m_fAngle = 0.f;
	_float	m_fTurnAngle = 0.f;

public:
	bool bMove(void) { return m_bMove; }

protected:
	void StartMove();		// 움직일 때 해당 함수를 불러준다. 그러면 몇초 후에 움직이기 시작한다.
	void Follow_Rail(_float _fDeltaTime);	// 레일을 따라가는데 관련된 함수
	void FailToArrive();	// 기차가 목적지에 도착하지 못했을 때 불러주는 함수
	void Bomb();	// 기차가 터지는 이펙트를 불러주는 함수 

public:
	void Arrive();			// 기차가 목적지에 도착했을 때 불러주는 함수

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

