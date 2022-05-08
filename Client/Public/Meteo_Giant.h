#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CMeteo_Giant final : public CGameObject {
private:
	explicit CMeteo_Giant(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMeteo_Giant(const CMeteo_Giant& rhs);
	virtual ~CMeteo_Giant() = default;

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
	_float		ObjectTurn = -20.f;
	_float3 vPos{}; // 초기 월드스페이스의 위치좌표
	_float3 vUp{}; // 초기 월드스페이스의 Y축
	D3DXVECTOR3 m_vPosition;
	_float3 fPos;
	_float fSize = 0.39f;
public:
	static CMeteo_Giant* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END