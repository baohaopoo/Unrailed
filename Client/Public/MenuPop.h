#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END
BEGIN(Client)

class MenuPop final : public CGameObject
{
public:
	MenuPop(LPDIRECT3DDEVICE9 pGraphic_Device);
	MenuPop(const MenuPop& rhs);
	virtual ~MenuPop() = default;



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
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	LPD3DXFONT font;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	bool		m_isRender = true;
public:
	static MenuPop* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END