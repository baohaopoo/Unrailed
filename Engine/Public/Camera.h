#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3		vEye, vAt, vAxisY;
		_float		fFovy, fNear, fFar, fAspect;

		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	class CTransform*				m_pTransform = nullptr;
	CAMERADESC						m_CameraDesc;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END