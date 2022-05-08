
#include "..\Public\VIBuffer_Object.h"
#include "GameInstance.h"

CVIBuffer_Object::CVIBuffer_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
	m_eVIBuffer = VIBuffer::VIB_OBJECT;
}

CVIBuffer_Object::CVIBuffer_Object(const CVIBuffer_Object & rhs)
	: CVIBuffer(rhs)
{
	m_eVIBuffer = VIBuffer::VIB_OBJECT;
}

HRESULT CVIBuffer_Object::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Object::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ�� �ִ� ���丮 ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);

	// ��ü ��ο��� �� ������ ��θ� �߶󳻴� �Լ�
	// ��� �� �� ������ ��ΰ� ���ϸ��̸� �� ���ϸ��� ������ ��θ� ����, ���� ���ϸ��� ������ ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	// data���� ��θ� �̾����
	lstrcat(szPath, L"\\Data\\ToolObject\\");

	// ���ڷ� ���� ������ �̸��� ����
	lstrcat(szPath, (TCHAR*)pArg);

	// ���� ����
	HANDLE		hFile = CreateFile(szPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;

	while (true)
	{
		// ������Ʈ ������ �д´�.
		SIZE_T iSize = 0;
		ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		// �о�� ������Ʈ ������ŭ �ݺ��� ����
		for (int i = 0; i < iSize; ++i) {

			// ������Ʈ�� Ÿ���� �����´�.
			SIZE_T iType = 0;
			ReadFile(hFile, &iType, sizeof(SIZE_T), &dwByte, nullptr);

			CTransform::TRANSFORMDESC TransDesc;
			ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, nullptr);

			// Ÿ�Կ� ���� �ٸ� VIBuffer ������Ʈ�� ����Ͽ� VIBuffer�� �����.
			switch (VIBuffer(iType))
			{
			case Engine::VIB_RECT:
				// �ϴ��� ť�길 �ٸ��� ���� �߰�
				break;
			case Engine::VIB_CUBE:
				// ���⼱ ���� ����Ʈ�� ��ü�� �߰��ϴ°��� �ƴ� 
				m_lstVIBuffer.push_back(
					make_pair(
						CVIBuffer_Cube::Create(m_pGraphic_Device, TransDesc.m_LocalMatrix),
						TransDesc.m_iTextureNumber
					)
				);
				break;
			default:
				break;
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_Object::Render()
{
	for (auto& VIBufferObject : m_lstVIBuffer) {
		VIBufferObject.first->Render();
	}

	return S_OK;
}

CVIBuffer_Object * CVIBuffer_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device){

	CVIBuffer_Object*	pInstance = new CVIBuffer_Object(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Object::Clone(void * pArg)
{
	CVIBuffer_Object*	pInstance = new CVIBuffer_Object(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Object::Free()
{
	__super::Free();
}