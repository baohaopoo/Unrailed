#pragma once

#include "Component.h"

/* 객체의 월드상태를 표현하는 월드행렬을 가지고 있는다. */
/* 객체에게 필요한 월드 변환의 기능을 가진다. (직진, 후진, 회전 ) */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
		_float		fScalePerSec;

		mutable _float4x4	m_LocalMatrix;		// 객체의 로컬에서의 행렬이다. RST순으로 곱하여져 있다.
		_float4x4	m_LocalRotate;		// 객체의 로컬에서의 회전 행렬이다.
		_float4x4	m_LocalScale;		// 객체의 로컬에서의 크기 행렬이다.
		_float4x4	m_LocalTransform;	// 객체의 로컬에서의 이동 행렬이다.

		_uint		m_iTextureNumber;	// 사용할 텍스쳐 번호, 로드한 텍스쳐의 번호를 알고 있어야 된다.

		mutable _float4x4	m_WorldMatrix;		// 객체의 월드에서의 행렬이다.

		struct tagTransformDesc(
			_float _fSpeedPerSec = 0.f, _float _fRotationPerSec = 0.f, _float _fScalePerSec = 0.f,
			_float4x4 _LocalMatrix = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			_float4x4 _LocalRotate = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			_float4x4 _LocalScale = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			_float4x4 _LocalTransform = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			_float4x4 _WorldTransform = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			_uint _iTextureNumber = 0,
			_float4x4 _WorldMatrix = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
		)
			:
			fSpeedPerSec(_fSpeedPerSec), fRotationPerSec(_fRotationPerSec), fScalePerSec(_fScalePerSec),
			m_LocalMatrix(_LocalMatrix), m_LocalRotate(_LocalRotate), m_LocalScale(_LocalScale), m_LocalTransform(_LocalTransform),
			m_iTextureNumber(_iTextureNumber), m_WorldMatrix(_WorldMatrix)
		{ }

	}TRANSFORMDESC;

private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
	}

	_float3 Get_LocalState(STATE eState) {
		return *(_float3*)&m_TransformDesc.m_LocalMatrix.m[eState][0];
	}
	void Set_LocalState(STATE eState, const _float3& vState) {
		memcpy(&m_TransformDesc.m_LocalMatrix.m[eState][0], &vState, sizeof(_float3));
	}

	void Set_TransformState(const TRANSFORMDESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

	const TRANSFORMDESC& Get_TransformState(void) const {
		return m_TransformDesc;
	}

	_float3 Get_Scale();

	const _float4x4& Get_WorldMatrix() const {
		return m_WorldMatrix;
	}
	void Set_WorldMatrix(const _float4x4& _WorldMatrix) {
		m_WorldMatrix = _WorldMatrix;
	}

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Bind_OnGraphicDevice() const;

public:
	HRESULT Go_Straight(_float fTimeDelta);
	HRESULT Go_BackWard(_float fTimeDelta);
	HRESULT Go_Right(_float fTimeDelta);
	HRESULT Go_Left(_float fTimeDelta);
	HRESULT CameraShake(_float fTimeDelta);
	HRESULT LiftUp(_float fTimeDelta);
	HRESULT LiftDown(_float fTimeDelta);
	HRESULT Scaled(_float3 vScale);
	HRESULT Scaling(_float fTimeDelta);
	HRESULT LookAtplayer(const _float3 & vTargetPos);
	HRESULT Turn(const _float3& vAxis, _float fTimeDelta);	// 시간에 따른 회전각을 사용하여 회전하는 함수
	HRESULT Turned(const _float3& vAxis, _float _fAngle);	// 현재 각도에서 갱신하여 추가로 회전하는 함수
	HRESULT TurnAround(const _float3& vAxis, DIRECTION _eDir);	// 원본 각도(0도)에서 원하는 만큼 회전하는 함수

	// 카메라와 원점 사이의 거리 _fRadius, x축 회전량 phi, y축 회전량 _theta, 이동 시간 _fTimeDelta
	HRESULT TurnByOrigin(_float _fRadius, _float _phi, _float _theta, _float _fTimeDelta);

	HRESULT Chase(const _float3& vTargetPos, _float fTimeDelta, _float fLimitDistance = 1.f);
	HRESULT LookAt(const _float3& vTargetPos);



	//UIObject 를 위한 용
	HRESULT Go_Up(_float fTimeDelta);
	HRESULT Go_Down(_float fTimeDelta);
	HRESULT Go_JustRight(_float fTimeDelta);
	HRESULT Go_JustLeft(_float fTimeDelta);


private:
	DIRECTION			m_eDir;
	_float4x4			m_WorldMatrix;		// 객체의 월드 변환 행렬
	TRANSFORMDESC		m_TransformDesc;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END