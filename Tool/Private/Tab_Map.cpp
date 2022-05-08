// Tab_Map.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab_Map.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"

#include "Terrain.h"
#include "VIBuffer.h"
#include "Camera_Default.h"

#include "Level_Map.h"
#include "Terrain.h"

#include "Rock.h"
#include "Tree.h"

// CTab_Map 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab_Map, CDialogEx)

CTab_Map::CTab_Map(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TAB_MAP, pParent)
	, m_iTerrainSizeX(0)
	, m_iTerrainSizeZ(0)
	, m_nModeRadio(0)
	, m_iTextureNumber(0)
{

}

CTab_Map::~CTab_Map()
{
}

void CTab_Map::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTerrainSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_iTerrainSizeZ);
	DDX_Control(pDX, IDC_EDIT1, m_EditMapSizeX);
	DDX_Control(pDX, IDC_EDIT2, m_EditMapSizeZ);
	DDX_Control(pDX, IDC_EDIT11, m_EditScaleX);
	DDX_Control(pDX, IDC_EDIT12, m_EditScaleY);
	DDX_Control(pDX, IDC_EDIT10, m_EditScaleZ);
	DDX_Control(pDX, IDC_EDIT4, m_EditRotateX);
	DDX_Control(pDX, IDC_EDIT6, m_EditRotateY);
	DDX_Control(pDX, IDC_EDIT5, m_EditRotateZ);
	DDX_Control(pDX, IDC_EDIT7, m_EditTransformX);
	DDX_Control(pDX, IDC_EDIT9, m_EditTransformY);
	DDX_Control(pDX, IDC_EDIT8, m_EditTransformZ);
	DDX_Control(pDX, IDC_BUTTON3, m_BtApplyScale);
	DDX_Control(pDX, IDC_BUTTON8, m_BtApplyRotation);
	DDX_Control(pDX, IDC_BUTTON9, m_BtApplyTransform);

	DDX_Radio(pDX, IDC_RADIO8, (int&)m_nModeRadio);
	DDX_Control(pDX, IDC_LIST2, m_ListBoxMapObject);
	DDX_Control(pDX, IDC_EDIT3, m_ObjectScaleX);
	DDX_Control(pDX, IDC_EDIT13, m_ObjectScaleY);
	DDX_Control(pDX, IDC_EDIT14, m_ObjectScaleZ);
	DDX_Control(pDX, IDC_EDIT16, m_ObjectRotationY);
	DDX_Text(pDX, IDC_EDIT21, m_iTextureNumber);
	DDX_Control(pDX, IDC_EDIT21, m_EditTextureNumber);
}


BEGIN_MESSAGE_MAP(CTab_Map, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO8, IDC_RADIO10, RadioCtrl)

	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Map::ChangeTerrainSize)
	ON_BN_CLICKED(IDC_BUTTON5, &CTab_Map::SaveMapData)
	ON_BN_CLICKED(IDC_BUTTON6, &CTab_Map::LoadMapData)
	ON_BN_CLICKED(IDC_BUTTON3, &CTab_Map::ApplySelectObjectScale)
	ON_BN_CLICKED(IDC_BUTTON8, &CTab_Map::ApplySelectObjectRotation)
	ON_BN_CLICKED(IDC_BUTTON9, &CTab_Map::ApplySelectObjectTransform)
	ON_BN_CLICKED(IDC_BUTTON7, &CTab_Map::ApplyObjectTransformation)
	ON_BN_CLICKED(IDC_BUTTON10, &CTab_Map::ApplySelectObjectTextureNumber)
END_MESSAGE_MAP()


