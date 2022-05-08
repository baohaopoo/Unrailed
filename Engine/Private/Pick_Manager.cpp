
#include "..\Public\Pick_Manager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CPick_Manager)

CPick_Manager::CPick_Manager()
{
}

CPick_Manager::CPick_Manager(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: m_pGraphic_Device(_pGraphic_Device)
{
}

CPick_Manager * CPick_Manager::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	m_pInstance = new CPick_Manager(_pGraphic_Device);

	return m_pInstance;
}

HRESULT CPick_Manager::InitDevice(HWND hWnd, _uint _iWinCX, _uint _iWinCY, _uint _iStartWinPosX, _uint _iStartWinPosY)
{
	m_pInstance = CPick_Manager::GetInstance();

	if (nullptr == m_pInstance)
		return E_FAIL;

	m_hWnd = hWnd;
	
	m_ptResolution.x = _iWinCX;
	m_ptResolution.y = _iWinCY;

	m_ptWindowStartPos.x = _iStartWinPosX;
	m_ptWindowStartPos.y = _iStartWinPosY;

	return S_OK;
}

void CPick_Manager::Tick(void)
{
	// �����쿡�� ���콺 ��ǥ, ������ ����/���� ����, ��������� �����, ����, ��������, ����� ������ 3�� ��ǥ
	// g_iWinCX, g_iWinCY
	GetCursorPos(&m_ptMouse);
	ScreenToClient(m_hWnd, &m_ptMouse);
	

	// ���� �����̽��� ���콺 ��ǥ
	D3DXVECTOR3 vMouseProjection{};
	vMouseProjection.x = 1.f* m_ptMouse.x / (m_ptResolution.x / 2) - 1;
	vMouseProjection.y = -1.f* m_ptMouse.y / (m_ptResolution.y / 2) + 1;
	vMouseProjection.z = 0.f;

	D3DXMATRIX MatrixProjectionInverse{};
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &MatrixProjectionInverse);
	D3DXMatrixInverse(&MatrixProjectionInverse, nullptr, &MatrixProjectionInverse);

	// ���� vMouseProjection�� ��ġ�� �佺���̽��� �ִ�.
	D3DXVec3TransformCoord(&vMouseProjection, &vMouseProjection, &MatrixProjectionInverse);


	// ���� ���� : ī�޶��� ��ġ�� ������ 0.f, 0.f, 0.f�� ��´�.
	D3DXVECTOR3 vCameraPos{ 0.f, 0.f, 0.f };

	D3DXMATRIX MatrixWorld{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &MatrixWorld);
	D3DXMatrixInverse(&MatrixWorld, nullptr, &MatrixWorld);

	// ���� ī�޶��� ��ġ�� ���忡 �ִ� ������Ʈ�� ī�޶� �������� �̵��ϱ� ������ �����Ѵ�.
	D3DXVec3TransformCoord(&vCameraPos, &vCameraPos, &MatrixWorld);

	// ���� ���� ���� �����ش�. �̸��� �򰥷��� �ٲ�.
	D3DXVECTOR3 vRay{ vMouseProjection };

	// ���� ���͸� ī�޶� �������� ���� �̵��Ѵ�. ��, ���� ���̴� ���⺤���̹Ƿ� D3DXVec3TransformNormal�� �����Ѵ�.
	D3DXVec3TransformNormal(&vRay, &vRay, &MatrixWorld);

	// ���� ���� �����̽��� ���� ���Ϳ� ���� ������ �����Ѵ�.
	m_RayDecs.vRayPos = vCameraPos;

	D3DXVec3Normalize(&vRay, &vRay);
	m_RayDecs.vRayDirection = vRay;

}

void CPick_Manager::LateTick(void)
{
	m_lstColPolygon.clear();
}

float CPick_Manager::Pick(D3DXVECTOR3 & _p1, D3DXVECTOR3 & _p2, D3DXVECTOR3 & _p3)
{
	float _fDistance = 0.f;

	if (D3DXIntersectTri(&_p1, &_p2, &_p3, &m_RayDecs.vRayPos, &m_RayDecs.vRayDirection, nullptr, nullptr, &_fDistance))
		return _fDistance;

	return -1;
}

