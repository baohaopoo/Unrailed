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

BEGIN(Client)

class CRock final : public CGameObject
{
public:
	typedef struct _tagRockDesc {
		CTransform::TRANSFORMDESC TransDesc;
		int iRockNumber;
	}ROCKDESC;

private:
	explicit CRock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRock(const CRock& rhs);
	virtual ~CRock() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	void Dynamite();
private:
	HRESULT	Ready_Layer_Effect(OBJECT_TYPE eType);
	HRESULT	Ready_Layer_reward(const _tchar* pLayerTag);
private:
	/* �����δ�.(���¸� ǥ���Ѵ�.) */
	CTransform*			m_pTransformCom = nullptr;
	
	/* �������� �׸�����?! (��) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;	

	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;
private:

	HRESULT SetUp_Components(void* pArg);
	//��¿�� �����ڵ�
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);
private:
	_float		m_fEffectTime = 0.f;
	int			m_iLife;

	// �������� 1 �پ���, VIBuffer�� ��ü�ȴ�.
	
	bool m_isCol = false;
	bool m_isEffect = false;
	bool m_isRender = true;
	bool m_isDead = false;
	bool m_isMyCol = false;
	bool m_isMade = false;
public:
	void Get_Damage(void);
	static CRock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END