// CTab_Map 메시지 처리기입니다.
void CTab_Map::ChangeTerrainSize()
{
	CString str;
	m_EditMapSizeX.GetWindowText(str);
	m_iTerrainSizeX = _ttoi(str);
	m_EditMapSizeZ.GetWindowText(str);
	m_iTerrainSizeZ = _ttoi(str);

	CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView* m_pToolView = dynamic_cast<CToolView*>(m_pMain->m_MainSplitter.GetPane(0, 1));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Delete_Layer(LEVEL_MAP, Layer_Terrain);
	pGameInstance->Delete_Layer(LEVEL_MAP, Layer_Map);
	CTerrain::MAPDESC MapDesc;
	MapDesc.iMapSizeX = m_iTerrainSizeX;
	MapDesc.iMapSizeY = m_iTerrainSizeZ;

	pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Temp"), Prototype_Object_Terrain, &MapDesc);
}



void CTab_Map::SaveMapData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog	Dlg(FALSE,	// true인 경우 열기 모드, false 인 경우 다른 이름으로 저장 모드
		L"dat", // default 파일 확장명, 저장 시 확장자명을 명시해주지 않으면 현재 기입한 확장자명을 사용하게 설정됨
		L"*.dat", // 대화상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고메시지 띄우기 옵션)
		L"Data File(*.dat)|*.dat||",	// 대화 상자에 표시될 파일 형식, 콤보박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||
		this); // 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 맨 마지막 경로를 잘라내는 함수
	// 경로 상에 맨 마지막 경로가 파일명이면 그 파일명을 제외한 경로만 남고, 만약 파일명이 없으면 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);

	// data폴더 경로를 이어붙임
	lstrcat(szPath, L"\\Data\\Map");

	// 대화상자를 열었을 때 보이는 기본 경로 설정, 절대 경로인 상황
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : 대화 상자를 실행, IDOK 실행 성공
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName 선택된 경로를 반환
		CString	strFind = Dlg.GetPathName();
		const TCHAR*	pGetPath = strFind.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		// Layer_Terrain
		// ============================================================================================================
		// 저장할 정보가 담긴 레이어를 찾는다.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MAP, Layer_Terrain);

		// 레이어가 없다면 return E_FAIL;
		if (nullptr == pLayer)
			return;

		// 레이어에 있는 오브젝트 정보들을 가지고 온다.
		list<CGameObject*> ObjectList = pLayer->Get_ObjectList();

		// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
		if (ObjectList.empty())
			return;

		DWORD		dwByte = 0;

		// 오브젝트의 갯수를 넣는다.
		SIZE_T iSize = ObjectList.size();
		WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

		for (auto& Object : ObjectList) {

			// 오브젝트의 VIBuffer 타입과 행렬 정보를 들고온다.
			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(Object->Get_Component(Component_VIBuffer));
			CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

			// 만약 둘중 하나라도 타입정보나 행렬정보가 없다면 넘긴다.
			if (nullptr == pVIBuffer || nullptr == pTransform)
				continue;

			// 타입의 정보를 저장한다.
			SIZE_T iType = (SIZE_T)pVIBuffer->Get_Type();
			WriteFile(hFile, &iType, sizeof(SIZE_T), &dwByte, NULL);

			// 해당 객체의 행렬정보를 모두 저장한다.
			CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
			TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();
			TransDesc.m_iTextureNumber = Object->Get_TextureNumber();
			WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
		}
		// ============================================================================================================










		// Layer Map
		// ============================================================================================================
		pLayer = pGameInstance->Find_Layer(LEVEL_MAP, Layer_Map);

		//// 레이어가 없다면 return E_FAIL;
		if (nullptr == pLayer) {
			iSize = 0;
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);
		}
		else {
			// 레이어에 있는 오브젝트 정보들을 가지고 온다.
			ObjectList = pLayer->Get_ObjectList();

			//// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
			//if (ObjectList.empty())
			//	return;

			dwByte = 0;

			// 오브젝트의 갯수를 넣는다.
			iSize = ObjectList.size();
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			for (auto& Object : ObjectList) {

				// 오브젝트의 월드 행렬 정보를 들고온다.
				CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

				// 만약 행렬정보가 없다면 넘긴다.
				if (nullptr == pTransform)
					continue;

				// 오브젝트 타입의 정보를 저장한다.
				int eType = (int)Object->Get_Type();
				WriteFile(hFile, &eType, sizeof(int), &dwByte, NULL);

				// 해당 객체의 행렬정보를 저장한다.
				CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
				TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();
				WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
			}
		}
		// ============================================================================================================







		// Layer Unbreakable
		// ============================================================================================================
		pLayer = pGameInstance->Find_Layer(LEVEL_MAP, TEXT("Layer_Unbreakable"));

		// 레이어가 없다면 return E_FAIL;
		if (nullptr == pLayer) {
			iSize = 0;
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);
		}
		else {

			// 레이어에 있는 오브젝트 정보들을 가지고 온다.
			ObjectList = pLayer->Get_ObjectList();

			// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
			if (ObjectList.empty())
				return;

			dwByte = 0;

			// 오브젝트의 갯수를 넣는다.
			iSize = ObjectList.size();
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			for (auto& Object : ObjectList) {

				// 오브젝트의 월드 행렬 정보를 들고온다.
				CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

				// 만약 행렬정보가 없다면 넘긴다.
				if (nullptr == pTransform)
					continue;

				// 오브젝트 타입의 정보를 저장한다.
				int eType = (int)Object->Get_Type();
				WriteFile(hFile, &eType, sizeof(int), &dwByte, NULL);

				if (eType == OBJECT_RAIL_CURVE || eType == OBJECT_RAIL_STRAIGHT) {
					int a = 10;
				}


				// 해당 객체의 행렬정보를 저장한다.
				CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
				TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();
				WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
			}
		}
		// ============================================================================================================
		CloseHandle(hFile);
	}
}


