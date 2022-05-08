#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include <random>
BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CTree final : public CGameObject
{
public:
	typedef struct _tagTreeDesc {
		CTransform::TRANSFORMDESC TransDesc;
		int iTreeNumber;
	}TreeDESC;

private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTree(const CTree& rhs);
	virtual ~CTree() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	void Dynamite();
private:
	HRESULT	Ready_Layer_reward(const _tchar* pLayerTag);

	HRESULT	Ready_Layer_Effect(OBJECT_TYPE eType);

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
	int			m_iLife;
	bool		m_isCol = false;
	bool		m_isRender = true;

	_float		m_fEffectTime = 0.f;
	_float		m_dynamiteTime = 0.f;

	random_device rd;
	std::uniform_int_distribution <int> uid{ 22,24 };
	std::default_random_engine dre;

public:
	// �������� 1 �پ���, VIBuffer�� ��ü�ȴ�.
	void Get_Damage(void);
	bool Making_Wood();
	_float3 getWoodPos();
	bool goMakeWood = false;
	_float3 woodPos = {};
public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END