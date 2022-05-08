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

class CMaterial_Wood final : public CGameObject {
private:
	explicit CMaterial_Wood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMaterial_Wood(const CMaterial_Wood& rhs);
	virtual ~CMaterial_Wood() = default;

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
	
	//�ݶ��̴�
	CCollider*			m_pColliderCom = nullptr;
	

public:
	void Stack();
private:
	bool m_isCol = false;
	HRESULT SetUp_Components(void* pArg);
	_float deltaTime = 0.f;
public:
	static CMaterial_Wood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END