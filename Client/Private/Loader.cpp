#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Player.h"
#include "Player_Feet.h"
#include "Player_LeftFeet.h"
#include "Terrain.h"
#include "Camera_Default.h"
#include "Tree.h"
#include "MenuBtn.h"
#include "Effect_Explo.h"
#include "Effect_Rain.h"
#include "Camel.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	case LEVEL_GAMEPLAY2:
		pLoader->Loading_ForGamePlayLevel();
		break;

	case LEVELGAMEPLAY3:
		pLoader->Loading_ForGamePlayLevel();
		break;

	}


	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));



	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));



	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));	



	lstrcpy(m_szLoading, TEXT("이펙트를 로드중입니다. "));




	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));





	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForBossLevel()
{
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	
	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGraphic_Device);
}
