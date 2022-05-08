#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_MapCube(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CTerrain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;	

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.fScalePerSec = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom, nullptr)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_MapCube(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 맨 마지막 경로를 잘라내는 함수
	// 경로 상에 맨 마지막 경로가 파일명이면 그 파일명을 제외한 경로만 남고, 만약 파일명이 없으면 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	// data폴더 경로를 이어붙임
	lstrcat(szPath, L"\\Data\\Map\\");

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
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, Layer_Terrain, Prototype_Object_CubeObject, &TransDesc);

				// 여기선 버퍼 리스트에 객체를 추가하는것이 아닌 
				/*m_lstVIBuffer.push_back(
					make_pair(
						CVIBuffer_Cube::Create(m_pGraphic_Device, TransDesc.m_LocalMatrix),
						TransDesc.m_iTextureNumber
					)
				);*/
				break;
			default:
				break;
			}
		}
	}
	CloseHandle(hFile);

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
