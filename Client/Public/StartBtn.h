#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
END
BEGIN(Client)

class StartBtn final : public CGameObject
{
public:
	explicit StartBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	StartBtn(const StartBtn& rhs);
	virtual ~StartBtn() = default;


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
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;


public:
	HRESULT Ready_Layer_ProgressBar(const _tchar* pLayerTag); //프로그레스 바 띄울용도
private:
	HRESULT SetUp_Components();
	LPD3DXFONT font;
	LPD3DXFONT font2; //로비용
	LPD3DXFONT font3; //로비용
	LPD3DXMESH mesh;
	bool CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2);
	
	bool m_bStartLoading = false;
	_float m_LoadingTime = 0.f;


	bool m_isDraw = true;
	bool m_isCol = false;
	DWORD dwTime; 
	int i = 0;
	int cnt = 0;
public:
	static StartBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END