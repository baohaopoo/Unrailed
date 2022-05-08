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

class CRail final : public CGameObject {
public:
	enum RAIL_TYPE {
		RAIL_STRAIGHT_LEFT,
		RAIL_STRAIGHT_RIGHT,
		RAIL_STRAIGHT_UP,
		RAIL_STRAIGHT_DOWN,

		RAIL_ROTATE_LEFT_UP,
		RAIL_ROTATE_LEFT_DOWN,

		RAIL_ROTATE_RIGHT_UP,
		RAIL_ROTATE_RIGHT_DOWN,

		RAIL_ROTATE_UP_LEFT,
		RAIL_ROTATE_UP_RIGHT,

		RAIL_ROTATE_DOWN_LEFT,
		RAIL_ROTATE_DOWN_RIGHT,

		RAIL_INVISIBLE,

		RAIL_ENDING,

		RAIL_END
	};

	typedef struct _tagRailDesc {
		CTransform::TRANSFORMDESC TransDesc;
		RAIL_TYPE eType_Rail;
	}RAILDESC;

private:
	explicit CRail(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRail(const CRail& rhs);
	virtual ~CRail() = default;

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
	static CRail* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void Change_RailType(RAIL_TYPE eType);
	RAIL_TYPE Get_RailType(void) { return m_eRailType; }

private:
	RAIL_TYPE m_eRailType = RAIL_END;
};

END