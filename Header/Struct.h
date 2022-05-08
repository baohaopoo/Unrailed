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
	// 텍스처 컴객체
	LPDIRECT3DTEXTURE9		pTexture;

	// 이미지 정보를 보관하기 위한 구조체
	D3DXIMAGE_INFO			tImgInfo;

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;

	BYTE			byOption;	// 수업 중에는 타일 옵션을 통해 장애물 판정을 할 예정
	BYTE			byDrawID;	// 몇 번 타일 이미지인가

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
	
	float		fFrame;	// 현재 애니메이션 인덱스
	float		fMax;	// 최대 인덱스
}FRAME;