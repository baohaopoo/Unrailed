#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class VIBufferUI;
END

BEGIN(Client)
class ProgressBar final : public CGameObject
{
private:
	ProgressBar(LPDIRECT3DDEVICE9 pGrahpic_Device);
	ProgressBar(const ProgressBar& rhs);
	virtual ~ProgressBar() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void LoadingGo();
	bool m_isGo = false;
	int	goCnt = 0;
	bool m_isCol = false;
	bool m_isRender = false;
	float fTime = 0.f;
private:
	/* 움직인다.(상태를 표현한다.) */
	CTransform*			m_pTransformCom = nullptr;

	/* 어떤모양으로 그릴건지?! (모델) */
	VIBufferUI*		m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

	_float				fCnt = 0.f;
	_float				cnt = 0;
private:
	HRESULT SetUp_Components();
	LPD3DXMESH mesh;
	int startcnt = 0;

	LPD3DXFONT font;
	_float4x4			m_ProjMatrix; //직교 투영
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_uint speed = 0;
	_float timeControl = 0.f;

public:
	static ProgressBar* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);


};

END