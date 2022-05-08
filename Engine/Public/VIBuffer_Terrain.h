#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pCubeMapPath);
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;

public:
	_float3 Compute_Height(class CTransform * pTerrainWorldMatrix, const _float3 & vPosition);

public:
	VTXTEX*		GetVertices(void) const { return (VTXTEX*)m_pVertices; }
	_uint		Get_VerticesX() { return m_iNumVerticesX; }
	_uint		Get_VerticesZ() { return m_iNumVerticesZ; }

public:
	_uint		m_iNumVerticesX, m_iNumVerticesZ;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		_uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pCubeMapPath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END