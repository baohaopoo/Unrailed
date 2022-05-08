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
	// 레이어 끼리 충돌처리를 할 것이기 때문에, 레이어 2개를 인자로 가지는 vector를 사용한다.
	// 추가하고 딱히 지우지 않을것이기 때문에 vector로 한다.
	// 딱히 여러 레벨에서 충돌처리를 하는것이 아닌 게임플레이 레벨에서만 충돌처리를 할 것이기 때문에 그냥 이것만 쓴다.
	vector<pair<CLayer*, CLayer*>>	m_vCollision;

	// Collision 오브젝트를 전부 렌더해줘야되는데, 같은 오브젝트가 여럿들어갈 수 있으므로, 이를 처리하기 위해 set을 사용하여 중복을 제거한다.
	// 같은 오브젝트가 추가되면 처음 들어간 값만 허용된다.
	set<CGameObject*>	m_CollisionObject;
};

END

