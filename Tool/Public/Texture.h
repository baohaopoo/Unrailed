#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const	TEXINFO*	Get_Texture(const TCHAR* pStateKey = L"",	// 멀티 텍스처일 때만 사용
											const int& iCount = 0)PURE;

public:
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath, 
										const TCHAR* pStateKey = L"",	// 멀티 텍스처일 때만 사용
										const int& iCount = 0) PURE;

	virtual void		Release(void)PURE;
};

