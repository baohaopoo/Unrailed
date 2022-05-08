#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const	TEXINFO*	Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",	// 멀티 텍스처일 때만 사용
									const int& iCount = 0) ;


public:
	HRESULT		InsertTexture(TEXTUREID eID,
							const TCHAR* pFilePath, 
							const TCHAR* pObjKey, 
							const TCHAR* pStateKey = L"",	// 멀티 텍스처일 때만 사용
							const int& iCount = 0);

	void		Release(void);


private:
	map<wstring, CTexture*>		m_mapTexture;

};

