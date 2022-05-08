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
	// ����� ���̴� ���� 520����Ʈ�� ������ �ȵȴ�.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// ������ ����� �̹��� ���� ������ ���� ����ü�� ����ϴ� �Լ�
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			MSG_BOX(szFullPath);
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		// �ؽ�ó �İ�ü ���� �Լ�
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // d3dusage_RenderTarget�� ��� �Ϲ����� �ؽ�ó�� �ƴ� ȭ�� ��� ���� �ؽ�ó ����
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,	// �̹��� ���͸�, �ý����� �����Ѵ�� ���� 
			D3DX_DEFAULT,	// �Ӹ��� �̿��� Ȯ�� ��� ���͸�, �ý����� �����Ѵ�� ����
			0,				// ������ ����
			nullptr,/*&m_pTexInfo->tImgInfo,*/  // �̹��� ���� ���
			nullptr,// �ȷ�Ʈ ����(8��Ʈ ���� argb�� ǥ���ϱ� ���� ����ü) 
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
			Iter->pTexture->Release();		// �ؽ�ó �İ�ü
			Safe_Delete<TEXINFO*>(Iter);	// ����ü			
		}
		MapIter.second.clear();				// ���� Ŭ����
	}

	m_mapMultiTexture.clear();				// �� Ŭ����
}
