#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CLayer;
class CGameObject;
class CVIBuffer;
class CGameObject;
END

BEGIN(Engine)

class CCollision_Manager final : public CBase {

	DECLARE_SINGLETON(CCollision_Manager)

private:
	explicit CCollision_Manager();
	explicit CCollision_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual ~CCollision_Manager() = default;

public:
	static CCollision_Manager* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);

public:
	HRESULT AddCollisionLayers(_uint _iNumLevel, const TCHAR* _pLayerTag1, const TCHAR* _pLayerTag2);
	bool CheckIntersect(_float3& center1, float& radius1, _float3& center2, float& radius2);
	void Tick(void);
	void LateTick(void);
	void Render(void);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

public:
	virtual void Free() override;

private:
	// ���̾� ���� �浹ó���� �� ���̱� ������, ���̾� 2���� ���ڷ� ������ vector�� ����Ѵ�.
	// �߰��ϰ� ���� ������ �������̱� ������ vector�� �Ѵ�.
	// ���� ���� �������� �浹ó���� �ϴ°��� �ƴ� �����÷��� ���������� �浹ó���� �� ���̱� ������ �׳� �̰͸� ����.
	vector<pair<CLayer*, CLayer*>>	m_vCollision;

	// Collision ������Ʈ�� ���� ��������ߵǴµ�, ���� ������Ʈ�� ������ �� �����Ƿ�, �̸� ó���ϱ� ���� set�� ����Ͽ� �ߺ��� �����Ѵ�.
	// ���� ������Ʈ�� �߰��Ǹ� ó�� �� ���� ���ȴ�.
	set<CGameObject*>	m_CollisionObject;
};

END

