#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rail.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CRail_Ending final : public CGameObject {
private:
	explicit CRail_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRail_Ending(const CRail_Ending& rhs);
	virtual ~CRail_Ending() = default;

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

private:
	HRESULT SetUp_Components(void* pArg);

public:
	static CRail_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	CRail::RAIL_TYPE Get_RailType(void) { return m_eRailType; }

private:
	CRail::RAIL_TYPE m_eRailType = CRail::RAIL_ENDING;
};

END