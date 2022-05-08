#pragma once

#include "Component.h"
#include "VIBuffer_Cube.h"
#include "Transform.h"
#include "Texture.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent{
public:
	// 충돌체의 로컬에서의 이동을 위한 행렬과 해당 충돌체를 월드에서 보여줄 월드 행렬을 가진 컴포넌트를 인자로 받는다. 
	typedef struct _tagColliderDesc {
		_float4x4	localMatrix;
		CTransform* pTransform;

		struct _tagColliderDesc(
			_float4x4 _LocalMatrix = _float4x4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
			CTransform* _pTransform = nullptr
		)
			: localMatrix(_LocalMatrix), pTransform(_pTransform)
		{}
	}ColliDesc;

protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	virtual HRESULT Render();

private:
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	ColliDesc			m_ColDesc;
	CTransform*			m_pTransform = nullptr;

	map<_float, CGameObject*>	m_ColObjects;

public:
	const ColliDesc&	Get_ColDesc(void) const { return m_ColDesc; }

public:
	// 충돌체 VIBuffer 관련 함수
	CVIBuffer_Cube*		Get_ColliderVIBuffer(void) const { return m_pVIBufferCom; }
	const _float4x4&	Get_WorldMatrix(void) const { return m_pTransform->Get_WorldMatrix(); }
	const _float4x4&	Get_LocalMatrix(void) const { return m_pTransform->Get_TransformState().m_LocalTransform; }

public:
	// 충돌된 오브젝트들 관련, 매 프레임마다 m_ColObjects에 있는 오브젝트의 정보가 갱신된다.
	void	Push_ColObject(CGameObject* _pObject);
	map<_float, CGameObject*>& Get_ColObjects(void) { return m_ColObjects; }
	CGameObject* Get_CollisionObject(void);

private:
	HRESULT SetUp_Components(void * pArg);

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END