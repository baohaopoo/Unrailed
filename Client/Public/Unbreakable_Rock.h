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

class CUnbreakable_Rock final : public CGameObject
{
public:
	typedef struct _tagRockDesc {
		CTransform::TRANSFORMDESC TransDesc;
		int iRockNumber;
	}ROCKDESC;

private:
	explicit CUnbreakable_Rock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUnbreakable_Rock(const CUnbreakable_Rock& rhs);
	virtual ~CUnbreakable_Rock() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	void Meteo();
private:
	/* �����δ�.(���¸� ǥ���Ѵ�.) */
	CTransform*			m_pTransformCom = nullptr;
	
	/* �������� �׸�����?! (��) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;	

	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

	// �浹ü ����
	CCollider*			m_pColliderCom = nullptr;
	_uint			m_iLife;
private:
	HRESULT SetUp_Components(void* pArg);

public:
	static CUnbreakable_Rock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END