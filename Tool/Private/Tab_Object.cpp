// Tab_Object.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Tab_Object.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "GameInstance.h"
#include "Camera_Default.h"

#include "Level_Object.h"
#include "MyForm.h"


// CTab_Object 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTab_Object, CDialogEx)

CTab_Object::CTab_Object(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TAB_OBJECT, pParent)
	, m_nModeRadio(2), m_nSelectObjectRadio(0), m_iTextureNumber(0)
	, m_iObjectRotateYAngle(0)
	, m_iMoveAll_X(0)
	, m_iMoveAll_Y(0)
	, m_iMoveAll_Z(0)
{

}

CTab_Object::~CTab_Object()
{
}

void CTab_Object::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON1, m_BtToolObjectSave);
	DDX_Control(pDX, IDC_BUTTON2, m_BtToolObjectLoad);

	DDX_Control(pDX, IDC_BUTTON4, m_BtMapObjectLoad);

	DDX_Radio(pDX, IDC_RADIO2, (int&)m_nModeRadio);
	DDX_Radio(pDX, IDC_RADIO5, (int&)m_nSelectObjectRadio);
	DDX_Control(pDX, IDC_EDIT11, m_EditScaleX);
	DDX_Control(pDX, IDC_EDIT12, m_EditScaleY);
	DDX_Control(pDX, IDC_EDIT2, m_EditScaleZ);
	DDX_Control(pDX, IDC_EDIT4, m_EditRotateX);
	DDX_Control(pDX, IDC_EDIT6, m_EditRotateY);
	DDX_Control(pDX, IDC_EDIT5, m_EditRotateZ);
	DDX_Control(pDX, IDC_EDIT7, m_EditTransformX);
	DDX_Control(pDX, IDC_EDIT9, m_EditTransformY);
	DDX_Control(pDX, IDC_EDIT8, m_EditTransformZ);
	DDX_Control(pDX, IDC_BUTTON7, m_BtColor);
	DDX_Control(pDX, IDC_BUTTON3, m_BtApplyScale);
	DDX_Control(pDX, IDC_BUTTON8, m_BtApplyRotation);
	DDX_Control(pDX, IDC_BUTTON9, m_BtApplyTransform);
	DDX_Control(pDX, IDC_EDIT3, m_EditTextureNumber);
	DDX_Control(pDX, IDC_CHECK1, m_CheckMinus);
	DDX_Text(pDX, IDC_EDIT13, m_iObjectRotateYAngle);
	DDX_Text(pDX, IDC_EDIT14, m_iMoveAll_X);
	DDX_Text(pDX, IDC_EDIT15, m_iMoveAll_Y);
	DDX_Text(pDX, IDC_EDIT16, m_iMoveAll_Z);
	DDX_Control(pDX, IDC_CHECK2, m_BtCheckMoveAllMinus);
}


BEGIN_MESSAGE_MAP(CTab_Object, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO2, IDC_RADIO4, RadioCtrl)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO5, IDC_RADIO7, RadioCtrl)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON3, &CTab_Object::ClickScaleApply)
	ON_BN_CLICKED(IDC_BUTTON8, &CTab_Object::ClickRotationApply)
	ON_BN_CLICKED(IDC_BUTTON9, &CTab_Object::ClickTransformApply)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Object::SaveToolObjectData)
	ON_BN_CLICKED(IDC_BUTTON2, &CTab_Object::LoadToolObjectData)
	ON_BN_CLICKED(IDC_BUTTON4, &CTab_Object::EveryObjectRotateY)
	ON_BN_CLICKED(IDC_BUTTON6, &CTab_Object::ClickTextureNumberApply)
	ON_BN_CLICKED(IDC_BUTTON10, &CTab_Object::ChangeSelectObjectTexture)
	ON_BN_CLICKED(IDC_BUTTON5, &CTab_Object::MoveEveryObject)
	ON_BN_CLICKED(IDC_BUTTON7, &CTab_Object::ObjectReversal_Y)
END_MESSAGE_MAP()


void CTab_Object::RadioCtrl(UINT id)
{
	UpdateData(TRUE);
	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CGameObject* pObject = pGameInstance->Find_Object(LEVEL_OBJECT, Layer_Camera_Object, string("camera"));
	CCamera_Default* pCamera = dynamic_cast<CCamera_Default*>(pObject);
	
	CLevel_Object* plevel = dynamic_cast<CLevel_Object*>(pGameInstance->GetCurrLevel());

	// CMainFrame* m_pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	// CToolView* m_pToolView = dynamic_cast<CToolView*>(m_pMain->m_MainSplitter.GetPane(0, 1));

	if (id == IDC_RADIO2)
		m_nModeRadio = 0;
	if (id == IDC_RADIO3)
		m_nModeRadio = 1;
	if (id == IDC_RADIO4)
		m_nModeRadio = 2;

	// 모드 선택과 관련된 switch문
	switch (m_nModeRadio) {
	case 0:
		// 생성모드
		plevel->changeMode(CLevel_Object::MODE::MODE_CREATE);
		pCamera->SetObserving(false);
		break;

	case 1:
		// 선택모드
		plevel->changeMode(CLevel_Object::MODE::MODE_SELECT);
		pCamera->SetObserving(false);
		break;

	case 2:
		// 움직임 모드
		plevel->changeMode(CLevel_Object::MODE::MODE_CAMERA);
		pCamera->SetObserving(true);
		break;
	}

	// 선택한 오브젝트와 관련된 switch문
	switch (m_nSelectObjectRadio) {
	case 0:
		break;

	case 1:
		break;

	case 2:
		break;
	}

	UpdateData(FALSE);
}


