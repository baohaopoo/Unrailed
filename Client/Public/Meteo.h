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
	/* 움직인다.(상태를 표현한다.) */
	CTransform*			m_pTransformCom = nullptr;

	/* 어떤모양으로 그릴건지?! (모델) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

	CCollider*			m_pColliderCom = nullptr;

private:
	HRESULT SetUp_Components(void* pArg);

private:
	bool m_isCol = false;
	_float		fEffectTime = 0.f;
	_float		fTime = 0.f;
	_float3 vPos{}; // 초기 월드스페이스의 위치좌표
	_float3 vUp{}; // 초기 월드스페이스의 Y축
	D3DXVECTOR3 m_vPosition;
	_float3 fPos;
public:
	static CMeteo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END