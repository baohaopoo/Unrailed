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
	/* �����δ�.(���¸� ǥ���Ѵ�.) */
	CTransform*			m_pTransformCom = nullptr;

	/* �������� �׸�����?! (��) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;

	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;

protected:
	virtual HRESULT SetUp_TrainComponents(void* pArg);
	HRESULT SetUp_VIBuffer(const TCHAR* strFileName);

	// ������ �̵��� ���ú� ���� �� �Լ����̴�.
	// ===========================================================================================
protected:
	int		m_iRailNumber = 0;	// ���� ���° ������ ���󰡰� �ִ��������� ����
	bool	m_bMove = false;
	bool	m_bArrive = false;
	_float	m_fStartTrain = 0.f;
	_float  m_fStartDelay = 0.f;
	_float	m_fAngle = 0.f;
	_float	m_fTurnAngle = 0.f;

public:
	bool bMove(void) { return m_bMove; }

protected:
	void StartMove();		// ������ �� �ش� �Լ��� �ҷ��ش�. �׷��� ���� �Ŀ� �����̱� �����Ѵ�.
	void Follow_Rail(_float _fDeltaTime);	// ������ ���󰡴µ� ���õ� �Լ�
	void FailToArrive();	// ������ �������� �������� ������ �� �ҷ��ִ� �Լ�
	void Bomb();	// ������ ������ ����Ʈ�� �ҷ��ִ� �Լ� 

public:
	void Arrive();			// ������ �������� �������� �� �ҷ��ִ� �Լ�

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

