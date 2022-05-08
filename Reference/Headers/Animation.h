#pragma once

#include "Component.h"

/* 1. ��ü���� �׸��¼����� ���� �׷����� ��Ƴ��´�. */
/* 2. ��Ƴ��� ��ü���� �����Լ��� ȣ�����ش�.  */
/* 3. �׷쿡 �´� �������¸� �����Ѵ�.  */

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
	D3DXVECTOR3				m_AniPos[2];	// ��ġ ������ Ű��
	D3DXQUATERNION			m_AniRot[2];	// ȸ�� ������ Ű��
	
	_float4x4				matTMParent;	// �θ��� �̵����
	_float4x4				matRParent;		// �θ��� ȸ�����
	_float4x4				matTMChild;		// �ڽ��� �̵����
	_float4x4				matRChild;		// �ڽ��� ȸ�����

	_float					fRot = 0.f;		// ������

public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

