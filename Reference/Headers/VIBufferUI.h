#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL VIBufferUI  final: public CVIBuffer
{
public:
	explicit VIBufferUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit VIBufferUI(const VIBufferUI& rhs);

	virtual ~VIBufferUI() = default;


public:

	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;


public:

	static VIBufferUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END