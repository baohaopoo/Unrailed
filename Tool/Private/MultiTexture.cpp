#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	/*auto	iter = find_if(m_mapMultiTexture.begin(), m_mapMultiTexture.end(), CFind_Tag(pStateKey));

	if (iter == m_mapMultiTexture.end())
		return nullptr;*/

	auto	iter = find_if(m_mapMultiTexture.begin(), m_mapMultiTexture.end(), 
		[&](auto& Pair)->bool
	{
		if (Pair.first == pStateKey)
			return true;

		return false;
	});

	if (iter == m_mapMultiTexture.end())
		return nullptr;


	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	// 경로의 길이는 절대 520바이트를 넘으면 안된다.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// 지정한 경로의 이미지 파일 정보를 얻어와 구조체에 기록하는 함수
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			MSG_BOX(szFullPath);
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		// 텍스처 컴객체 생성 함수
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // d3dusage_RenderTarget인 경우 일반적인 텍스처가 아닌 화면 출력 용의 텍스처 생성
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,	// 이미지 필터링, 시스템이 지정한대로 수행 
			D3DX_DEFAULT,	// 밉맵을 이용한 확대 축소 필터링, 시스템이 지정한대로 수행
			0,				// 제거할 색상
			nullptr,/*&m_pTexInfo->tImgInfo,*/  // 이미지 정보 기록
			nullptr,// 팔레트 정보(8비트 시절 argb를 표현하기 위한 구조체) 
			&(pTexInfo->pTexture))))
		{
			MSG_BOX(L"Multi Texture Load Failed");
			return E_FAIL;
		}

		m_mapMultiTexture[pStateKey].push_back(pTexInfo);
	}


	return S_OK;
}

void CMultiTexture::Release(void)
{
	for (auto& MapIter : m_mapMultiTexture)
	{
		for (auto& Iter : MapIter.second)
		{
			Iter->pTexture->Release();		// 텍스처 컴객체
			Safe_Delete<TEXINFO*>(Iter);	// 구초체			
		}
		MapIter.second.clear();				// 벡터 클리어
	}

	m_mapMultiTexture.clear();				// 맵 클리어
}
