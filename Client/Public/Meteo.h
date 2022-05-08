#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include <random>
BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CMeteo final : public CGameObject {
private:
	explicit CMeteo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMeteo(const CMeteo& rhs);
	virtual ~CMeteo() = default;
private:
	default_random_engine dre;
	uniform_int_distribution<int> uid{ -30, 30 };


public:
	void Boom();

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

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

private:
	bool m_isCol = false;
	_float		fEffectTime = 0.f;
	_float		fTime = 0.f;
	_float3 vPos{}; // �ʱ� ���彺���̽��� ��ġ��ǥ
	_float3 vUp{}; // �ʱ� ���彺���̽��� Y��
	D3DXVECTOR3 m_vPosition;
	_float3 fPos;
public:
	static CMeteo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END