// 큐브 버튼을 눌렀을 경우
void CTab_Object::Click_CubeButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 큐브 버튼을 눌러 만들었으므로, 
	// level_object에 있는 곳으로 이동하여 현재 선택한 오브젝트가 cube이니,
	// 해당 형식으로 오브젝트를 생성하라 한다.

}

int CTab_Object::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetTimer(0, 1000 / 60, NULL);	// 1초에 60번

	return 0;
}

void CTab_Object::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CTab_Object::ClickScaleApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pSelectObject)
		return;

	CString str;
	float fScaleX, fScaleY, fScaleZ;

	m_EditScaleX.GetWindowText(str);
	fScaleX = _ttof(str);

	m_EditScaleY.GetWindowText(str);
	fScaleY = _ttof(str);

	m_EditScaleZ.GetWindowText(str);
	fScaleZ = _ttof(str);

	D3DXMATRIX ScaleMatrix;
	D3DXMatrixIdentity(&ScaleMatrix);

	D3DXMatrixScaling(&ScaleMatrix, fScaleX, fScaleY, fScaleZ);

	// 구한 스케일값을 오브젝트의 스케일 행렬에 각각 곱한다.
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	_float3		vRight = pTransform->Get_LocalState(CTransform::STATE_RIGHT);
	_float3		vUp = pTransform->Get_LocalState(CTransform::STATE_UP);
	_float3		vLook = pTransform->Get_LocalState(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	vRight *= fScaleX;
	vUp *= fScaleY;
	vLook *= fScaleZ;

	pTransform->Set_LocalState(CTransform::STATE_RIGHT, vRight);
	pTransform->Set_LocalState(CTransform::STATE_UP, vUp);
	pTransform->Set_LocalState(CTransform::STATE_LOOK, vLook);
}


void CTab_Object::ClickRotationApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pSelectObject)
		return;

	CString str;
	float fRight, fUp, fLook;

	m_EditRotateX.GetWindowText(str);
	fRight = _ttof(str);

	m_EditRotateY.GetWindowText(str);
	fUp = _ttof(str);

	m_EditRotateZ.GetWindowText(str);
	fLook = _ttof(str);

	// 칸에 적혀있는 값을 float 형식으로 받아온다. 해당 값들을 적용하여 회전 행렬을 만들어 오브젝트가 존재할 시,
	// 해당 오브젝트의 로컬 회전행렬에 적용시킨다. 행렬의 곱셈순서는 x,y,z로 적용순서는 z,y,x순이다.
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));
	CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();

	// 행렬들을 항등 행렬로 바꾼다.
	D3DXMatrixIdentity(&TransDesc.m_LocalRotate);

	_float3		vRight = pTransform->Get_LocalState(CTransform::STATE_RIGHT);
	_float3		vUp = pTransform->Get_LocalState(CTransform::STATE_UP);
	_float3		vLook = pTransform->Get_LocalState(CTransform::STATE_LOOK);

	D3DXMatrixRotationAxis(&TransDesc.m_LocalRotate, &vUp, D3DXToRadian(fUp));

	D3DXVec3TransformNormal(&vRight, &vRight, &TransDesc.m_LocalRotate);
	D3DXVec3TransformNormal(&vUp, &vUp, &TransDesc.m_LocalRotate);
	D3DXVec3TransformNormal(&vLook, &vLook, &TransDesc.m_LocalRotate);

	pTransform->Set_LocalState(CTransform::STATE_RIGHT, vRight);
	pTransform->Set_LocalState(CTransform::STATE_UP, vUp);
	pTransform->Set_LocalState(CTransform::STATE_LOOK, vLook);
}


void CTab_Object::ClickTransformApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_pSelectObject)
		return;

	CString str;
	float fTransformX, fTransformY, fTransformZ;

	m_EditTransformX.GetWindowText(str);
	fTransformX = _ttof(str);

	m_EditTransformY.GetWindowText(str);
	fTransformY = _ttof(str);

	m_EditTransformZ.GetWindowText(str);
	fTransformZ = _ttof(str);

	D3DXMATRIX TransformMatrix;
	D3DXMatrixIdentity(&TransformMatrix);

	if (m_CheckMinus.GetCheck()) {
		fTransformX *= -1;
		fTransformY *= -1;
		fTransformZ *= -1;
	}

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));

	_float3		vPos = pTransform->Get_LocalState(CTransform::STATE_POSITION);

	vPos += _float3(fTransformX, fTransformY, fTransformZ);

	pTransform->Set_LocalState(CTransform::STATE_POSITION, vPos);
}

