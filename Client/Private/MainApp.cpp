#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"



// 레벨
// =========================================
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "Level_GamePlay.h"
#include "GameOver.h"
// =========================================



// 플레이어
// =========================================
#include "Player_Feet.h"
#include "Player_LeftFeet.h"
#include "Player_LeftHand.h"
#include "Player_RightHand.h"
#include "Player.h"
// =========================================



// 오브젝트
// =========================================
// 오브젝트 - Loading
#include "Cube_Object.h"
#include "Water_Cube_Object.h"
#include "Unbreakable_Rock.h"
#include "Terrain.h"

// 오브젝트 - Resource
#include "Tree.h"
#include "Rock.h"

// 오브젝트 - Material
#include "Material_Iron.h"
#include "Material_Wood.h"

// 오브젝트 - Tool
#include "PickAx.h"
#include "Axe.h"
#include "Bucket.h"
#include "Dynamite.h"

// 오브젝트 - Building
#include "TrainStation.h"

// 오브젝트 - Train
#include "Train_Head.h"
#include "Train_Material.h"
#include "Train_Rail.h"
#include "Train_Water.h"
#include "Train_Dynamite.h"

// 오브젝트 - Netural
#include "Camel.h"
#include "Bird.h"
#include "Bird_LeftWing.h"
#include "Bird_RightWing.h"
#include "Giant.h"
#include "Giant_Head.h"
#include "Giant_RightHand.h"
#include "Giant_LeftHand.h"

// 오브젝트 - Rail
#include "Rail.h"
#include "Rail_Invisible.h"
#include "Rail_Ending.h"
#include "Rail_Manager.h"
// =========================================



// 이펙트
// =========================================
#include "Effect_Explo.h"
#include "Effect_Rain.h"
#include "Effect_TrainSmoke.h"
#include "Meteo.h"
#include "Meteo_Giant.h"
// =========================================



// 로고
// =========================================
#include "LogoUI.h"
#include "LoadingUI.h"
#include "LobbyUI.h"
#include "MenuBtn.h"
#include "UIObject.h"
#include "StartBtn.h"
#include "BoltUI.h"
#include "lifeUI.h"
#include "MenuPop.h"
#include "ProgressBar.h"
#include "LobbyTerrain.h"
#include "ExitBtn.h"
#include "ProgressBarExit.h"
#include "TalkBallon.h"
#include "mSign.h"
#include "ToolUI.h"
#include "PlayBackground.h"
#include "BucketProgressBar.h"
// =========================================



// 카메라
// =========================================
#include "Camera_Default.h"
#include "LobbyCamera.h"
// =========================================


// 사운드
#include "SoundMgr.h"


CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(SetUp_DefaultRenderState()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	CSoundMgr::Get_Instance()->Initialize();

	CRail_Manager::GetInstance()->Create(m_pGraphic_Device);

	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();	

	if (FAILED(m_pRenderer->Render_GameObjects()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel*			pLevel = nullptr;

	switch (eLevelIndex)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pGraphic_Device);
		break;
	case LEVEL_GAMEOVER:
		pLevel = GameOver::Create(m_pGraphic_Device);
		break;
	default:
		//pLevel = CLevel_Loading::Create(m_pGraphic_Device, (LEVEL)eLevelIndex);
		pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
		break;
	}

	if (FAILED(m_pGameInstance->Open_Level(eLevelIndex, pLevel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (FAILED(Ready_Prototype_GameObject_Camera())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Camera"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_UI())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_UI"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_InGame"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_Effect())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Effect"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_Player())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Player"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame()
{
	/* InGame */
	if (FAILED(Ready_Prototype_GameObject_InGame_Loading())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Loading"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Resource())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Resource"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Material())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Material"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Tool())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Tool"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Building())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Building"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Train())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Train"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Netural())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Netural"));
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject_InGame_Rail())) {
		MSG_BOX(TEXT("Failed to Ready Prototype_GameObject_Rail"));
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_Player()
{
	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Feet"),
		CPlayer_Feet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_LeftFeet"),
		CPlayer_LeftFeet::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_LeftHand"),
		CPlayer_LeftHand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_RightHand"),
		CPlayer_RightHand::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Loading()
{
	/* For.Prototype_GameObject_CubeObject */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CubeObject"),
		CCube_Object::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CubeObject_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WaterCubeObject"),
		CCube_Water::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_UnbreakableRock */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Unbreakable_Rock"),
		CUnbreakable_Rock::Create(m_pGraphic_Device))))
		return E_FAIL;

	//mSign
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_mSign"), mSign::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Resource()
{
	// 자원 - 돌
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rock"),
		CRock::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 자원 - 나무
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Material()
{
	// 물질 - 철
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Iron"),
		CMaterial_Iron::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 물질 - 목재
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wood"),
		CMaterial_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Tool()
{
	// 도끼
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Axe"),
		CAxe::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 곡괭이
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PickAx"),
		CPickAx::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	// 양동이
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bucket"),
		CBucket::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dynamite"),
		CDynamite::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Building()
{
	// 정거장
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TrainStation"),
		CTrainStation::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Train()
{
	// 기차 머리
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Head"),
		CTrain_Head::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 기차 자원 정재칸
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Material"),
		CTrain_Material::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 기자 레일 정재칸
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Rail"),
		CTrain_Rail::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 기차 물 보관칸
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Water"),
		CTrain_Water::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 기차 다이너마이트 생성 칸
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train_Dynamite"),
		CTrain_Dynamite::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Netural()
{
	// 낙타
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camel"),
		CCamel::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Bird"),
		CBird::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BirdLeft_Wing"),
		CBird_LeftWing::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BirdRight_Wing"),
		CBird_RightWing::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Meteo"),
		CMeteo::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeteoGiant"),
		CMeteo_Giant::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Giant"),
		CGiant::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GiantHead"),
		CGiant_Head::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GiantLeftHand"),
		CGiant_LeftHand::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GiantRightHand"),
		CGiant_RightHand::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_InGame_Rail()
{
	// 레일
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rail"),
		CRail::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 투명한 레일
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rail_Invisible"),
		CRail_Invisible::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 엔딩 레일
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rail_End"),
		CRail_Ending::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_Effect()
{
	/* Effect */
	// =====================================================================================================================================================
	/* For.Prototype_GameObject_Effect_Explo */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Explo"),
		CEffect_Explo::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Rain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Rain"),
		CEffect_Rain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Rain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_TrainSmoke"),
		CEffect_TrainSmoke::Create(m_pGraphic_Device))))
		return E_FAIL;
	// =====================================================================================================================================================

	return S_OK;
}
//민희 오브젝트
HRESULT CMainApp::Ready_Prototype_GameObject_UI()
{
	/* UI */
	// =====================================================================================================================================================
	 /* For.Prototype_GameObject_LogoUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LogoUI"), LogoUI::Create(m_pGraphic_Device))))
		return E_FAIL;
	////로비 카메라
	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LobbyCamera"), LobbyCamera::Create(m_pGraphic_Device))))
	//	return E_FAIL;
	//로비 지형.
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LobbyTerain"), LobbyTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	///* For.Prototype_GameObject_LoadingUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"), LoadingUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_LobbyUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LobbyUI"), LobbyUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_LobbyUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuBtn"), MenuBtn::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_LobbyUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ExitBtn"), ExitBtn::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_UIObject */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIObject"), UIObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BtnObject */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Btn"), StartBtn::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BoltUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BoltUI"), BoltUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_LifeUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LifeUI"), lifeUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MenuPop */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MenuPoP"), MenuPop::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ProgressBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ProgressBar"), ProgressBar::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ProgressBarExit"), ProgressBarExit::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Talkballon
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TalkBallon"), TalkBallon ::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tool UI
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ToolUI"), ToolUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Tool UI
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayGround"), PlayBackground::Create(m_pGraphic_Device))))
		return E_FAIL;

	//waterpreogressBar

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WaterProgressBar"), BucketProgressBar::Create(m_pGraphic_Device))))
		return E_FAIL;

	// =====================================================================================================================================================

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject_Camera()
{
	/* For.Prototype_GameObject_Camera_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Renderer */
	// =====================================================================================================================================================
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);
	// =====================================================================================================================================================




	/* For.Prototype_Component_Transform */
	// =====================================================================================================================================================
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	// =====================================================================================================================================================





	/* For.Prototype_Component_Collider */
	// =====================================================================================================================================================
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;
	// =====================================================================================================================================================






	/* For. Prototype_Component_VIBuffer */
	// =====================================================================================================================================================
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Object */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Object"), CVIBuffer_Object::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 20, 20))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_UI"), VIBufferUI::Create(m_pGraphic_Device))))
		return E_FAIL;
	// =====================================================================================================================================================




	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture()
{
	// 민희 Texture
	if (FAILED(Ready_Prototype_Texture_MinHee())) {
		MSG_BOX(TEXT("Failed to Load Texture MinHee"));
		return E_FAIL;
	}

	// 성욱 Texture
	if (FAILED(Ready_Prototype_Texture_SeongUk())) {
		MSG_BOX(TEXT("Failed to Load Texture SeongUk"));
		return E_FAIL;
	}

	// 주현 Texture
	if (FAILED(Ready_Prototype_Texture_JuHyun())) {
		MSG_BOX(TEXT("Failed to Load Texture JuHyun"));
		return E_FAIL;
	}

	// 현우 Texture
	if (FAILED(Ready_Prototype_Texture_HyunWoo())) {
		MSG_BOX(TEXT("Failed to Load Texture HyunWoo"));
		return E_FAIL;
	}

	// 기타 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Temp/Terrain/Grass_%d.tga"), 2))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_SeongUk()
{
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_MinHee()
{
	// Texture
	// ======================================================================================================================================
	// 타일 6장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MinHee"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/MinHee/Tile%d.dds"), 6))))
		return E_FAIL;
	// ======================================================================================================================================


	// UI
	// ======================================================================================================================================
	// 로고 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LogoUI"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/Logo%d.jpg"), 1))))
		return E_FAIL;

	// 로딩 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_emptyLoading"), 
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/emptyLoading%d.jpg"), 1))))
		return E_FAIL;

	// 로비 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LobbyUI"), 
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/Lobby%d.png"), 1))))
		return E_FAIL;

	// 로비 버튼 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UIBtn"), 
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/Btn%d.jpg"), 1))))
		return E_FAIL;

	// 프로그레스바 버튼 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ProgressBar"), 
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/ProgressBar%d.jpg"), 1))))
		return E_FAIL;


	// 프로그레스바 버튼 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterProgressBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/waterProgress.png"), 1))))
		return E_FAIL;


	// 메뉴팝업 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MenuPOP"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/Mission%d.jpg"), 1))))
		return E_FAIL;

	//UIObject 가라 이미지 1장
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/Temp/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	////UIObject 머리통위에 있는 talk ballon용 이미지 1장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GorH"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/GOHH.png"), 1))))
		return E_FAIL;

	////Play Level에 있는 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_mSign"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/mSign.png"), 1))))
		return E_FAIL;

	////Play Level에 있는 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_btn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/MinHee/UI/PlayLogo%d.png"), 2))))
		return E_FAIL;


	// ======================================================================================================================================

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_JuHyun()
{
	// Texture
	// ======================================================================================================================================
	// 타일 12장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_JuHyun"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/JuHyun/Tile%d.dds"), 12))))
		return E_FAIL;
	// ======================================================================================================================================

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture_HyunWoo()
{

	// Texture
	// ======================================================================================================================================
	// 타일 12장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/HyunWoo/Tile%d.dds"), 60))))
		return E_FAIL;

	// 이펙트 2장
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/HyunWoo/Effect/Texture%d.dds"), 2))))
		return E_FAIL;
	// ======================================================================================================================================

	return S_OK;
}

HRESULT CMainApp::SetUp_DefaultRenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 현재 내 게임에 필요한 기본 렌더링 상태를 설정한다. */
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	
	/* 텍스쳐로부터 값을 가져오는 속성을 셋팅하낟.  */
	/*m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);*/

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

CMainApp * Client::CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);	

	CGameInstance::Release_Engine();
}

