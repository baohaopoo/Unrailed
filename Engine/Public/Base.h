#pragma once

#include "Engine_Defines.h"

/* 1. ��� Ŭ�������� �θ𰡵ȴ�. */
/* 2. ���۷���ī��Ʈ�� ���� ������ �Ѵ�. */
/* 2_1. ���۷���ī��Ʈ : ��������. ����(��ü�޸������� ���� ���ٰ����ϵ��� ���� ����:������) */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷���ī��Ʈ�� ������Ų��. */
	/* unsigned long  : ������Ű�������� ���۷���ī��Ʈ�� ��ȯ*/
	unsigned long AddRef();

	/* ���۷���ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�.  */
	/* unsigned long  : �����ϱ� ������ ���۷��� ī��Ʈ�� ��. */
	unsigned long Release();

private:
	/*���������� �����ϰ��ִ� �����̴�. */
	unsigned long	m_dwRefCnt = 0;

public:
	virtual void Free() = 0;
};

END