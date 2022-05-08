#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
END

BEGIN(Client)

class CGrass final : public CGameObject
{
public:
	typedef struct _tagTreeDesc {
		CTransform::TRANSFORMDESC TransDesc;
		int iTreeNumber;
	}TreeDESC;

private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGrass(const CGrass& rhs);
	virtual ~CGrass() = default;

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

private:
	HRESULT SetUp_Components(void* pArg);

private:
	int			m_iLife;

public:
	// �������� 1 �پ���, VIBuffer�� ��ü�ȴ�.
	void Get_Damage(void);

public:
	static CGrass* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END