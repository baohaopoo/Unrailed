#include "..\Public\VIBuffer.h"



CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iStride(rhs.m_iStride)
	, m_dwFVF(rhs.m_dwFVF)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitives(rhs.m_iNumPrimitives)
	, m_eFormat(rhs.m_eFormat)
	, m_iPrimitiveIndicesSize(rhs.m_iPrimitiveIndicesSize)
	, m_pVertices(rhs.m_pVertices)
	, m_pIndices(rhs.m_pIndices)
	, m_eVIBuffer(rhs.m_eVIBuffer)
{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitives);
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iPrimitiveIndicesSize * m_iNumPrimitives, 0, m_eFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;

	
}

HRESULT CVIBuffer::SaveVIBuffer(const string& _strFilePath)
{
	// Ư�� ��ο� �ִ� ������ ���� �����Ѵٸ�, �̾ ���̰�, �ƴ϶�� ���� ���� ������ �����ϴ� ������ �Ѵ�.
	// ������ �̷��� �Ǹ� ������ �����ϴ� ���Ͽ� ���̱� ������, ���ϴ� �����Ͱ� �ջ�� �� �ִ�. 
	// ������ �����ʹ� ���ؽ� ���ۿ� �ε��� ���� �Ѵ� �����Ѵ�.

	return S_OK;
}

void CVIBuffer::Free()
{
	if (false == m_isCloned) {
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
