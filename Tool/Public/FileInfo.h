#pragma once
#include "Struct.h"
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static	CString		ConvertRelativePath(CString strFullPath);

	// 폴더의 경로 정보를 추출하는 함수
	static void			DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList);

	// 폴더 안의 파일 개수 반환하는 함수
	static int			DirFileCount(const wstring& wstrPath);
};

