// Tab_Object.cpp : ���� �����Դϴ�.
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


// CTab_Object ��ȭ �����Դϴ�.

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

	// ��� ���ð� ���õ� switch��
	switch (m_nModeRadio) {
	case 0:
		// �������
		plevel->changeMode(CLevel_Object::MODE::MODE_CREATE);
		pCamera->SetObserving(false);
		break;

	case 1:
		// ���ø��
		plevel->changeMode(CLevel_Object::MODE::MODE_SELECT);
		pCamera->SetObserving(false);
		break;

	case 2:
		// ������ ���
		plevel->changeMode(CLevel_Object::MODE::MODE_CAMERA);
		pCamera->SetObserving(true);
		break;
	}

	// ������ ������Ʈ�� ���õ� switch��
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


// ť�� ��ư�� ������ ���
void CTab_Object::Click_CubeButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	// ť�� ��ư�� ���� ��������Ƿ�, 
	// level_object�� �ִ� ������ �̵��Ͽ� ���� ������ ������Ʈ�� cube�̴�,
	// �ش� �������� ������Ʈ�� �����϶� �Ѵ�.

}

int CTab_Object::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	SetTimer(0, 1000 / 60, NULL);	// 1�ʿ� 60��

	return 0;
}

void CTab_Object::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialogEx::OnTimer(nIDEvent);
}

void CTab_Object::ClickScaleApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ���� �����ϰ��� ������Ʈ�� ������ ��Ŀ� ���� ���Ѵ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ĭ�� �����ִ� ���� float �������� �޾ƿ´�. �ش� ������ �����Ͽ� ȸ�� ����� ����� ������Ʈ�� ������ ��,
	// �ش� ������Ʈ�� ���� ȸ����Ŀ� �����Ų��. ����� ���������� x,y,z�� ��������� z,y,x���̴�.
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pSelectObject->Get_Component(Component_Transform));
	CTransform::TRANSFORMDESC TransDesc = pTransform->Get_TransformState();

	// ��ĵ��� �׵� ��ķ� �ٲ۴�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

// �ʿ��� ����� ������ ���, ������ VIBuffer�� ����Ѵ�.
void CTab_Object::SaveToolObjectData()
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
	lstrcat(szPath, L"\\Data\\ToolObject");

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

		// ������ ������ ��� ���̾ ã�´�.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

		// ���̾ ���ٸ� return E_FAIL;
		if (nullptr == pLayer)
			return;

		// ���̾ �ִ� ������Ʈ �������� ������ �´�.
		list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

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
			TransDesc.m_iTextureNumber = Object->Get_TextureNumber();
			WriteFile(hFile, &TransDesc, sizeof(CTransform::TRANSFORMDESC), &dwByte, NULL);
		}
		CloseHandle(hFile);
	}

}

// �ʿ��� ����� ������ ���, ������ VIBuffer�� ����Ѵ�.
void CTab_Object::LoadToolObjectData()
{
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
	lstrcat(szPath, L"\\Data\\ToolObject");

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

		while (true)
		{
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

// �ʿ��� ����� ������ ���, VIBuffer_Object�� ����Ѵ�.
void CTab_Object::EveryObjectRotateY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ������ ������ ��� ���̾ ã�´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// ���̾ ���ٸ� return E_FAIL;
	if (nullptr == pLayer)
		return;

	// ���̾ �ִ� ������Ʈ �������� ������ �´�.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
	if (ObjectList.empty())
		return;

	// ������Ʈ ����Ʈ�� ������ �����´�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString str;

	m_EditTextureNumber.GetWindowText(str);
	m_iTextureNumber = _ttoi(str);
}


void CTab_Object::ChangeSelectObjectTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (nullptr == m_pSelectObject)
		return;

	CString str;

	m_EditTextureNumber.GetWindowText(str);
	m_pSelectObject->Set_TextureNumber(_ttoi(str));
}

void CTab_Object::MoveEveryObject()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ������ ������ ��� ���̾ ã�´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// ���̾ ���ٸ� return E_FAIL;
	if (nullptr == pLayer)
		return;

	// ���̾ �ִ� ������Ʈ �������� ������ �´�.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
	if (ObjectList.empty())
		return;

	// ������Ʈ ����Ʈ�� ������ �����´�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ������ ������ ��� ���̾ ã�´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_OBJECT, Layer_Object);

	// ���̾ ���ٸ� return E_FAIL;
	if (nullptr == pLayer)
		return;

	// ���̾ �ִ� ������Ʈ �������� ������ �´�.
	list<CGameObject*>& ObjectList = pLayer->Get_ObjectList();

	// ������ �� ������ ����ִٸ�, ������ �� �����Ƿ� return E_FAIL;
	if (ObjectList.empty())
		return;

	// ������Ʈ ����Ʈ�� ������ �����´�.
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
