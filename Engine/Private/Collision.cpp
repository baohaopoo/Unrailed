#include "..\Public\Collision.h"
#include "GameObject.h"

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CCollision::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCollision::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CCollision::Add_CollisionList(_uint _iNumLevel, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CCollision::Collision_GameObjects()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollision::Render_Priority()
{
	HRESULT hr = 0;

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* CCW : Counter Clock Wise : 반시계방향ㄹ으 ㅋ럴링한다. == 시계방향(전면)만 보여준다.  */
	/* CW : Clock Wise : 시계방향ㄹ으 ㅋ럴링한다. == 반시계방향(뒷면)만 보여준다.  */

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& pGameObject : m_RenderList[GROUP_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_PRIORITY].clear();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CCollision::Render_NonBlend()
{
	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_NONBLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_NONBLEND].clear();

	return S_OK;
}

HRESULT CCollision::Render_Blend()
{
	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CCollision::Render_UI()
{
	HRESULT hr = 0;

	for (auto& pGameObject : m_RenderList[GROUP_UI])
	{
		if (nullptr != pGameObject)
		{
			hr = pGameObject->Render();
			Safe_Release(pGameObject);
		}

		if (FAILED(hr))
			return E_FAIL;
	}

	m_RenderList[GROUP_UI].clear();

	return S_OK;
}

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollision*	pInstance = new CCollision(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCollision"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CCollision::Free()
{
	__super::Free();

	for (_uint i = 0; i < GROUP_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

}


