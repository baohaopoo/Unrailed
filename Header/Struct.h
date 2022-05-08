#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;

	D3DXMATRIX		matWorld;

}INFO;

typedef	struct tagTexture
{
	// �ؽ�ó �İ�ü
	LPDIRECT3DTEXTURE9		pTexture;

	// �̹��� ������ �����ϱ� ���� ����ü
	D3DXIMAGE_INFO			tImgInfo;

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;

	BYTE			byOption;	// ���� �߿��� Ÿ�� �ɼ��� ���� ��ֹ� ������ �� ����
	BYTE			byDrawID;	// �� �� Ÿ�� �̹����ΰ�

	int				iIndex;			
	int				iParentIndex = 0;

}TILE;

typedef struct tagUnitData
{

#ifndef _AFX
	wstring		strName;

#else
	CString		strName;
#endif

	int			iAttack;
	int			iHp;
	BYTE		byJobIndex;
	BYTE		byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring			wstrObjKey = L"";
	wstring			wstrStateKey = L"";
	wstring			wstrPath = L"";
	int				iCount = 0;

}IMGPATH;

typedef struct tagFrame
{
	tagFrame() {}
	tagFrame(float _fFrame, float _fMax)
	: fFrame(_fFrame), fMax(_fMax) {}
	
	float		fFrame;	// ���� �ִϸ��̼� �ε���
	float		fMax;	// �ִ� �ε���
}FRAME;