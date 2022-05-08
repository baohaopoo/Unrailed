#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	virtual HRESULT Render();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	_uint							m_iNumVertices = 0;
	_uint							m_iStride = 0; /* 정점하나의 메모리 크기. */
	_ulong							m_dwFVF = 0;
	D3DPRIMITIVETYPE				m_ePrimitiveType;
	_uint							m_iNumPrimitives = 0;
	void*							m_pVertices = nullptr;

protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iPrimitiveIndicesSize = 0;
	D3DFORMAT						m_eFormat;
	void*							m_pIndices = nullptr;

protected:
	VIBuffer						m_eVIBuffer;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual HRESULT SaveVIBuffer(const string& _strFilePath);
	VIBuffer	Get_Type(void) { return m_eVIBuffer; }
	void*		Get_Vertices(void) { return m_pVertices; }
	void*		Get_Indices(void) { return m_pIndices; }
	_uint		Get_NumVertices(void) { return m_iNumVertices; }
	_uint		Get_NumPrimitives(void) { return m_iNumPrimitives; }

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END