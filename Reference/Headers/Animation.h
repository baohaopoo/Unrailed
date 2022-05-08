#pragma once

#include "Component.h"

/* 1. 객체들을 그리는순서에 따라 그룹지어 모아놓는다. */
/* 2. 모아놓은 객체들의 렌더함수를 호출해준다.  */
/* 3. 그룹에 맞는 렌더상태를 설정한다.  */

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CComponent
{
public:
	enum RENDERGROUP { GROUP_PRIORITY, GROUP_NONBLEND, GROUP_ALPHABLEND, GROUP_UI, GROUP_END };
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CAnimation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	void					InitAnimation();
	_float					Linear(_float v0, _float v1, _float t);
	HRESULT					Animation();
private:
	D3DXVECTOR3				m_AniPos[2];	// 위치 포지션 키값
	D3DXQUATERNION			m_AniRot[2];	// 회전 포지션 키값
	
	_float4x4				matTMParent;	// 부모의 이동행렬
	_float4x4				matRParent;		// 부모의 회전행렬
	_float4x4				matTMChild;		// 자식의 이동행렬
	_float4x4				matRChild;		// 자식의 회전행렬

	_float					fRot = 0.f;		// 아직모름

public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

