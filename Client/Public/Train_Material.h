#pragma once

#include "Train.h"

BEGIN(Client)

class CTrain_Material final : public CTrain {
private:
	explicit CTrain_Material(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTrain_Material(const CTrain_Material& rhs);
	virtual ~CTrain_Material() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_TrainComponents(void* pArg) override;

public:
	static CTrain_Material* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	// ===================================================================
	// 추가 변수 및 함수
	// ===================================================================
	list<pair<string, CGameObject*>> m_list;

private:
	int			m_iLife;
	int			materialNum = 0;

private:
	vector<CGameObject*> m_Woodvec;
	vector<CGameObject*> m_Ironvec;

public:
	//여기서 rail생성한다. num값을 받아서, 
	void GetMaterial();
	void PushMaterial(string name,CGameObject* pObject);

public:
	void TrainState(bool wood, bool iron);
	void both();
};

END