#pragma once

#include "Base.h"

/* 현재 화면에 보여줘야하늕 레벨의 주소를 가지고 있는다. */
/* 현재 레벨의 틱 무한호출. */
/* 현재 레벨의 렌더 무한호출. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pLevel);		// 기존에 있던 레벨의 정보를 삭제하고 인자로 받은 레벨로 현재 레벨을 설정하는 함수 
	void Tick(_float fTimeDelta); /* 크게 의미는 없어. */
	HRESULT Render(); /* 크게 의미는 없어. */

	// Tool 전용 함수
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT SetLevel(_uint iNumLevels, class CLevel* pLevel);	// 레벨들을 할당하는 함수, 인자로 받은 인덱스에 인자로 받은 레벨을 할당해준다. 같은 레벨에 이미 레벨이 존재한다면 기존에 있는 레벨을 삭제하고 할당한다.
	HRESULT ChangeLevel(_uint iLevelIndex);		// 기존에 있던 레벨을 놔두고 인자로 받은 레벨 인덱스에 있는 레벨을 현재 레벨로 바꾸는 함수
	class CLevel* GetCurrLevel(void) { return m_ppLevels[m_iLevelIndex]; }	// 현재 사용하고 있는 레벨의 정보를 반환하는 함수

private:
	_uint					m_iLevelIndex = 0;
	class CLevel*			m_pCurrentLevel = nullptr;

	// Tool 전용 멤버 변수
	_uint					m_iMaxLevelIndex = 0;		// 최대 레벨
	class CLevel**			m_ppLevels = nullptr;		// 레벨들의 정보를 담고 있는 변수

public:
	virtual void Free() override;
};

END