// 맵에서 사용할 데이터 사용, 각각의 VIBuffer를 사용한다.
void CTab_Object::SaveToolObjectData()
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
	lstrcat(szPath, L"\\Data\\ToolObject");

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

		// 저장할 정보가 담긴 레이어를 찾는다.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

		// 레이어가 없다면 return E_FAIL;
		if (nullptr == pLayer)
			return;

		// 레이어에 있는 오브젝트 정보들을 가지고 온다.
		list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

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
			TransDesc.m_iTextureNumber = Object->Get_TextureNumber();
			WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
		}
		CloseHandle(hFile);
	}

}

// 맵에서 사용할 데이터 사용, 각각의 VIBuffer를 사용한다.
void CTab_Object::LoadToolObjectData()
{
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
	lstrcat(szPath, L"\\Data\\ToolObject");

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

		while (true)
		{
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
					pGameInstance->Add_GameObjectToLayer(LEVEL_OBJECT, Layer_Object, Prototype_Object_CubeObject, &TransDesc);
					break;
				default:
					break;
				}
			}
		}
		CloseHandle(hFile);
	}
}

// 맵에서 사용할 데이터 사용, VIBuffer_Object를 사용한다.
void CTab_Object::EveryObjectRotateY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// 저장할 정보가 담긴 레이어를 찾는다.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// 레이어가 없다면 return E_FAIL;
	if (nullptr == pLayer)
		return;

	// 레이어에 있는 오브젝트 정보들을 가지고 온다.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
	if (ObjectList.empty())
		return;

	// 오브젝트 리스트의 정보를 가져온다.
	for (auto& Object : ObjectList) {
		CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

		CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();

		_float4x4 MatrixRotateY;
		D3DXMatrixRotationY(&MatrixRotateY, D3DXToRadian(m_iObjectRotateYAngle));

		TransDesc.m_LocalMatrix *= MatrixRotateY;

		dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Set_TransformState(TransDesc);
	}

	UpdateData(FALSE);
}

void CTab_Object::ClickTextureNumberApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString str;

	m_EditTextureNumber.GetWindowText(str);
	m_iTextureNumber = _ttoi(str);
}


void CTab_Object::ChangeSelectObjectTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == m_pSelectObject)
		return;

	CString str;

	m_EditTextureNumber.GetWindowText(str);
	m_pSelectObject->Set_TextureNumber(_ttoi(str));
}

void CTab_Object::MoveEveryObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// 저장할 정보가 담긴 레이어를 찾는다.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// 레이어가 없다면 return E_FAIL;
	if (nullptr == pLayer)
		return;

	// 레이어에 있는 오브젝트 정보들을 가지고 온다.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
	if (ObjectList.empty())
		return;

	// 오브젝트 리스트의 정보를 가져온다.
	for (auto& Object : ObjectList) {
		CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));
		
		CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();

		_float3 vPos{};
		memcpy(&vPos, TransDesc.m_LocalMatrix.m[3], sizeof(_float3));

		if (m_BtCheckMoveAllMinus.GetCheck())
			vPos -= _float3(m_iMoveAll_X, m_iMoveAll_Y, m_iMoveAll_Z);
		else
			vPos += _float3(m_iMoveAll_X, m_iMoveAll_Y, m_iMoveAll_Z);

		memcpy(TransDesc.m_LocalMatrix.m[3], &vPos, sizeof(_float3));

		dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Set_TransformState(TransDesc);
	}

	UpdateData(FALSE);
}

void CTab_Object::ObjectReversal_Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// 저장할 정보가 담긴 레이어를 찾는다.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// 레이어가 없다면 return E_FAIL;
	if (nullptr == pLayer)
		return;

	// 레이어에 있는 오브젝트 정보들을 가지고 온다.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// 가지고 온 정보가 비어있다면, 저장할 수 없으므로 return E_FAIL;
	if (ObjectList.empty())
		return;

	// 오브젝트 리스트의 정보를 가져온다.
	for (auto& Object : ObjectList) {
		CTransform* pTransform = dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform));

		CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();

		_float3 vPos{};
		memcpy(&vPos, TransDesc.m_LocalMatrix.m[3], sizeof(_float3));

		vPos.x *= -1;

		memcpy(TransDesc.m_LocalMatrix.m[3], &vPos, sizeof(_float3));

		dynamic_cast<CTransform*>(Object->Get_Component(Component_Transform))->Set_TransformState(TransDesc);
	}

	UpdateData(FALSE);
}
