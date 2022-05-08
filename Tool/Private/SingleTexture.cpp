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

	// ������ ����� �̹��� ���� ������ ���� ����ü�� ����ϴ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		MSG_BOX(pFilePath);
		Safe_Release(m_pTexInfo->pTexture);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	// D3DPOOL : dx �ڿ����� � �޸𸮿� ������ ������ ����
	
	// D3DPOOL_DEFAULT = 0, ���� �׷��� �޸� (���� ����)
	// D3DPOOL_MANAGED = 1, �׷��� �޸� + ���� �޸�(���� ����)
	// D3DPOOL_SYSTEMMEM = 2, ���� �޸�
	// D3DPOOL_SCRATCH : ���� �޸𸮸� ��������� dx ��ġ�� ���ٺҰ�


	// �ؽ�ó �İ�ü ���� �Լ�
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->Get_Device(), 
		pFilePath, 
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0, // d3dusage_RenderTarget�� ��� �Ϲ����� �ؽ�ó�� �ƴ� ȭ�� ��� ���� �ؽ�ó ����
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,	// �̹��� ���͸�, �ý����� �����Ѵ�� ���� 
		D3DX_DEFAULT,	// �Ӹ��� �̿��� Ȯ�� ��� ���͸�, �ý����� �����Ѵ�� ����
		0,				// ������ ����
		nullptr,/*&m_pTexInfo->tImgInfo,*/  // �̹��� ���� ���
		nullptr,// �ȷ�Ʈ ����(8��Ʈ ���� argb�� ǥ���ϱ� ���� ����ü) 
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
