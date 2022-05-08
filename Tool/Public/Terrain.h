#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"
#include "Cube_Object.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Tool)

// 여기서의 터레인은 맵을 만들고 각 객체를 선택할 수 있으며, 해당 객체의 정보를 바꿀 수 있어야 한다.
// 즉, 모든 객체가 오브젝트 매니저에 들어있어야한다.
class CTerrain final : public CGameObject
{
public:
	typedef struct _tagMapDesc {
		size_t iMapSizeX;		// 맵의 가로 / 세로 길이(갯수)
		size_t iMapSizeY;
	}MAPDESC;

private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components(void* pArg);

private:
	//vector<CGameObject*> m_vObjects;
	MAPDESC m_MapInfo;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
};

END