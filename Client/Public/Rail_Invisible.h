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

class CRail_Invisible final : public CGameObject {
private:
	explicit CRail_Invisible(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRail_Invisible(const CRail_Invisible& rhs);
	virtual ~CRail_Invisible() = default;

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

public:
	static CRail_Invisible* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	CRail::RAIL_TYPE Get_RailType(void) { return m_eRailType; }
	void ShowVisible(bool _bVisible) { m_bVisible = _bVisible; }

private:
	bool m_bVisible = false;
	CRail::RAIL_TYPE m_eRailType = CRail::RAIL_END;
};

END