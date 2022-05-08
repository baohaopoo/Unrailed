#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Cube;

END

BEGIN(Client)
class CEffect_TrainSmoke final : public CGameObject
{
public:
	enum SmokeType
	{
		ST_TRAIN, ST_DYNAMITE, ST_DASH, ST_METEO, ST_END
	};
public:
	typedef struct _tagTrainSmokeDesc
	{
		CTransform::TRANSFORMDESC		TransDesc;
		SmokeType eSmoke = ST_END;
		_uint		iTextureNumber;


	}TRAINSMOKEDESC;

public:
	explicit CEffect_TrainSmoke(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_TrainSmoke(const CEffect_TrainSmoke & rhs);
	virtual ~CEffect_TrainSmoke() = default;

public:
	virtual	HRESULT			NativeConstruct_Prototype();
	virtual	HRESULT			NativeConstruct(void* pArg);
	virtual	void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();


private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

public:
	void ChangeColor(_uint iTextureNumber);
	const _float Get_Time(void) const { return fTime; }

private:
	HRESULT						SetUp_Components(void * pArg);

private:
	_float fTime = 0.f;
	_float3 fPos;
	_float3	vUp;

public:
	static	CEffect_TrainSmoke*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;


};

END