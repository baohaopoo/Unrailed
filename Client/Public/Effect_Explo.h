#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include <random>
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;

END

BEGIN(Client)

class CEffect_Explo final : public CGameObject
{
public:
	enum ExploType
	{
		ET_RESOURCE, ET_DYNAMITE, ET_METEO, ET_END
	};
	typedef struct _tagExploDesc
	{
		CTransform::TRANSFORMDESC	TransDesc;
		_uint		iTextureNumber;
		ExploType eExplo = ET_END;
	}EXPLODESC;
private:
	explicit CEffect_Explo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Explo(const CEffect_Explo& rhs);
	virtual ~CEffect_Explo() = default;
public:
	virtual	HRESULT			NativeConstruct_Prototype();
	virtual	HRESULT			NativeConstruct(void* pArg);
	virtual	void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
private:
	random_device rd;
	std::uniform_int_distribution <int> uid{ -20,20 };
	std::default_random_engine dre;
	std::mt19937 md;
private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	D3DXVECTOR3 m_vPosition;
	_float fv_X, fv_Y, fv_Z;    // X������ �ӵ�	// Y������ �ӵ�	// Z������ �ӵ�
	_float fG;					// Y�������� �߷� ���ӵ�!!  9.8f�� �ƴ�!!
	_float fEndTime;			// �������� ���� �ð�
	_float fMaxHeight = 4.f;	// �ִ� ����
	_float fHeight;				// �ִ� ���� - ���� ����
	_float fEndHeight;			// ���� ���� ���� - ���� ���� ����
	_float fMaxTime = 0.6f;		// �ִ� ���̱��� ���� �ð�
	_float fTime = 0.f;
	_float3 fPos;


private:
	HRESULT					SetUp_Components(void * pArg);

public:
	static	CEffect_Explo*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void			Free() override;

};

END