#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
{
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
}

void CGameInstance::Tick_Engine(_uint iNumLevel, _float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pInput_Device)
		return;

	// ����̽� ����
	m_pInput_Device->SetUp_InputDeviceState();

	// ��ŷ�� ���� ��ŷ Ȱ��ȭ
	m_pPick_Manager->Tick();

	// �浹 ó���� ������ ������Ʈ Ȱ��
	m_pObject_Manager->Tick(iNumLevel, fTimeDelta);

	// ������Ʈ ��ó�� �� �Ҹ� ó��
	m_pObject_Manager->LateTick(iNumLevel, fTimeDelta);

	// ���� �Ŵ��� Ȱ��, �ǹ̾���
	m_pLevel_Manager->Tick(fTimeDelta);	// �ǹ̾����� �ֱ��Ѵ�.

	// ��ŷ ���� ��Ȱ��
	m_pPick_Manager->LateTick();
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, LPDIRECT3DDEVICE9* ppGraphic_Device)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pInput_Device ||
		nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppGraphic_Device)))
		return E_FAIL;	

	if (FAILED(m_pInput_Device->InitDevice(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	m_pPick_Manager = m_pPick_Manager->Create(*ppGraphic_Device);
	m_pCollision_Manager = m_pCollision_Manager->Create(*ppGraphic_Device);
	m_pFrustum_Manager = m_pFrustum_Manager->Create(*ppGraphic_Device);

	if (FAILED(m_pPick_Manager->InitDevice(GraphicDesc.hWnd, GraphicDesc.iWinCX, GraphicDesc.iWinCY)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pInput_Device ||
		nullptr == m_pCollision_Manager)
		return;

	m_pInput_Device->SetUp_InputDeviceState();

	// �浹ó�� Ȱ��
	m_pCollision_Manager->Tick();

	// �������� ����
	m_pFrustum_Manager->Tick();

	// �浹 ó���� ������ ������Ʈ Ȱ��
	m_pObject_Manager->Tick(fTimeDelta);

	// ������Ʈ ��ó�� �� �Ҹ� ó��
	m_pObject_Manager->LateTick(fTimeDelta);

	// �浹ó�� ���� ��Ȱ��
	m_pCollision_Manager->LateTick();

	// �ǹ̾���
	m_pLevel_Manager->Tick(fTimeDelta);

	m_pPick_Manager->Tick();
	m_pPick_Manager->LateTick();
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Render();
//	m_pCollision_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Render_Begin(void)
{
	if (nullptr == m_pGraphic_Device)
		return;

	return m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End()
{
	if (nullptr == m_pGraphic_Device)
		return;

	return m_pGraphic_Device->Render_End();
}

bool CGameInstance::Get_DIKeyState(_uchar byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);	
}

_char CGameInstance::Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE eDIMBState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMButtonState(eDIMBState);
}

_long CGameInstance::Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eDIMMState);
}

HRESULT CGameInstance::Add_Timers(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timers(pTimerTag);	
}

_double CGameInstance::Compute_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->OpenLevel(iLevelIndex, pLevel);
}

HRESULT CGameInstance::ChangeLevel(_uint iLevelIndex)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->ChangeLevel(iLevelIndex);
}

HRESULT CGameInstance::SetLevel(_uint iNumLevels, CLevel * pLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->SetLevel(iNumLevels, pLevel);
}

CLevel * CGameInstance::GetCurrLevel(void)
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->GetCurrLevel();
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);	
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObjectToLayer(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_GameObject(iLevelIndex, pLayerTag, pObject);
}

HRESULT CGameInstance::Delete_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_Layer(iLevelIndex, pLayerTag);
}

CGameObject * CGameInstance::Find_Object(_uint iLevelIndex, const _tchar * pLayerTag, const string & _strObjectName)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Object(iLevelIndex, pLayerTag, _strObjectName);
}

CLayer * CGameInstance::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Layer(iLevelIndex, pLayerTag);
}

map<const _tchar*, class CLayer*>& CGameInstance::Find_LevelLayers(_uint iLevelIndex)
{
	if (nullptr != m_pObject_Manager) {
		return m_pObject_Manager->Find_LevelLayers(iLevelIndex);
	}
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

float CGameInstance::Pick(D3DXVECTOR3 & _p1, D3DXVECTOR3 & _p2, D3DXVECTOR3 & _p3)
{
	if (nullptr == m_pPick_Manager)
		return -1.f;

	return m_pPick_Manager->Pick(_p1, _p2, _p3);
}

void CGameInstance::Tick(void)
{
	if (nullptr == m_pPick_Manager)
		return;

	return m_pPick_Manager->Tick();
}

HRESULT CGameInstance::AddCollisionLayers(_uint _iNumLevel, const TCHAR * _pLayerTag1, const TCHAR * _pLayerTag2)
{
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

	return m_pCollision_Manager->AddCollisionLayers(_iNumLevel, _pLayerTag1, _pLayerTag2);
}

bool CGameInstance::CheckIntersect(_float3 center1, float radius1, _float3 center2, float radius2)
{
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

	return m_pCollision_Manager->CheckIntersect(center1, radius1, center2, radius2);
}

bool CGameInstance::Check_InOut(const _float4x4& Object_WorldMatrix)
{
	if (nullptr == m_pFrustum_Manager)
		return E_FAIL;

	return m_pFrustum_Manager->Check_InOut(Object_WorldMatrix);
}

HRESULT CGameInstance::InitPickManager(HWND hWnd, _uint _iWinCX, _uint _iWinCY, _uint _iStartWinPosX, _uint _iStartWinPosY)
{
	if (nullptr == m_pPick_Manager)
		return E_FAIL;

	return m_pPick_Manager->InitDevice(hWnd, _iWinCX, _iWinCY, _iStartWinPosX, _iStartWinPosY);
}

void CGameInstance::MatchPrecisionPoint(_float3 & _fPoint)
{
	if (nullptr == m_pPick_Manager)
		return;

	return m_pPick_Manager->MatchPrecisionPoint(_fPoint);
}

CPick_Manager::POLYGON* CGameInstance::Picking(_uint _iNumLevel)
{
	if (nullptr == m_pPick_Manager)
		return nullptr;

	return m_pPick_Manager->Picking(_iNumLevel);
}

CGameObject * CGameInstance::Picking_Layer(_uint _iNumLevel, const _tchar * _tagLayer)
{
	if (nullptr == m_pPick_Manager)
		return nullptr;

	return m_pPick_Manager->Picking_Layer(_iNumLevel, _tagLayer);
}

CGameObject * CGameInstance::Picking_Object(_uint _iNumLevel)
{
	if (nullptr == m_pPick_Manager)
		return nullptr;

	return m_pPick_Manager->Picking_Object(_iNumLevel);
}

_float3 CGameInstance::TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3)
{
	if (nullptr == m_pPick_Manager)
		return _float3(0.f, 0.f, 0.f);

	return m_pPick_Manager->TriBevelMiddle(p1, p2, p3);
}

_float3 CGameInstance::TriBevelMiddle(CPick_Manager::POLYGON & polygon)
{
	if (nullptr == m_pPick_Manager)
		return _float3(0.f, 0.f, 0.f);

	return m_pPick_Manager->TriBevelMiddle(polygon);
}

void CGameInstance::Release_Engine()
{

	CGameInstance::GetInstance()->DestroyInstance();	

	CLevel_Manager::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CGraphic_Device::GetInstance()->DestroyInstance();	
}

void CGameInstance::Free()
{
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
