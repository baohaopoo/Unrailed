#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath, 
										const TCHAR* pStateKey /*= L""*/, 
										const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// 지정한 경로의 이미지 파일 정보를 얻어와 구조체에 기록하는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		MSG_BOX(pFilePath);
		Safe_Release(m_pTexInfo->pTexture);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	// D3DPOOL : dx 자원들을 어떤 메모리에 보관할 것인지 결정
	
	// D3DPOOL_DEFAULT = 0, 보통 그래픽 메모리 (동적 버퍼)
	// D3DPOOL_MANAGED = 1, 그래픽 메모리 + 메인 메모리(정적 버퍼)
	// D3DPOOL_SYSTEMMEM = 2, 메인 메모리
	// D3DPOOL_SCRATCH : 메인 메모리를 사용하지만 dx 장치로 접근불가


	// 텍스처 컴객체 생성 함수
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->Get_Device(), 
		pFilePath, 
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0, // d3dusage_RenderTarget인 경우 일반적인 텍스처가 아닌 화면 출력 용의 텍스처 생성
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,	// 이미지 필터링, 시스템이 지정한대로 수행 
		D3DX_DEFAULT,	// 밉맵을 이용한 확대 축소 필터링, 시스템이 지정한대로 수행
		0,				// 제거할 색상
		nullptr,/*&m_pTexInfo->tImgInfo,*/  // 이미지 정보 기록
		nullptr,// 팔레트 정보(8비트 시절 argb를 표현하기 위한 구조체) 
		&(m_pTexInfo->pTexture))))
	{
		MSG_BOX(L"Single Texture Load Failed");
		return E_FAIL;
	}


	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
