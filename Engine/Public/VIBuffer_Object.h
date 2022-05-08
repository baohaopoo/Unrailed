#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Object final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Object(const CVIBuffer_Object& rhs);
	virtual ~CVIBuffer_Object() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;

public:
	// CVIBuffer_Object는 여러 오브젝트들이 합쳐져서 만들어진 객체이므로,
	// 이를 렌더하기 위해선 각 오브젝트들을 모두 각각 렌더해야한다.
	virtual HRESULT Render() override;

public:
	static CVIBuffer_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

public:
	// 1인자에 버퍼, 2인자에 텍스쳐 넘버가 들어간다.
	const list<pair<CVIBuffer*, size_t>>& Get_lstVIBuffer(void) const { return m_lstVIBuffer; }

private:
	list<pair<CVIBuffer*, size_t>> m_lstVIBuffer;
};

END