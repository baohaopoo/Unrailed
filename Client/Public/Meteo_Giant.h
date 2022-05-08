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
	_float		ObjectTurn = -20.f;
	_float3 vPos{}; // �ʱ� ���彺���̽��� ��ġ��ǥ
	_float3 vUp{}; // �ʱ� ���彺���̽��� Y��
	D3DXVECTOR3 m_vPosition;
	_float3 fPos;
	_float fSize = 0.39f;
public:
	static CMeteo_Giant* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END