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

class CGiant_LeftHand final : public CGameObject {
private:
	explicit CGiant_LeftHand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGiant_LeftHand(const CGiant_LeftHand& rhs);
	virtual ~CGiant_LeftHand() = default;

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
	bool ObjectRotate = false;
	float ObjectTurn = 0.f;
	float fTime = 0.f;
	float fLeftTurn = -0.1f;
	float fRightTurn = 0.1f;
	float fMeteo = 0.f;
	float fMeteoDestroy = 0.f;
	int createMeteo = 0;
public:
	static CGiant_LeftHand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END