float CPick_Manager::Pick(POLYGON & polygon)
{
	return Pick(polygon.p1, polygon.p2, polygon.p3);
}

// ���콺�� ������ Ư�� ������ �ִ� ��� ������Ʈ���� �ش��ϴ� ��ġ�� pickó���� �ϰԵǰ�, �̷��� �ؼ� �浹�� ��������� list�� Ǫ���ϰ�, 
// �� ���ߴٸ� �̸� �����Ͽ� ���� ����� �������� ã�� �ش� �������� ��ȯ�Ѵ�.
CPick_Manager::POLYGON* CPick_Manager::Picking(_uint _iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	map<const _tchar*, class CLayer*> m_pLayers = pGameInstance->Find_LevelLayers(_iNumLevel);

	for (auto& pLayer : m_pLayers) {
		list<CGameObject*> lstObject = pLayer.second->Get_ObjectList();
		for (auto& Object : lstObject) {
			CheckObjectPicked(Object);
		}
	}

	if (!m_lstColPolygon.empty())
		// ��ŷ�� ������ �� ���� ī�޶� ����� �������� �����. ���� �ش��ϴ� �������� ��ȯ�Ѵ�.
		return &m_lstColPolygon.begin()->second.first;

	return nullptr;
}

CGameObject * CPick_Manager::Picking_Object(_uint _iNumLevel)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	map<const _tchar*, class CLayer*> m_pLayers = pGameInstance->Find_LevelLayers(_iNumLevel);

	for (auto& pLayer : m_pLayers) {
		list<CGameObject*> lstObject = pLayer.second->Get_ObjectList();
		for (auto& Object : lstObject) {
			CheckObjectPicked(Object);
		}
	}

	if (!m_lstColPolygon.empty())
		// ��ŷ�� ������ �� ���� ī�޶� ����� �������� �����. ���� �ش��ϴ� �������� ���� ������Ʈ�� ��ȯ�Ѵ�.
		return m_lstColPolygon.begin()->second.second;

	return nullptr;
}

_float3 CPick_Manager::TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3)
{
	// 1. ���ڷ� ���� �� 3���� ���� �ﰢ������ ���� ���̰� �� ���� ���Ѵ�.
	float fSide0 = 0.f;
	float fSide1 = pow((p1.x - p2.x), 2.f) + pow((p1.y - p2.y), 2.f) + pow((p1.z - p2.z), 2.f);	// �� 1,2
	float fSide2 = pow((p2.x - p3.x), 2.f) + pow((p2.y - p3.y), 2.f) + pow((p2.z - p3.z), 2.f);	// �� 2,3
	float fSide3 = pow((p3.x - p1.x), 2.f) + pow((p3.y - p1.y), 2.f) + pow((p3.z - p1.z), 2.f);	// �� 3,1
																								// �� 1,2�� ���� ���� ���� ��� 
	(fSide1 > fSide2) ? ((fSide1 > fSide3) ? fSide0 = fSide1 : fSide0 = fSide3) : ((fSide2 > fSide3) ? fSide0 = fSide2 : fSide0 = fSide3);
	_float3 fPoint{};

	if (fSide0 == fSide1)
		fPoint = (p1 + p2) / 2.f;
	else if (fSide0 == fSide2)
		fPoint = (p2 + p3) / 2.f;
	else if (fSide0 == fSide3)
		fPoint = (p3 + p1) / 2.f;

	return fPoint;
}

_float3 CPick_Manager::TriBevelMiddle(CPick_Manager::POLYGON & polygon)
{
	return TriBevelMiddle(polygon.p1, polygon.p2, polygon.p3);
}

void CPick_Manager::MatchPrecisionFloat(_float& _fNum)
{
	// 2��� ����
	float _fMul2 = _fNum * 2;

	int _iNum = abs((int)_fMul2);

	float _fCal = (_float)abs(_fMul2) - _iNum;
	if (_fCal > 0)
		_fNum = 0.5f * _iNum;
	else if (_fCal < 0)
		_fNum = -0.5f * _iNum;
	else
		;
}

