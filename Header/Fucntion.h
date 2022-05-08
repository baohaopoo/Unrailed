#pragma once

template<typename T>
bool		CompareY(T Dest, T Sour)
{
	return Dest->Get_Info().fY < Sour->Get_Info().fY;
}



class CFind_Tag
{
public:
	CFind_Tag(const TCHAR* pString)
		: m_pString(pString) {}

public:
	template<typename T>
	bool		operator()(T& Pair)
	{
		if (!lstrcmp(m_pString, Pair.first))
			return true;

		return false;
	}

private:
	const	TCHAR*		m_pString;
};

class CDeleteMap
{
public:
	template<typename T>
	void		operator()(T& Pair)
	{
		if (nullptr != Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

class CDeleteObj
{
public:
	template<typename T>
	void		operator()(T& iter)
	{
		if (nullptr != iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
};

static D3DXVECTOR3		Get_Mouse(void)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
}