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
	int			m_iLife;
	bool		m_isCol = false;
	bool		m_isRender = true;

	_float		m_fEffectTime = 0.f;
	_float		m_dynamiteTime = 0.f;

	random_device rd;
	std::uniform_int_distribution <int> uid{ 22,24 };
	std::default_random_engine dre;

public:
	// 라이프가 1 줄어들고, VIBuffer가 교체된다.
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