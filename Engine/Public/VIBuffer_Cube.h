#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct_Prototype(D3DXMATRIX& LocalMatrix);
	// virtual HRESULT NativeConstruct_Prototype(const _float3& vFocus);
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;

public:
	virtual HRESULT SaveVIBuffer(const string& _strFilePath) override;

public:
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, D3DXMATRIX& LocalMatrix);
	//static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _float3& vFocus);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END