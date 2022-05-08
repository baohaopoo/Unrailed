
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

	// 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 맨 마지막 경로를 잘라내는 함수
	// 경로 상에 맨 마지막 경로가 파일명이면 그 파일명을 제외한 경로만 남고, 만약 파일명이 없으면 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	// data폴더 경로를 이어붙임
	lstrcat(szPath, L"\\Data\\ToolObject\\");

	// 인자로 받은 파일의 이름을 붙임
	lstrcat(szPath, (TCHAR*)pArg);

	// 파일 열기
	HANDLE		hFile = CreateFile(szPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD		dwByte = 0;

	while (true)
	{
		// 오브젝트 갯수를 읽는다.
		SIZE_T iSize = 0;
		ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		// 읽어온 오브젝트 갯수만큼 반복문 실행
		for (int i = 0; i < iSize; ++i) {

			// 오브젝트의 타입을 가져온다.
			SIZE_T iType = 0;
			ReadFile(hFile, &iType, sizeof(SIZE_T), &dwByte, nullptr);

			CTransform::TRANSFORMDESC TransDesc;
			ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, nullptr);

			// 타입에 따라 다른 VIBuffer 컴포넌트를 사용하여 VIBuffer를 만든다.
			switch (VIBuffer(iType))
			{
			case Engine::VIB_RECT:
				// 일단은 큐브만 다른건 차후 추가
				break;
			case Engine::VIB_CUBE:
				// 여기선 버퍼 리스트에 객체를 추가하는것이 아닌 
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