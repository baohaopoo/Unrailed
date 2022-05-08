#include "stdafx.h"
#include "..\Public\Level_Loading.h"


#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "Loader.h"
#include "GameOver.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	

	D3DXCreateFont(m_pGraphic_Device, 30, 10, 920, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"오이체", &font);
	
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadingUI"))))
		return E_FAIL;



	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	++cnt;
	if (cnt > 70) cnt = 0;



	
	Safe_AddRef(pGameInstance);


	if (true == m_pLoader->Get_Finished())
	{

		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			CLevel*		pLevel = nullptr;



			switch (m_eNextLevel)
			{
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEOVER:
				pLevel = GameOver::Create(m_pGraphic_Device);
				break;
			}
			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
				goto Finish;
		}
	}

Finish:
	Safe_Release(pGameInstance);
	/*Safe_AddRef(pGameInstance);

	if (true == m_pLoader->Get_Finished())
	{
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			CLevel*		pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;
			}
			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel))) {
				Safe_Release(pGameInstance);
				return;
			}
		}
	}

	Safe_Release(pGameInstance);*/
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	_tchar		szText[MAX_PATH] = TEXT("");

	lstrcpy(szText, TEXT("로딩 레벨입니다 : "));
	lstrcat(szText, m_pLoader->Get_LoadingText());
	char string1[100] = "지도 생성중";






	//char string1[100] = "지도생성중";
	//RECT rt1 = { 590,300,630,400 };
	//font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


	if (cnt >70)
	{

		cnt *= -1;
	}

	RECT rt1;


	if (cnt<30)
		rt1 = { 600 + cnt,300,-1,-1 };
	else if (cnt > 30) {
		rt1 = { 600 - cnt,300,-1,-1 };

	}



	font->DrawTextA(NULL, string1, -1, &rt1, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



	SetWindowText(g_hWnd, szText);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);


}
