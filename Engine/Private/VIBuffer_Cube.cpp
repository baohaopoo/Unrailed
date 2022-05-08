#include "..\Public\VIBuffer_Cube.h"
#include "Pick_Manager.h"
#include "GameInstance.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
	m_eVIBuffer = VIBuffer::VIB_CUBE;
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	: CVIBuffer(rhs)
{
	m_eVIBuffer = VIBuffer::VIB_CUBE;
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iNumVertices = 8;
	m_iStride = sizeof(VTXCUBETEX);	
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 12;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXCUBETEX[m_iNumVertices];

	VTXCUBETEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0/*m_iStride * m_iNumVertices*/, (void**)&pVertices, 0);

	/* 값을 바꾸고. */
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;

	memcpy(m_pVertices, pVertices, sizeof(VTXCUBETEX) * m_iNumVertices);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_eFormat = D3DFMT_INDEX16;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACELISTINDICES16[m_iNumPrimitives];

	FACELISTINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* +x */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* -x */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* +y */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* -y */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* +z */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* -z */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;	

	memcpy(m_pIndices, pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitives);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype(D3DXMATRIX& LocalMatrix)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	NativeConstruct_Prototype();

	VTXCUBETEX*		pVertices = nullptr;
	m_pVB->Lock(0, 0/*m_iStride * m_iNumVertices*/, (void**)&pVertices, 0);

	/* 값을 바꾸고. */
	for (int i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&pVertices[i].vPosition, &pVertices[i].vPosition, &LocalMatrix);
	}

	memcpy(m_pVertices, pVertices, sizeof(VTXCUBETEX) * m_iNumVertices);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	
	_float4x4 LocalMatrix{};

	if (nullptr == pArg)
		return S_OK;
	else {
		LocalMatrix = *((_float4x4*)pArg);
	}

	NativeConstruct_Prototype(LocalMatrix);

	return S_OK;
}

HRESULT CVIBuffer_Cube::SaveVIBuffer(const string& _strFilePath)
{
	// 큐브는 8개의 정점을 가지고 있다.
	// 우선 해당하는 객체의 정보를 저장한 후, 이에 맞는 갯수의 정점들을 저장한다.
	ofstream out(_strFilePath, std::ios::binary | std::ios::app);

	if (out){
		out << VIBuffer::VIB_CUBE << endl;
		for (int i = 0; i < m_iNumVertices; ++i) {
			out << ((VTXTEX*)m_pVertices)[i].vPosition.x << " ";
			out << ((VTXTEX*)m_pVertices)[i].vPosition.y << " ";
			out << ((VTXTEX*)m_pVertices)[i].vPosition.z << endl;
		}
	}

	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, D3DXMATRIX& LocalMatrix)
{
	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(LocalMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _float3 & vFocus)
//{
//	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(pGraphic_Device);
//
//	if (FAILED(pInstance->NativeConstruct_Prototype(vFocus)))
//	{
//		MSG_BOX(TEXT("Failed to Created CVIBuffer_Cube"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CComponent * CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();


}
