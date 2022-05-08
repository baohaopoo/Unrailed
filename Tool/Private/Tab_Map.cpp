// Tab_Map.cpp : ���� �����Դϴ�.
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

// CTab_Map ��ȭ �����Դϴ�.

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


// CTab_Map �޽��� ó�����Դϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog	Dlg(FALSE,	// true�� ��� ���� ���, false �� ��� �ٸ� �̸����� ���� ���
		L"dat", // default ���� Ȯ���, ���� �� Ȯ���ڸ��� ��������� ������ ���� ������ Ȯ���ڸ��� ����ϰ� ������
		L"*.dat", // ��ȭ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ� ���� ���� �� ���޽��� ���� �ɼ�)
		L"Data File(*.dat)|*.dat||",	// ��ȭ ���ڿ� ǥ�õ� ���� ����, �޺��ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||
		this); // �θ� ������ �ּ�

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ�� �ִ� ���丮 ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);

	// ��ü ��ο��� �� ������ ��θ� �߶󳻴� �Լ�
	// ��� �� �� ������ ��ΰ� ���ϸ��̸� �� ���ϸ��� ������ ��θ� ����, ���� ���ϸ��� ������ ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);

	// data���� ��θ� �̾����
	lstrcat(szPath, L"\\Data\\Map");

	// ��ȭ���ڸ� ������ �� ���̴� �⺻ ��� ����, ���� ����� ��Ȳ
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : ��ȭ ���ڸ� ����, IDOK ���� ����
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName ���õ� ��θ� ��ȯ
		CString	strFind = Dlg.GetPathName();
		const TCHAR*	pGetPath = strFind.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();

		// Layer_Terrain
		// ============================================================================================================
		// ������ ������ ��� ���̾ ã�´�.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_MAP, Layer_Terrain);

		// ���̾ ���ٸ� return E_FAIL;
		if (nullptr == pLayer)
			return;

		// ���̾ �ִ� ������Ʈ �������� ������ �´�.
		list<CGameObject*> ObjectList = pLayer->Get_ObjectList();

		// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
		if (ObjectList.empty())
			return;

		DWORD		dwByte = 0;

		// ������Ʈ�� ������ �ִ´�.
		SIZE_T iSize = ObjectList.size();
		WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

		for (auto& Object : ObjectList) {

			// ������Ʈ�� VIBuffer Ÿ�԰� ��� ������ ���´�.
			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(Object->Get_Component(Component_VIBuffer));
			CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

			// ���� ���� �ϳ��� Ÿ�������� ��������� ���ٸ� �ѱ��.
			if (nullptr == pVIBuffer || nullptr == pTransform)
				continue;

			// Ÿ���� ������ �����Ѵ�.
			SIZE_T iType = (SIZE_T)pVIBuffer->Get_Type();
			WriteFile(hFile, &iType, sizeof(SIZE_T), &dwByte, NULL);

			// �ش� ��ü�� ��������� ��� �����Ѵ�.
			CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
			TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();
			TransDesc.m_iTextureNumber = Object->Get_TextureNumber();
			WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
		}
		// ============================================================================================================










		// Layer Map
		// ============================================================================================================
		pLayer = pGameInstance->Find_Layer(LEVEL_MAP, Layer_Map);

		//// ���̾ ���ٸ� return E_FAIL;
		if (nullptr == pLayer) {
			iSize = 0;
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);
		}
		else {
			// ���̾ �ִ� ������Ʈ �������� ������ �´�.
			ObjectList = pLayer->Get_ObjectList();

			//// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
			//if (ObjectList.empty())
			//	return;

			dwByte = 0;

			// ������Ʈ�� ������ �ִ´�.
			iSize = ObjectList.size();
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			for (auto& Object : ObjectList) {

				// ������Ʈ�� ���� ��� ������ ���´�.
				CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

				// ���� ��������� ���ٸ� �ѱ��.
				if (nullptr == pTransform)
					continue;

				// ������Ʈ Ÿ���� ������ �����Ѵ�.
				int eType = (int)Object->Get_Type();
				WriteFile(hFile, &eType, sizeof(int), &dwByte, NULL);

				// �ش� ��ü�� ��������� �����Ѵ�.
				CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
				TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();
				WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
			}
		}
		// ============================================================================================================







		// Layer Unbreakable
		// ============================================================================================================
		pLayer = pGameInstance->Find_Layer(LEVEL_MAP, TEXT("Layer_Unbreakable"));

		// ���̾ ���ٸ� return E_FAIL;
		if (nullptr == pLayer) {
			iSize = 0;
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);
		}
		else {

			// ���̾ �ִ� ������Ʈ �������� ������ �´�.
			ObjectList = pLayer->Get_ObjectList();

			// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
			if (ObjectList.empty())
				return;

			dwByte = 0;

			// ������Ʈ�� ������ �ִ´�.
			iSize = ObjectList.size();
			WriteFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, NULL);

			for (auto& Object : ObjectList) {

				// ������Ʈ�� ���� ��� ������ ���´�.
				CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

				// ���� ��������� ���ٸ� �ѱ��.
				if (nullptr == pTransform)
					continue;

				// ������Ʈ Ÿ���� ������ �����Ѵ�.
				int eType = (int)Object->Get_Type();
				WriteFile(hFile, &eType, sizeof(int), &dwByte, NULL);

				if (eType == OBJECT_RAIL_CURVE || eType == OBJECT_RAIL_STRAIGHT) {
					int a = 10;
				}


				// �ش� ��ü�� ��������� �����Ѵ�.
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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog	Dlg(TRUE,	// true�� ��� ���� ���, false �� ��� �ٸ� �̸����� ���� ���
		L"dat", // default ���� Ȯ���, ���� �� Ȯ���ڸ��� ��������� ������ ���� ������ Ȯ���ڸ��� ����ϰ� ������
		L"*.dat", // ��ȭ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ� ���� ���� �� ���޽��� ���� �ɼ�)
		L"Data File(*.dat)|*.dat||",	// ��ȭ ���ڿ� ǥ�õ� ���� ����, �޺��ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||
		this); // �θ� ������ �ּ�

	TCHAR	szPath[MAX_PATH] = L"";

	// ���� ������Ʈ�� �ִ� ���丮 ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_PATH, szPath);

	// ��ü ��ο��� �� ������ ��θ� �߶󳻴� �Լ�
	// ��� �� �� ������ ��ΰ� ���ϸ��̸� �� ���ϸ��� ������ ��θ� ����, ���� ���ϸ��� ������ ������ �������� �߶󳽴�.
	PathRemoveFileSpec(szPath);

	// data���� ��θ� �̾����
	lstrcat(szPath, L"\\Data\\Map");

	// ��ȭ���ڸ� ������ �� ���̴� �⺻ ��� ����, ���� ����� ��Ȳ
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : ��ȭ ���ڸ� ����, IDOK ���� ����
	if (IDOK == Dlg.DoModal())
	{

		// ���� ����
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
			// ������Ʈ ������ �д´�.
			SIZE_T iSize = 0;
			ReadFile(hFile, &iSize, sizeof(SIZE_T), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

			CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
			CToolView* m_pToolView = dynamic_cast<CToolView*>(m_pMain->m_MainSplitter.GetPane(0, 1));

			m_pGraphic_Device = m_pToolView->Get_Device();

			// �о�� ������Ʈ ������ŭ �ݺ��� ����
			for (int i = 0; i < iSize; ++i) {

				// ������Ʈ�� Ÿ���� �����´�.
				SIZE_T iType = 0;
				ReadFile(hFile, &iType, sizeof(SIZE_T), &dwByte, nullptr);

				// �ش� ��ü�� ��������� ��� �����Ѵ�.
				CTransform::TRANSFORMDESC TransDesc;
				ReadFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);

				// Ÿ�Կ� ���� �ٸ� VIBuffer ������Ʈ�� ����Ͽ� VIBuffer�� �����.
				switch (VIBuffer(iType))
				{
				case Engine::VIB_RECT:
					// �ϴ��� ť�길 �ٸ��� ���� �߰�
					break;
				case Engine::VIB_CUBE:
					// ���⼱ ���� ����Ʈ�� ��ü�� �߰��ϴ°��� �ƴ� 
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

			// �о�� ������Ʈ ������ŭ �ݺ��� ����
			for (int i = 0; i < iSize; ++i) {

				// ������Ʈ�� Ÿ���� �����´�.
				int iType = 0;
				ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

				// �ش� ��ü�� ��������� ��� �����Ѵ�.
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

				// Ÿ�Կ� ���� �ٸ� class�� ����Ͽ� ������Ʈ�� �����.
				switch ((OBJECT_TYPE)iType)
				{
					// �ڿ� - ��
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



					// �ڿ� - ����
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




					// �ڿ� - ö & ����
					// ========================================================================================================================
				case Engine::OBJECT_MATERIAL_IRON:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Iron"), &TransDesc);
					break;
				case Engine::OBJECT_MATERIAL_WOOD:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Wood"), &TransDesc);
					break;
					// ========================================================================================================================




					// �ǹ� - ������
					// ========================================================================================================================
				case Engine::OBJECT_PLATFORM_START:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_TrainStation"), &TransDesc);
					break;
				case Engine::OBJECT_PLATFORM_FINISH:
					break;
					// ========================================================================================================================



					// ������Ʈ - ����
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



					// ������Ʈ - ����
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



					// ������Ʈ - �߸�
					// ========================================================================================================================
				case Engine::OBJECT_CAMEL:
					pGameInstance->Add_GameObjectToLayer(LEVEL_MAP, Layer_Map, TEXT("Prototype_GameObject_Camel"), &TransDesc);
					break;
					// ========================================================================================================================



					// ������Ʈ - ����
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



					// �� �ͷ��� ������Ʈ - �� Ÿ�� ť��
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

			// �о�� ������Ʈ ������ŭ �ݺ��� ����
			for (int i = 0; i < iSize; ++i) {

				// ������Ʈ�� Ÿ���� �����´�.
				int iType = 0;
				ReadFile(hFile, &iType, sizeof(int), &dwByte, nullptr);

				// �ش� ��ü�� ��������� ��� �����Ѵ�.
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

				// Ÿ�Կ� ���� �ٸ� class�� ����Ͽ� ������Ʈ�� �����.
				switch ((OBJECT_TYPE)iType)
				{
					// �� �ͷ��� ������Ʈ - �μ����� �ʴ� ��
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

	// ��� ���ð� ���õ� switch��
	switch (m_nModeRadio) {
	case 0:
		// �������
		plevel->changeMode(CLevel_Map::MODE::MODE_CREATE);
		pCamera->SetObserving(false);
		break;

	case 1:
		// ���ø��
		plevel->changeMode(CLevel_Map::MODE::MODE_SELECT);
		pCamera->SetObserving(false);
		break;

	case 2:
		// ������ ���
		plevel->changeMode(CLevel_Map::MODE::MODE_CAMERA);
		pCamera->SetObserving(true);
		break;
	}

	UpdateData(FALSE);
}


BOOL CTab_Map::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CString tpath = _T("..\\Data\\MapObject\\*.*");

	//// �˻� Ŭ����
	//CFileFind finder;

	//// CFileFind�� ����, ���͸��� �����ϸ� TRUE �� ��ȯ��
	//BOOL bWorking = finder.FindFile(tpath); //

	//CString fileName;
	//CString DirName;

	//// Tile_ListBox �ʱ�ȭ
	//while (bWorking)
	//{
	//	//���� ���� or ���� �� �����ϸ�ٸ� TRUE ��ȯ
	//	bWorking = finder.FindNextFile();

	//	// folder �� ���� continue
	//	if (finder.IsDirectory() || finder.IsDots())
	//		continue;

	//	// ���� �϶�

	//	//������ �̸�
	//	CString _fileName = finder.GetFileName();

	//	fileName = finder.GetFileTitle();

	//	//�о�� ���� �̸��� ����Ʈ�ڽ��� map�� ����
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

	//	// ã�� ���ߴٸ�
	//	if (iter == m_vObjectFile.end()) {
	//		m_vObjectFile.push_back(szPath);
	//		m_ListBoxMapObject.AddString(fileName);
	//	}
	//}

	// ���ҽ� - ��
	AddStringToListBox(string("00. Rock1"));
	AddStringToListBox(string("01. Rock2"));
	AddStringToListBox(string("02. Rock3"));

	// ���ҽ� - ����
	AddStringToListBox(string("03. Tree_Dark1"));
	AddStringToListBox(string("04. Tree_Dark2"));
	AddStringToListBox(string("05. Tree_Dark3"));
	AddStringToListBox(string("06. Tree_Dark4"));
	AddStringToListBox(string("07. Tree_Light"));

	// ���� - ö & ����
	AddStringToListBox(string("08. Material_Iron"));
	AddStringToListBox(string("09. Material_Wood"));

	// ������Ʈ - ������
	AddStringToListBox(string("10. PlatformStart"));
	AddStringToListBox(string("11. PlatformFinish"));

	// ������Ʈ - ����
	AddStringToListBox(string("12. Train_Head"));
	AddStringToListBox(string("13. Train_Material"));
	AddStringToListBox(string("14. Train_Rail"));
	AddStringToListBox(string("15. Train_Water"));
	AddStringToListBox(string("16. Train_Dynamite"));

	// ������Ʈ - ���
	AddStringToListBox(string("17. Pickax"));
	AddStringToListBox(string("18. Axe"));
	AddStringToListBox(string("19. Bucket"));

	// ������Ʈ - �߸�
	AddStringToListBox(string("20. Camel"));

	// ������Ʈ - ��������
	AddStringToListBox(string("21. Rail_Straight"));
	AddStringToListBox(string("22. Rail_Curve"));

	// �� ������Ʈ - �μ��� ���� ��
	AddStringToListBox(string("23. Unbreakable Rock1"));
	AddStringToListBox(string("24. Unbreakable Rock2"));
	AddStringToListBox(string("25. Unbreakable Rock3"));

	// �� ������Ʈ - �� ť��
	AddStringToListBox(string("26. MapCube"));

	// ������Ʈ - ��������
	AddStringToListBox(string("27. Rail_End"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTab_Map::ApplySelectObjectScale()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString str;
	_uint iX, iY, iZ;

	// ũ��
	m_EditScaleX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditScaleY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditScaleZ.GetWindowText(str);
	iZ = _ttoi(str);

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// ���� ũ�� ��� ������ Ȯ��/���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString str;
	_uint iX, iY, iZ;

	// ũ��
	m_EditRotateX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditRotateY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditRotateZ.GetWindowText(str);
	iZ = _ttoi(str);

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// �� ��ġ���� ȸ��
	if (nullptr != pTransform) {
		// z�� ȸ��
		pTransform->Turned(_float3(0.f, 0.f, 1.f), iZ);
		// y�� ȸ��
		pTransform->Turned(_float3(0.f, 1.f, 0.f), iY);
		// x�� ȸ��
		pTransform->Turned(_float3(1.f, 0.f, 0.f), iX);
		
		//CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();
		//TransDesc.m_WorldMatrix = pTransform->Get_WorldMatrix();

		//pTransform->Set_TransformState(TransDesc);
	}


}


void CTab_Map::ApplySelectObjectTransform()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString str;
	_uint iX, iY, iZ;

	// ũ��
	m_EditTransformX.GetWindowText(str);
	iX = _ttoi(str);
	m_EditTransformY.GetWindowText(str);
	iY = _ttoi(str);
	m_EditTransformZ.GetWindowText(str);
	iZ = _ttoi(str);


	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	// ���� ��ġ���� �߰����� �̵�
	if (nullptr != pTransform) {
		_float3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos += _float3(iX, iY, iZ);
		pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}

}


void CTab_Map::ApplyObjectTransformation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���⼭ ����� ���� ������ �ƴ� ���庯ȯ ���������, �ѹ��� ���� �� �� �ִ� ���� ���,
	// �ش� ����ü�� ����Ͽ� ���� �����Ѵ�.

	CString str;
	float fX, fY, fZ;

	m_ObjectScaleX.GetWindowText(str);
	fX = _ttoi(str);
	m_ObjectScaleY.GetWindowText(str);
	fY = _ttoi(str);
	m_ObjectScaleZ.GetWindowText(str);
	fZ = _ttoi(str);

	// ũ��
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;

	// ȸ��
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString str;
	m_EditTextureNumber.GetWindowText(str);

	m_iTextureNumber = _ttoi(str);
}