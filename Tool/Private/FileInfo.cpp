#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";

	TCHAR		szCurDir[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurDir);


	//	szCurDir에서 strFullPath로 가는 상대경로를 구해서 szRelativePath에 저장해주는 함수
	// 단, 같은 드라이브 내에서만 가능
	PathRelativePathTo(szRelativePath, 
		szCurDir,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	// 경로 상의 모든 파일(*.*)을 찾기 위한 코드
	wstring	strFilePath = wstrPath + L"\\*.*";

	// 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// 주어진 경로에 파일의 유무를 확인하는 함수
	// 존재하지 않으면 false, 있으면 true

	// 만약 파일이 경로 안에 있는 것을 발견하고, 그 파일에 접근하고 싶을 경우
	// 반드시 FindNextFile함수를 호출해줘야 한다.
	// 이때 FindNextFile함수를 호출하면 가장 첫 번째 파일에 접근할 수 있다.
	BOOL	iContinued = Find.FindFile(strFilePath.c_str());
	
	while (iContinued)
	{
		// FindNextFile : 동일 경로 안에서 다음 파일들을 찾아준다. 더이상 찾을게 없거나 마지막 대상을 찾으면 0을 반환한다.
		iContinued = Find.FindNextFile();

		// 찾은 파일의 이름이 .이거나 ..이면 건너뛴다.
		if(Find.IsDots())
			continue;

		// 찾은 파일의 이름이 파일명이 아닌 경로명이면 재귀호출로 내부 탐색 지시
		else if (Find.IsDirectory())
		{
			// GetFilePath : 현재 찾은 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			// 드디어 찾았다
			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// 현재 경로에서 마지막 경로를 잘라내는 함수(현재는 파일명.확장자를 잘라냄)
			PathRemoveFileSpec(szPath);

			pImgPath->iCount = DirFileCount(szPath);

			// GetFileTitle : 파일 이름만 얻어오는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			// L"AKIHA_AKI01_000"

			// 맨뒤쪽에 있는 0을 잘라내고 그 뒤에 %d 정수값을 삽입해주자.
			// substr(시작, 끝) : 시작과 끝에 해당하는 문자열늘 남기고 나머지는 잘라냄
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// L"AKIHA_AKI01_00%d.png"

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);
			// D:\유준환\125기\Frame125\Texture\Stage\Player\Attack

			// PathCombine(1, 2, 3) : 2와 3을 이어붙여 1에 저장, 2와 3사이에 자동으로 \\가 삽입되어 저장
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// D:\유준환\125기\Frame125\Texture\Stage\Player\Attack\\AKIHA_AKI01_00%d.png

			// 절대 경로를 상대경로로 바꿔서 저장
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// ..\Frame125\Texture\Stage\Player\Attack\\AKIHA_AKI01_00%d.png
			PathRemoveFileSpec(szBuf);
			// ..\Frame125\Texture\Stage\Player\Attack\

			// PathFindFileName : 파일명을 찾거나 또는 가장 말단의 폴더명을 찾아냄
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			// ..\Frame125\Texture\Stage\Player\
			
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			iContinued = 0;
		}			
	}
}

int CFileInfo::DirFileCount(const wstring& wstrPath)
{
	// 경로 상의 모든 파일(*.*)을 찾기 위한 코드
	wstring	strFilePath = wstrPath + L"\\*.*";

	// 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// 주어진 경로에 파일의 유무를 확인하는 함수
	// 존재하지 않으면 false, 있으면 true

	// 만약 파일이 경로 안에 있는 것을 발견하고, 그 파일에 접근하고 싶을 경우
	// 반드시 FindNextFile함수를 호출해줘야 한다.
	// 이때 FindNextFile함수를 호출하면 가장 첫 번째 파일에 접근할 수 있다.
	BOOL	iContinued = Find.FindFile(strFilePath.c_str());

	int		iFileCnt = 0;

	while (iContinued)
	{
		iContinued = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if(Find.IsDirectory())
			continue;

		if(Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
