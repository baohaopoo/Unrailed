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



class ExitBtn final : public CGameObject
{
public:
	explicit ExitBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	ExitBtn(const ExitBtn& rhs);
	virtual ~ExitBtn() = default;

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
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

	HRESULT Ready_Layer_ProgressBarExit(const _tchar* pLayerTag); //���α׷��� �� ���뵵

																  //��������
																  //��������

private:
	_float4x4 m_ProMatrix;
	_float m_fX, m_fY, m_fSizeX, m_fSizeY;


private:
	HRESULT SetUp_Components();
	LPD3DXFONT font;
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
	static ExitBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;



};

END