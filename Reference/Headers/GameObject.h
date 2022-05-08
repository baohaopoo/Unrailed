#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device, const string _ObjectName = "default");
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag) {
		return Find_Component(pComponentTag);
	}
	void Delete_Component(const _tchar* pComponentTag);

	const string& Get_ObjectName(void) const { return m_ObjectName; }

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	string					m_ObjectName;
	size_t					m_iTextureNumber = 0;

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

public:
	const COMPONENTS& Get_Components(void) const { return m_Components; }

protected:
	bool	m_bDead = false;

public:
	bool	Get_Dead(void) const { return m_bDead; }
	void	Set_Dead(bool _bDead) { m_bDead = _bDead; }

public:
	size_t	Get_TextureNumber(void) const { return m_iTextureNumber; }
	void	Set_TextureNumber(size_t _iTextureNumber) { m_iTextureNumber = _iTextureNumber; }

protected:
	OBJECT_TYPE m_eType;
public:
	OBJECT_TYPE Get_Type(void) const { return m_eType; }

protected:
	HRESULT SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);

public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END