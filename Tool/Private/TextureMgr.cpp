#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)


CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),
		[&](auto& Pair)->bool
	{
		if (Pair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::InsertTexture(TEXTUREID eID, const TCHAR * pFilePath, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),
		[&](auto& Pair)->bool
	{
		if (Pair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eID)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			MSG_BOX(pFilePath);
			return E_FAIL;
		}

		m_mapTexture.emplace(pObjKey, pTexture);
	}
		

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), CDeleteMap());
	m_mapTexture.clear();

}
