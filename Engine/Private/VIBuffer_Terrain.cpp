#include "..\Public\VIBuffer_Terrain.h"
#include "Transform.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
	m_eVIBuffer = VIBuffer::VIB_TERRAIN;
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
	m_eVIBuffer = VIBuffer::VIB_TERRAIN;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iStride = sizeof(VTXTEX);	
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	if (nullptr != m_pVertices) {
		Safe_Delete_Array(m_pVertices);
	}

	m_pVertices = new VTXTEX[m_iNumVertices];

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0/*m_iStride * m_iNumVertices*/, (void**)&pVertices, 0);

	/* 값을 바꾸고. */

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(
				(_float)(-0.5f * m_iNumVerticesX + j + 0.5f),
				0.f,
				(_float)(-0.5f * m_iNumVerticesZ + i + 0.5f)
			);

			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f) * 20.f, i / (m_iNumVerticesZ - 1.f) * 20.f);
		}
	}

	memcpy(m_pVertices, pVertices, sizeof(VTXTEX) * m_iNumVertices);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_eFormat = D3DFMT_INDEX16;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	_uint		iNumFace = 0;

	if (nullptr != m_pIndices)
		Safe_Delete_Array(m_pIndices);

	m_pIndices = new FACELISTINDICES16[m_iNumPrimitives];

	FACELISTINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1, 
				iIndex
			};

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];
			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];
			++iNumFace;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitives);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pCubeMapPath)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pCubeMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	BITMAPINFOHEADER			ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	
	_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ZeroMemory(pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight);

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);
	
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXTEX[m_iNumVertices];

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0/*m_iStride * m_iNumVertices*/, (void**)&pVertices, 0);

	/* 값을 바꾸고. */

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.f/*(pPixel[iIndex] & 0x000000ff) / 15.0f*/, (_float)i);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f) * 20.f, i / (m_iNumVerticesZ - 1.f) * 20.f);
		}
	}

	memcpy(m_pVertices, pVertices, sizeof(VTXTEX) * m_iNumVertices);

	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_eFormat = D3DFMT_INDEX16;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	_uint		iNumFace = 0;

	m_pIndices = new FACELISTINDICES16[m_iNumPrimitives];

	FACELISTINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[1];
			pIndices[iNumFace]._2 = iIndices[2];
			++iNumFace;

			pIndices[iNumFace]._0 = iIndices[0];
			pIndices[iNumFace]._1 = iIndices[2];
			pIndices[iNumFace]._2 = iIndices[3];
			++iNumFace;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitives);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
		return S_OK;

	POINT pt{};
	memcpy(&pt, pArg, sizeof(POINT));

	if (FAILED(NativeConstruct_Prototype(pt.x, pt.y)))
		return E_FAIL;

	return S_OK;
}

_float3 CVIBuffer_Terrain::Compute_Height(CTransform* pTerrainWorldMatrix, const _float3 & vPosition)
{
	if (vPosition.x < 0 || vPosition.z < 0)
		return _float3(vPosition.x, 1.f, vPosition.z);

	_float4x4		WorldMatrix = pTerrainWorldMatrix->Get_WorldMatrix();
	_float4x4		WorldMatrixInv = *D3DXMatrixInverse(&WorldMatrixInv, nullptr, &WorldMatrix);

	_float3			vLocalPos = *D3DXVec3TransformCoord(&vLocalPos, &vPosition, &WorldMatrixInv);

	_uint			iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	_uint			iIndices[] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1, 
		iIndex
	};

	VTXTEX*		pVertices = (VTXTEX*)m_pVertices;
	
	D3DXPLANE		Plane;

	/* 오.위*/
	if ((vLocalPos.x - pVertices[iIndices[0]].vPosition.x) > 
		(pVertices[iIndices[0]].vPosition.z - vLocalPos.z))
	{
		D3DXPlaneFromPoints(&Plane, &pVertices[iIndices[0]].vPosition,
			&pVertices[iIndices[1]].vPosition,
			&pVertices[iIndices[2]].vPosition);
	}
	/* 왼.아*/
	else
	{
		D3DXPlaneFromPoints(&Plane, &pVertices[iIndices[0]].vPosition,
			&pVertices[iIndices[2]].vPosition,
			&pVertices[iIndices[3]].vPosition);
	}

	/*(-ax - cz - d) / b  */
	vLocalPos.y = (-Plane.a * vLocalPos.x - Plane.c * vLocalPos.z - Plane.d) / Plane.b;	

	_float3		vWorldPos = *D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, &WorldMatrix);

	return vWorldPos;
}


CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pCubeMapPath)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pCubeMapPath)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
