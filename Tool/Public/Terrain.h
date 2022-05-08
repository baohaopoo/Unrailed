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

// ���⼭�� �ͷ����� ���� ����� �� ��ü�� ������ �� ������, �ش� ��ü�� ������ �ٲ� �� �־�� �Ѵ�.
// ��, ��� ��ü�� ������Ʈ �Ŵ����� ����־���Ѵ�.
class CTerrain final : public CGameObject
{
public:
	typedef struct _tagMapDesc {
		size_t iMapSizeX;		// ���� ���� / ���� ����(����)
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