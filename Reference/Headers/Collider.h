#pragma once

#include "Component.h"
#include "VIBuffer_Cube.h"
#include "Transform.h"
#include "Texture.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent{
public:
	// �浹ü�� ���ÿ����� �̵��� ���� ��İ� �ش� �浹ü�� ���忡�� ������ ���� ����� ���� ������Ʈ�� ���ڷ� �޴´�. 
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
	// �浹ü VIBuffer ���� �Լ�
	CVIBuffer_Cube*		Get_ColliderVIBuffer(void) const { return m_pVIBufferCom; }
	const _float4x4&	Get_WorldMatrix(void) const { return m_pTransform->Get_WorldMatrix(); }
	const _float4x4&	Get_LocalMatrix(void) const { return m_pTransform->Get_TransformState().m_LocalTransform; }

public:
	// �浹�� ������Ʈ�� ����, �� �����Ӹ��� m_ColObjects�� �ִ� ������Ʈ�� ������ ���ŵȴ�.
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