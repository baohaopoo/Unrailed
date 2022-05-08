#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel * pLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// �ٲٱ��� ���� �ִ� ������ �����Ѵٸ�
	if(nullptr != m_pCurrentLevel)
	{
		// ���� ������ ������Ʈ �Ŵ����� ������ �ִ� ���������� ���� �����Ѵ�.
		if (FAILED(pGameInstance->Clear_LevelResource(m_iLevelIndex)))
			return E_FAIL;
	}

	Safe_Release(m_pCurrentLevel);
	
	// ���� ������ ���ڷ� ���� ������ ��ü�Ѵ�.
	m_pCurrentLevel = pLevel;

	// ���� �ε��� ���� ���� ������ ��ü�Ѵ�.
	m_iLevelIndex = iLevelIndex;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Manager::ChangeLevel(_uint iLevelIndex)
{
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	if ((nullptr != m_ppLevels[iLevelIndex]) && (m_iLevelIndex < m_iMaxLevelIndex)) {
		m_iLevelIndex = iLevelIndex;
		m_pCurrentLevel = m_ppLevels[m_iLevelIndex];

		Safe_Release(pGameInstance);

		return S_OK;
	}

	Safe_Release(pGameInstance);

	return E_FAIL;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

HRESULT CLevel_Manager::Reserve_Manager(_uint iNumLevels)
{
	// �̹� reserve�� �� ���
	if (0 != m_iMaxLevelIndex)
		return E_FAIL;

	// reserve�� ���� �ʾҴٸ� ���� �Ҵ����ش�.
	m_ppLevels = new CLevel*[iNumLevels];
	for (int i = 0; i < iNumLevels; ++i)
		m_ppLevels[i] = nullptr;

	// �Ҵ����� ������ŭ �ִ� ���� �ε����� �����Ѵ�.
	m_iMaxLevelIndex = iNumLevels;

	return S_OK;
}

HRESULT CLevel_Manager::SetLevel(_uint iNumLevels, CLevel* pLevel)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// ������ �����ϴ� ������ �ִٸ�
	if (nullptr != m_ppLevels[iNumLevels])
	{
		// �ش� ������ �����ϴ� ���̾� ����
		if (FAILED(pGameInstance->Clear_LevelResource(iNumLevels)))
			return E_FAIL;
	}

	// ���� ���� ����
	Safe_Release(m_ppLevels[iNumLevels]);

	// ���� ������ ��ġ�ϴ� ���� ���ڷ� ���� ���� �Ҵ� 
	m_ppLevels[iNumLevels] = pLevel;

	// �� �Լ����� ���� ��Ŀ�� �ǰ� �ִ� ������ �ٲ��� ����. �Ҵ縸 ��.

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_Manager::Free()
{
	if (nullptr != m_ppLevels) {
		for (int i = 0; i < m_iMaxLevelIndex; ++i) {
			Safe_Release(m_ppLevels[i]);
		}
	}

	// �ٽ� release �� �ʿ䰡 �ֳ�?
	//Safe_Release(m_pCurrentLevel);
}
