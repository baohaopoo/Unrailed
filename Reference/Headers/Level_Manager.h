#pragma once

#include "Base.h"

/* ���� ȭ�鿡 ��������ψd ������ �ּҸ� ������ �ִ´�. */
/* ���� ������ ƽ ����ȣ��. */
/* ���� ������ ���� ����ȣ��. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pLevel);		// ������ �ִ� ������ ������ �����ϰ� ���ڷ� ���� ������ ���� ������ �����ϴ� �Լ� 
	void Tick(_float fTimeDelta); /* ũ�� �ǹ̴� ����. */
	HRESULT Render(); /* ũ�� �ǹ̴� ����. */

	// Tool ���� �Լ�
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT SetLevel(_uint iNumLevels, class CLevel* pLevel);	// �������� �Ҵ��ϴ� �Լ�, ���ڷ� ���� �ε����� ���ڷ� ���� ������ �Ҵ����ش�. ���� ������ �̹� ������ �����Ѵٸ� ������ �ִ� ������ �����ϰ� �Ҵ��Ѵ�.
	HRESULT ChangeLevel(_uint iLevelIndex);		// ������ �ִ� ������ ���ΰ� ���ڷ� ���� ���� �ε����� �ִ� ������ ���� ������ �ٲٴ� �Լ�
	class CLevel* GetCurrLevel(void) { return m_ppLevels[m_iLevelIndex]; }	// ���� ����ϰ� �ִ� ������ ������ ��ȯ�ϴ� �Լ�

private:
	_uint					m_iLevelIndex = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

	// Tool ���� ��� ����
	_uint					m_iMaxLevelIndex = 0;		// �ִ� ����
	class CLevel**			m_ppLevels = nullptr;		// �������� ������ ��� �ִ� ����

public:
	virtual void Free() override;
};

END