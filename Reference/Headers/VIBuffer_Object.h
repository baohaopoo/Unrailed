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
	// CVIBuffer_Object�� ���� ������Ʈ���� �������� ������� ��ü�̹Ƿ�,
	// �̸� �����ϱ� ���ؼ� �� ������Ʈ���� ��� ���� �����ؾ��Ѵ�.
	virtual HRESULT Render() override;

public:
	static CVIBuffer_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

public:
	// 1���ڿ� ����, 2���ڿ� �ؽ��� �ѹ��� ����.
	const list<pair<CVIBuffer*, size_t>>& Get_lstVIBuffer(void) const { return m_lstVIBuffer; }

private:
	list<pair<CVIBuffer*, size_t>> m_lstVIBuffer;
};

END