void CTab_Map::LoadMapData()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Delete_Layer(LEVEL_MAP, Layer_Terrain);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog	Dlg(TRUE,	// true인 경우 열기 모드, false 인 경우 다른 이름으로 저장 모드
		L"dat", // default 파일 확장명, 저장 시 확장자명을 명시해주지 않으면 현재 기입한 확장자명을 사용하게 설정됨
		L"*.dat", // 대화상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고메시지 띄우기 옵션)
		L"Data File(*.dat)|*.dat||",	// 대화 상자에 표시될 파일 형식, 콤보박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||
		this); // 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트가 있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 맨 마지막 경로를 잘라내는 함수
	// 경로 상에 맨 마지막 경로가 파일명이면 그 파일명을 제외한 경로만 남고, 만약 파일명이 없으면 마지막 폴더명을 잘라낸다.
	PathRemoveFileSpec(szPath);

	// data폴더 경로를 이어붙임
	lstrcat(szPath, L"\\Data\\Map");

	// 대화상자를 열었을 때 보이는 기본 경로 설정, 절대 경로인 상황
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : 대화 상자를 실행, IDOK 실행 성공
	if (IDOK == Dlg.DoModal())
	{

		// 파일 열기
		CString	strFind = Dlg.GetPathName();
		const TCHAR*	pGetPath = strFind.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte = 0;
		DWORD		dwStrByte = 0;

		while (true) {

			// Layer_Terrain
			// ============================================================================================================
			// 오브젝트 갯수를 읽는다.
			SIZE_T iSize = 0;
			ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CToolView* m_pToolView = dynamic_cast<CToolView*>(m_pMain->m_MainSplitter.GetPane(0, 1));

			m_pGraphic_Device = m_pToolView->Get_Device();

			// 읽어온 오브젝트 갯수만큼 반복문 실행
			for (int i = 0; i < iSize; ++i) {

				// 오브젝트의 타입을 가져온다.
				SIZE_T iType = 0;
				ReadFile(hFile, &iType, sizeof(SIZE_T), &dwByte, nullptr);

				// 해당 객체의 행렬정보를 모두 저장한다.
				CTransform::TRANSFORMDESC TransDesc;
				ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);

				// 타입에 따라 다른 VIBuffer 컴포넌트를 사용하여 VIBuffer를 만든다.
				switch (VIBuffer(iType))
				{
				case Engine::VIB_RECT:
					// 일단은 큐브만 다른건 차후 추가
					break;
				case Engine::VIB_CUBE:
					// 여기선 버퍼 리스트에 객체를 추가하는것이 아닌 
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, Prototype_Object_CubeObject, &TransDesc);
					break;
				default:
					break;
				}
			}
			// ============================================================================================================




			



			// Layer Map
			// ============================================================================================================
			ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			// 읽어온 오브젝트 갯수만큼 반복문 실행
			for (int i = 0; i < iSize; ++i) {

				// 오브젝트의 타입을 가져온다.
				int iType = 0;
				ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

				// 해당 객체의 행렬정보를 모두 저장한다.
				CTransform::TRANSFORMDESC TransDesc;
				ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
				TransDesc.m_WorldMatrix._11 *= 30.f;
				TransDesc.m_WorldMatrix._12 *= 30.f;
				TransDesc.m_WorldMatrix._13 *= 30.f;
				TransDesc.m_WorldMatrix._21 *= 30.f;
				TransDesc.m_WorldMatrix._22 *= 30.f;
				TransDesc.m_WorldMatrix._23 *= 30.f;
				TransDesc.m_WorldMatrix._31 *= 30.f;
				TransDesc.m_WorldMatrix._32 *= 30.f;
				TransDesc.m_WorldMatrix._33 *= 30.f;

				CRock::ROCKDESC RockDesc;
				RockDesc.TransDesc = TransDesc;

				CTree::TreeDESC TreeDesc;
				TreeDesc.TransDesc = TransDesc;

				// 타입에 따라 다른 class를 사용하여 오브젝트를 만든다.
				switch ((OBJECT_TYPE)iType)
				{
					// 자원 - 돌
					// ========================================================================================================================
				case Engine::OBJECT_ROCK1:
					RockDesc.iRockNumber = 1;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
					break;
				case Engine::OBJECT_ROCK2:
					RockDesc.iRockNumber = 2;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
					break;
				case Engine::OBJECT_ROCK3:
					RockDesc.iRockNumber = 3;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rock"), &RockDesc);
					break;
					// ========================================================================================================================



					// 자원 - 나무
					// ========================================================================================================================
				case Engine::OBJECT_TREE_DARK1:
					TreeDesc.iTreeNumber = 1;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
					break;
				case Engine::OBJECT_TREE_DARK2:
					TreeDesc.iTreeNumber = 2;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
					break;
				case Engine::OBJECT_TREE_DARK3:
					TreeDesc.iTreeNumber = 3;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
					break;
				case Engine::OBJECT_TREE_DARK4:
					TreeDesc.iTreeNumber = 4;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
					break;
				case Engine::OBJECT_TREE_LIGHT:
					TreeDesc.iTreeNumber = 5;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Tree"), &TreeDesc);
					break;
					// ========================================================================================================================




					// 자원 - 철 & 목재
					// ========================================================================================================================
				case Engine::OBJECT_MATERIAL_IRON:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Iron"), &TransDesc);
					break;
				case Engine::OBJECT_MATERIAL_WOOD:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Wood"), &TransDesc);
					break;
					// ========================================================================================================================




					// 건물 - 기차역
					// ========================================================================================================================
				case Engine::OBJECT_PLATFORM_START:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_TrainStation"), &TransDesc);
					break;
				case Engine::OBJECT_PLATFORM_FINISH:
					break;
					// ========================================================================================================================



					// 오브젝트 - 기차
					// ========================================================================================================================
				case Engine::OBJECT_TRAIN_HEAD:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Head"), &TransDesc);
					break;
				case Engine::OBJECT_TRAIN_MATERIAL:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Material"), &TransDesc);
					break;
				case Engine::OBJECT_TRAIN_RAIL:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Rail"), &TransDesc);
					break;
				case Engine::OBJECT_TRAIN_WATER:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Water"), &TransDesc);
					break;
				case Engine::OBJECT_TRAIN_DYNAMITE:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Train_Dynamite"), &TransDesc);
					break;
					// ========================================================================================================================



					// 오브젝트 - 도구
					// ========================================================================================================================
				case Engine::OBJECT_PICKAX:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_PickAx"), &TransDesc);
					break;
				case Engine::OBJECT_AXE:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Axe"), &TransDesc);
					break;
				case Engine::OBJECT_BUCKET:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Bucket"), &TransDesc);
					break;
					// ========================================================================================================================



					// 오브젝트 - 중립
					// ========================================================================================================================
				case Engine::OBJECT_CAMEL:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Camel"), &TransDesc);
					break;
					// ========================================================================================================================



					// 오브젝트 - 레일
					// ========================================================================================================================
				case Engine::OBJECT_RAIL_STRAIGHT:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rail_Straight"), &TransDesc);
					break;
				case Engine::OBJECT_RAIL_CURVE:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rail_Curve"), &TransDesc);
					break;
				case Engine::OBJECT_RAIL_END:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Rail_End"), &TransDesc);
					break;
					// ========================================================================================================================



					// 맵 터레인 오브젝트 - 맵 타일 큐브
					// ========================================================================================================================
				case Engine::OBJECT_MAPCUBE:
					TransDesc.m_WorldMatrix._11 /= 30.f;
					TransDesc.m_WorldMatrix._12 /= 30.f;
					TransDesc.m_WorldMatrix._13 /= 30.f;
					TransDesc.m_WorldMatrix._21 /= 30.f;
					TransDesc.m_WorldMatrix._22 /= 30.f;
					TransDesc.m_WorldMatrix._23 /= 30.f;
					TransDesc.m_WorldMatrix._31 /= 30.f;
					TransDesc.m_WorldMatrix._32 /= 30.f;
					TransDesc.m_WorldMatrix._33 /= 30.f;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Terrain, TEXT("Prototype_GameObject_MapCube"), &TransDesc);
					break;
					// ========================================================================================================================


				case Engine::OBJECT_END:
					break;
				default:
					break;
				}
			}
			// ============================================================================================================




			// Layer Unbreakable
			// ============================================================================================================
			ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			// 읽어온 오브젝트 갯수만큼 반복문 실행
			for (int i = 0; i < iSize; ++i) {

				// 오브젝트의 타입을 가져온다.
				int iType = 0;
				ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

				// 해당 객체의 행렬정보를 모두 저장한다.
				CTransform::TRANSFORMDESC TransDesc;
				ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
				TransDesc.m_WorldMatrix._11 *= 30.f;
				TransDesc.m_WorldMatrix._12 *= 30.f;
				TransDesc.m_WorldMatrix._13 *= 30.f;
				TransDesc.m_WorldMatrix._21 *= 30.f;
				TransDesc.m_WorldMatrix._22 *= 30.f;
				TransDesc.m_WorldMatrix._23 *= 30.f;
				TransDesc.m_WorldMatrix._31 *= 30.f;
				TransDesc.m_WorldMatrix._32 *= 30.f;
				TransDesc.m_WorldMatrix._33 *= 30.f;

				CRock::ROCKDESC RockDesc;
				RockDesc.TransDesc = TransDesc;

				CTree::TreeDESC TreeDesc;
				TreeDesc.TransDesc = TransDesc;

				// 타입에 따라 다른 class를 사용하여 오브젝트를 만든다.
				switch ((OBJECT_TYPE)iType)
				{
					// 맵 터레인 오브젝트 - 부서지지 않는 돌
					// ========================================================================================================================
				case Engine::OBJECT_UNBREAKABLE_ROCK1:
					RockDesc.iRockNumber = 1;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
					break;
				case Engine::OBJECT_UNBREAKABLE_ROCK2:
					RockDesc.iRockNumber = 2;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
					break;
				case Engine::OBJECT_UNBREAKABLE_ROCK3:
					RockDesc.iRockNumber = 3;
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, TEXT("Layer_Unbreakable"), TEXT("Prototype_GameObject_Unbreakable_Rock"), &RockDesc);
					break;
					// ========================================================================================================================
				}
			}
			// ============================================================================================================


		}
		CloseHandle(hFile);
	}
}

