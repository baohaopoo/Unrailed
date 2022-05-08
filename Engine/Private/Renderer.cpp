#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform));

	if (nullptr == pTransform)
		return S_OK;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (pGameInstance->Check_InOut(pTransform->Get_WorldMatrix())) {
		m_RenderList[eGroup].push_back(pGameObject);

		Safe_AddRef(pGameObject);
	}

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_UI(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform));

	if (nullptr == pTransform)
		return S_OK;

	m_RenderList[GROUP_UI].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_NonCulling(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CTransform* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(Component_Transform));

	if (nullptr == pTransform)
		return S_OK;

	m_RenderList[eGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render_GameObjects()
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

HRESULT CRenderer::Render_Priority()
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

HRESULT CRenderer::Render_NonBlend()
{
	HRESULT hr = 0;
	
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_RenderList[GROUP_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
//	HRESULT hr = 0;
//
//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
//	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//	m_RenderList[GROUP_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
//	{
//		return ((CBlendObject*)pSour)->Get_CamDistance() > ((CBlendObject*)pDest)->Get_CamDistance();
//	});
//
//	for (auto& pGameObject : m_RenderList[GROUP_ALPHABLEND])
//	{
//		if (nullptr != pGameObject)
//		{
//			hr = pGameObject->Render();
//			Safe_Release(pGameObject);
//		}
//
//		if (FAILED(hr))
//			return E_FAIL;
//	}
//
//	m_RenderList[GROUP_ALPHABLEND].clear();
//
//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//	return S_OK;


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

HRESULT CRenderer::Render_UI()
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

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < GROUP_END; ++i)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}

}