void CPick_Manager::MatchPrecisionPoint(_float3& _fPoint)
{
	MatchPrecisionFloat(_fPoint.x);
	MatchPrecisionFloat(_fPoint.y);
	MatchPrecisionFloat(_fPoint.z);
}

void CPick_Manager::MatchPrecisionPolygon(POLYGON& _polygon)
{
	MatchPrecisionPoint(_polygon.p1);
	MatchPrecisionPoint(_polygon.p2);
	MatchPrecisionPoint(_polygon.p3);
}

bool CPick_Manager::CheckObjectPicked(CGameObject* pObject)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CVIBuffer* _pVIBuffer = dynamic_cast<CVIBuffer*>(pObject->Get_Component(Component_VIBuffer));

	// ���� VIBuffer�� ������ ������Ʈ�� ��� ���� �ʴٸ�
	if (nullptr == _pVIBuffer)
		// ���ٰ� ��ȯ�Ѵ�.
		return false;

	// VIBuffer Ÿ�Կ� ���� �ٸ��� ������� �������� picking ó���Ѵ�.
	map<_float, pair<POLYGON, CGameObject*>> lstColPolygon;
	POLYGON polygon{};

	// �⺻������ �����̴� ����̶�� �����Ӱ� ���õ� ������Ʈ�� �����Ѵ�.
	CTransform* pTransform = dynamic_cast<CTransform*>(pObject->Get_Component(Component_Transform));
	const CTransform::TRANSFORMDESC& TransformDesc = pTransform->Get_TransformState();

	// Ÿ���� ť����
	if (VIB_CUBE == _pVIBuffer->Get_Type()) {

		// ���ؽ� ������ ���´�.
		VTXCUBETEX* pVertices = (VTXCUBETEX*)_pVIBuffer->Get_Vertices();
		FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
		_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
		_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

		// �ش� VIB�� ��� �������� ������ŭ ���ư��� ���ȴ��� ���θ� Ȯ���Ѵ�.
		for (int i = 0; i < iNumPrimitives; ++i) {
			polygon.p1 = pVertices[pIndicies[i]._0].vPosition;
			polygon.p2 = pVertices[pIndicies[i]._1].vPosition;
			polygon.p3 = pVertices[pIndicies[i]._2].vPosition;

			D3DXVec3TransformCoord(&polygon.p1, &polygon.p1, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
			D3DXVec3TransformCoord(&polygon.p2, &polygon.p2, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
			D3DXVec3TransformCoord(&polygon.p3, &polygon.p3, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));

			//MatchPrecisionPolygon(polygon);
			_float fDist = pGameInstance->Pick(polygon.p1, polygon.p2, polygon.p3);

			// ���� ��ŷ�� �����ߴٸ�
			if (-1 != fDist) {
				//MatchPrecisionPolygon(polygon);
				// ���� ������ ��ŷ�� ���������� �Ÿ��� ��ŷ�� �������� ������ Ǫ���Ѵ�.
				// m_lstColPolygon[fDist] = polygon;
				lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
			}
		}
	}
	// ������Ʈ�� �������� ���
	else if (VIB_OBJECT == _pVIBuffer->Get_Type()) {
		list<pair<CVIBuffer*, size_t>> lstVIBuffer = ((CVIBuffer_Object*)_pVIBuffer)->Get_lstVIBuffer();

		for (auto& VIBuffer : lstVIBuffer) {
			CVIBuffer* _pVIBuffer = VIBuffer.first;

			// ���ؽ� ������ ���´�.
			VTXCUBETEX* pVertices = (VTXCUBETEX*)_pVIBuffer->Get_Vertices();
			FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
			_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
			_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

			// �ش� VIB�� ��� �������� ������ŭ ���ư��� ���ȴ��� ���θ� Ȯ���Ѵ�.
			for (int i = 0; i < iNumPrimitives; ++i) {
				polygon.p1 = pVertices[pIndicies[i]._0].vPosition;
				polygon.p2 = pVertices[pIndicies[i]._1].vPosition;
				polygon.p3 = pVertices[pIndicies[i]._2].vPosition;

				D3DXVec3TransformCoord(&polygon.p1, &polygon.p1, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
				D3DXVec3TransformCoord(&polygon.p2, &polygon.p2, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
				D3DXVec3TransformCoord(&polygon.p3, &polygon.p3, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));

				if (polygon.p1.x > 1000 || polygon.p1.x < -1000 || polygon.p1.y > 1000 || polygon.p1.y < -1000 || polygon.p1.z > 1000 || polygon.p1.z < -1000 ||
					polygon.p2.x > 1000 || polygon.p2.x < -1000 || polygon.p2.y > 1000 || polygon.p2.y < -1000 || polygon.p2.z > 1000 || polygon.p2.z < -1000 ||
					polygon.p3.x > 1000 || polygon.p3.x < -1000 || polygon.p3.y > 1000 || polygon.p3.y < -1000 || polygon.p3.z > 1000 || polygon.p3.z < -1000)
				{
					int a = 10;
				}

				//MatchPrecisionPolygon(polygon);
				_float fDist = pGameInstance->Pick(polygon.p1, polygon.p2, polygon.p3);

				// ���� ��ŷ�� �����ߴٸ�
				if (-1 != fDist) {
					//MatchPrecisionPolygon(polygon);
					// ���� ������ ��ŷ�� ���������� �Ÿ��� ��ŷ�� �������� ������ Ǫ���Ѵ�.
					// m_lstColPolygon[fDist] = polygon;
					lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
				}
			}
		}
	}
	// ť��� ������Ʈ�� ������ ���������
	else {

		// ���ؽ� ������ ���´�.
		VTXTEX* pVertices = (VTXTEX*)_pVIBuffer->Get_Vertices();
		FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
		_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
		_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

		// �ش� VIB�� ��� �������� ������ŭ ���ư��� ���ȴ��� ���θ� Ȯ���Ѵ�.
		for (int i = 0; i < iNumPrimitives; ++i) {
			polygon.p1 = pVertices[pIndicies[i]._0].vPosition;
			polygon.p2 = pVertices[pIndicies[i]._1].vPosition;
			polygon.p3 = pVertices[pIndicies[i]._2].vPosition;

			//MatchPrecisionPolygon(polygon);
			_float fDist = pGameInstance->Pick(polygon.p1, polygon.p2, polygon.p3);

			// ���� ��ŷ�� �����ߴٸ�
			if (-1 != fDist) {
				// ���� ������ ��ŷ�� ���������� �Ÿ��� ��ŷ�� �������� ������ Ǫ���Ѵ�.
				// m_lstColPolygon[fDist] = polygon;
				lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
			}
		}
	}

	// ��ŷ�� ���� �����ϴٸ�
	if (!lstColPolygon.empty()) {
		// ���� ����� �Ÿ��� ���� �������� ������ ������
		m_lstColPolygon.emplace(*lstColPolygon.begin());

		// �������� ���ش�.
		lstColPolygon.clear();
		return true;
	}

	// ��ŷ�� ���� �������� �ʴ´ٸ� ���ٰ� �Ѵ�.
	return false;
}

CGameObject * CPick_Manager::Picking_Layer(_uint _iNumLevel, const _tchar * _tagLayer)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// ���ڷ� ���� ������ �ִ� ���̾ ã�´�.
	CLayer*		pLayer = pGameInstance->Find_Layer(_iNumLevel, _tagLayer);

	// ���̾ ���ٸ� ���ٰ� ��ȯ�Ѵ�.
	if (nullptr == pLayer)
		return nullptr;

	// ���̾ ã��, ã�� ���̾�� ������Ʈ ����Ʈ�� ������ �´�. 
	list<CGameObject*> ObjectList = pLayer->Get_ObjectList();

	// ������Ʈ�� ������ �ִ� VIBuffer�� Polygon�� ���� Ȯ�����ְ�
	for (auto& Object : ObjectList) {
		CheckObjectPicked(Object);
	}

	// ���� �浹 ó���� �� �������� �ִٸ�, �ش� �������� ������ �ִ� ������Ʈ�� ������ ��ȯ�Ѵ�.
	if (!m_lstColPolygon.empty()) {
		return m_lstColPolygon.begin()->second.second;
	}

	// �浹ó���� �� �������� ���ٸ� ���ٰ� ��ȯ�Ѵ�.
	return nullptr;
}

void CPick_Manager::Free()
{

}