void CTab_Map::RadioCtrl(UINT id)
{
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Find_Object(LEVEL_MAP, Layer_Camera_Map, string("camera"));
	CCamera_Default* pCamera = dynamic_cast<CCamera_Default*>(pObject);

	if (nullptr == pCamera)
		return;

	CLevel_Map* plevel = dynamic_cast<CLevel_Map*>(pGameInstance->GetCurrLevel());

	if (id == IDC_RADIO8)
		m_nModeRadio = 0;
	if (id == IDC_RADIO9)
		m_nModeRadio = 1;
	if (id == IDC_RADIO10)
		m_nModeRadio = 2;

	// 모드 선택과 관련된 switch문
	switch (m_nModeRadio) {
	case 0:
		// 생성모드
		plevel->changeMode(CLevel_Map::MODE::MODE_CREATE);
		pCamera->SetObserving(false);
		break;

	case 1:
		// 선택모드
		plevel->changeMode(CLevel_Map::MODE::MODE_SELECT);
		pCamera->SetObserving(false);
		break;

	case 2:
		// 움직임 모드
		plevel->changeMode(CLevel_Map::MODE::MODE_CAMERA);
		pCamera->SetObserving(true);
		break;
	}

	UpdateData(FALSE);
}


BOOL CTab_Map::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//CString tpath = _T("..\\Data\\MapObject\\*.*");

	//// 검색 클래스
	//CFileFind finder;

	//// CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	//BOOL bWorking = finder.FindFile(tpath); //

	//CString fileName;
	//CString DirName;

	//// Tile_ListBox 초기화
	//while (bWorking)
	//{
	//	//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
	//	bWorking = finder.FindNextFile();

	//	// folder 일 경우는 continue
	//	if (finder.IsDirectory() || finder.IsDots())
	//		continue;

	//	// 파일 일때

	//	//파일의 이름
	//	CString _fileName = finder.GetFileName();

	//	fileName = finder.GetFileTitle();

	//	//읽어온 파일 이름을 리스트박스와 map에 넣음
	//	CString temp = tpath;
	//	TCHAR	szPath[MAX_PATH] = L"";
	//	lstrcpy(szPath, temp.GetString());
	//	PathRemoveFileSpec(szPath);
	//	lstrcat(szPath, L"\\");
	//	lstrcat(szPath, fileName);
	//	lstrcat(szPath, L".dat");

	//	auto& iter = std::find_if(m_vObjectFile.begin(), m_vObjectFile.end(), [&szPath](const TCHAR* strFilePath) -> bool {
	//		return wcscmp(szPath, strFilePath);
	//	});

	//	// 찾지 못했다면
	//	if (iter == m_vObjectFile.end()) {
	//		m_vObjectFile.push_back(szPath);
	//		m_ListBoxMapObject.AddString(fileName);
	//	}
	//}

	// 리소스 - 돌
	AddStringToListBox(string("00. Rock1"));
	AddStringToListBox(string("01. Rock2"));
	AddStringToListBox(string("02. Rock3"));

	// 리소스 - 나무
	AddStringToListBox(string("03. Tree_Dark1"));
	AddStringToListBox(string("04. Tree_Dark2"));
	AddStringToListBox(string("05. Tree_Dark3"));
	AddStringToListBox(string("06. Tree_Dark4"));
	AddStringToListBox(string("07. Tree_Light"));

	// 물질 - 철 & 목재
	AddStringToListBox(string("08. Material_Iron"));
	AddStringToListBox(string("09. Material_Wood"));

	// 오브젝트 - 정거장
	AddStringToListBox(string("10. PlatformStart"));
	AddStringToListBox(string("11. PlatformFinish"));

	// 오브젝트 - 기차
	AddStringToListBox(string("12. Train_Head"));
	AddStringToListBox(string("13. Train_Material"));
	AddStringToListBox(string("14. Train_Rail"));
	AddStringToListBox(string("15. Train_Water"));
	AddStringToListBox(string("16. Train_Dynamite"));

	// 오브젝트 - 장비
	AddStringToListBox(string("17. Pickax"));
	AddStringToListBox(string("18. Axe"));
	AddStringToListBox(string("19. Bucket"));

	// 오브젝트 - 중립
	AddStringToListBox(string("20. Camel"));

	// 오브젝트 - 기차레일
	AddStringToListBox(string("21. Rail_Straight"));
	AddStringToListBox(string("22. Rail_Curve"));

	// 맵 오브젝트 - 부술수 없는 돌
	AddStringToListBox(string("23. Unbreakable Rock1"));
	AddStringToListBox(string("24. Unbreakable Rock2"));
	AddStringToListBox(string("25. Unbreakable Rock3"));

	// 맵 오브젝트 - 맵 큐브
	AddStringToListBox(string("26. MapCube"));

	// 오브젝트 - 기차레일
	AddStringToListBox(string("27. Rail_End"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTab_Map::ApplySelectObjectScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;
	_uint iX, iY, iZ;

	// 크기
	m_EditScaleX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditScaleY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditScaleZ.GetWindowText(str);
	iZ = _ttoi(str);

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// 원본 크기 대비 비율로 확대/축소
	if (nullptr != pTransform) {
		pTransform->Scaled(_float3(iX, iY, iZ));

		_float4x4 worldMatrix = pTransform->Get_WorldMatrix();
		worldMatrix._11 /= 30;
		worldMatrix._12 /= 30;
		worldMatrix._13 /= 30;
						  
		worldMatrix._21 /= 30;
		worldMatrix._22 /= 30;
		worldMatrix._23 /= 30;
						   
		worldMatrix._31 /= 30;
		worldMatrix._32 /= 30;
		worldMatrix._33 /= 30;

		pTransform->Set_WorldMatrix(worldMatrix);
	}
}


void CTab_Map::ApplySelectObjectRotation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;
	_uint iX, iY, iZ;

	// 크기
	m_EditRotateX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditRotateY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditRotateZ.GetWindowText(str);
	iZ = _ttoi(str);

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// 현 위치에서 회전
	if (nullptr != pTransform) {
		// z축 회전
		pTransform->Turned(_float3(0.f, 0.f, 1.f), iZ);
		// y축 회전
		pTransform->Turned(_float3(0.f, 1.f, 0.f), iY);
		// x축 회전
		pTransform->Turned(_float3(1.f, 0.f, 0.f), iX);
		
		//CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
		//TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();

		//pTransform->Set_TransformState(TransDesc);
	}


}


