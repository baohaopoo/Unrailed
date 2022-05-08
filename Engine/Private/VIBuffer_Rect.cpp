#include "..\Public\VIBuffer_Rect.h"



CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
	m_eVIBuffer = VIBuffer::VIB_RECT;
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{
	m_eVIBuffer = VIBuffer::VIB_RECT;
}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	// m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE3(1);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 2;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0/*m_iStride * m_iNumVertices*/, (void**)&pVertices, 0);

	/* 값을 바꾸고. */
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_eFormat = D3DFMT_INDEX16;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	FACELISTINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Rect::Free()
{
	__super::Free();
}
