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


	//	szCurDir���� strFullPath�� ���� ����θ� ���ؼ� szRelativePath�� �������ִ� �Լ�
	// ��, ���� ����̺� �������� ����
	PathRelativePathTo(szRelativePath, 
		szCurDir,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	// ��� ���� ��� ����(*.*)�� ã�� ���� �ڵ�
	wstring	strFilePath = wstrPath + L"\\*.*";

	// ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	// �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�
	// �������� ������ false, ������ true

	// ���� ������ ��� �ȿ� �ִ� ���� �߰��ϰ�, �� ���Ͽ� �����ϰ� ���� ���
	// �ݵ�� FindNextFile�Լ��� ȣ������� �Ѵ�.
	// �̶� FindNextFile�Լ��� ȣ���ϸ� ���� ù ��° ���Ͽ� ������ �� �ִ�.
	BOOL	iContinued = Find.FindFile(strFilePath.c_str());
	
	while (iContinued)
	{
		// FindNextFile : ���� ��� �ȿ��� ���� ���ϵ��� ã���ش�. ���̻� ã���� ���ų� ������ ����� ã���� 0�� ��ȯ�Ѵ�.
		iContinued = Find.FindNextFile();

		// ã�� ������ �̸��� .�̰ų� ..�̸� �ǳʶڴ�.
		if(Find.IsDots())
			continue;

		// ã�� ������ �̸��� ���ϸ��� �ƴ� ��θ��̸� ���ȣ��� ���� Ž�� ����
		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			// ���� ã�Ҵ�
			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// ���� ��ο��� ������ ��θ� �߶󳻴� �Լ�(����� ���ϸ�.Ȯ���ڸ� �߶�)
			PathRemoveFileSpec(szPath);

			pImgPath->iCount = DirFileCount(szPath);

			// GetFileTitle : ���� �̸��� ������ �Լ�
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			// L"AKIHA_AKI01_000"

			// �ǵ��ʿ� �ִ� 0�� �߶󳻰� �� �ڿ� %d �������� ����������.
			// substr(����, ��) : ���۰� ���� �ش��ϴ� ���ڿ��� ����� �������� �߶�
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// L"AKIHA_AKI01_00%d.png"

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);
			// D:\����ȯ\125��\Frame125\Texture\Stage\Player\Attack

			// PathCombine(1, 2, 3) : 2�� 3�� �̾�ٿ� 1�� ����, 2�� 3���̿� �ڵ����� \\�� ���ԵǾ� ����
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// D:\����ȯ\125��\Frame125\Texture\Stage\Player\Attack\\AKIHA_AKI01_00%d.png

			// ���� ��θ� ����η� �ٲ㼭 ����
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// ..\Frame125\Texture\Stage\Player\Attack\\AKIHA_AKI01_00%d.png
			PathRemoveFileSpec(szBuf);
			// ..\Frame125\Texture\Stage\Player\Attack\

			// PathFindFileName : ���ϸ��� ã�ų� �Ǵ� ���� ������ �������� ã�Ƴ�
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
	// ��� ���� ��� ����(*.*)�� ã�� ���� �ڵ�
	wstring	strFilePath = wstrPath + L"\\*.*";

	// ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	// �־��� ��ο� ������ ������ Ȯ���ϴ� �Լ�
	// �������� ������ false, ������ true

	// ���� ������ ��� �ȿ� �ִ� ���� �߰��ϰ�, �� ���Ͽ� �����ϰ� ���� ���
	// �ݵ�� FindNextFile�Լ��� ȣ������� �Ѵ�.
	// �̶� FindNextFile�Լ��� ȣ���ϸ� ���� ù ��° ���Ͽ� ������ �� �ִ�.
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