void CTab_Map::ApplySelectObjectTransform()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;
	_uint iX, iY, iZ;

	// 크기
	m_EditTransformX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditTransformY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditTransformZ.GetWindowText(str);
	iZ = _ttoi(str);


	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// 현재 위치에서 추가적인 이동
	if (nullptr != pTransform) {
		_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos += _float3(iX, iY, iZ);
		pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}

}


void CTab_Map::ApplyObjectTransformation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 여기서 적용될 것은 로컬이 아닌 월드변환 행렬이지만, 한번에 전달 할 수 있는 값이 없어서,
	// 해당 구조체를 사용하여 값을 전달한다.

	CString str;
	float fX, fY, fZ;

	m_ObjectScaleX.GetWindowText(str);
	fX = _ttoi(str);
	m_ObjectScaleY.GetWindowText(str);
	fY = _ttoi(str);
	m_ObjectScaleZ.GetWindowText(str);
	fZ = _ttoi(str);

	// 크기
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;

	// 회전
	m_ObjectRotationY.GetWindowText(str);
	m_fRotateY = _ttoi(str);
}

void CTab_Map::AddStringToListBox(string strName)
{
	CString FileName;
	FileName = strName.c_str();

	m_ListBoxMapObject.AddString(FileName);
}


void CTab_Map::ApplySelectObjectTextureNumber()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;
	m_EditTextureNumber.GetWindowText(str);

	m_iTextureNumber = _ttoi(str);
}