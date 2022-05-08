
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
	// 윈도우에서 마우스 좌표, 윈도우 가로/세로 길이, 투영행렬의 역행렬, 레이, 레이포스, 대상의 폴리곤 3개 좌표
	// g_iWinCX, g_iWinCY
	GetCursorPos(&m_ptMouse);
	ScreenToClient(m_hWnd, &m_ptMouse);
	

	// 투영 스페이스의 마우스 좌표
	D3DXVECTOR3 vMouseProjection{};
	vMouseProjection.x = 1.f* m_ptMouse.x / (m_ptResolution.x / 2) - 1;
	vMouseProjection.y = -1.f* m_ptMouse.y / (m_ptResolution.y / 2) + 1;
	vMouseProjection.z = 0.f;

	D3DXMATRIX MatrixProjectionInverse{};
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &MatrixProjectionInverse);
	D3DXMatrixInverse(&MatrixProjectionInverse, nullptr, &MatrixProjectionInverse);

	// 현재 vMouseProjection의 위치는 뷰스페이스에 있다.
	D3DXVec3TransformCoord(&vMouseProjection, &vMouseProjection, &MatrixProjectionInverse);


	// 레이 포스 : 카메라의 위치를 월드의 0.f, 0.f, 0.f로 잡는다.
	D3DXVECTOR3 vCameraPos{ 0.f, 0.f, 0.f };

	D3DXMATRIX MatrixWorld{};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &MatrixWorld);
	D3DXMatrixInverse(&MatrixWorld, nullptr, &MatrixWorld);

	// 현재 카메라의 위치는 월드에 있던 오브젝트를 카메라를 원점으로 이동하기 이전에 존재한다.
	D3DXVec3TransformCoord(&vCameraPos, &vCameraPos, &MatrixWorld);

	// 레이 벡터 또한 구해준다. 이름이 헷갈려서 바꿈.
	D3DXVECTOR3 vRay{ vMouseProjection };

	// 레이 벡터를 카메라 시점으로 같이 이동한다. 단, 현재 레이는 방향벡터이므로 D3DXVec3TransformNormal로 연산한다.
	D3DXVec3TransformNormal(&vRay, &vRay, &MatrixWorld);

	// 이제 월드 스페이스에 레이 벡터와 레이 포스가 존재한다.
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

// 마우스를 누르면 특정 레벨에 있는 모든 오브젝트들이 해당하는 위치와 pick처리를 하게되고, 이렇게 해서 충돌된 폴리곤들을 list에 푸쉬하고, 
// 다 구했다면 이를 정렬하여 가장 가까운 폴리곤을 찾아 해당 폴리곤을 반환한다.
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
		// 피킹된 폴리곤 중 가장 카메라에 가까운 폴리곤을 얻었다. 이제 해당하는 폴리곤을 반환한다.
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
		// 피킹된 폴리곤 중 가장 카메라에 가까운 폴리곤을 얻었다. 이제 해당하는 폴리곤을 가진 오브젝트를 반환한다.
		return m_lstColPolygon.begin()->second.second;

	return nullptr;
}

_float3 CPick_Manager::TriBevelMiddle(_float3 & p1, const _float3 & p2, const _float3 & p3)
{
	// 1. 인자로 받은 점 3개로 만든 삼각형에서 가장 길이가 긴 변을 구한다.
	float fSide0 = 0.f;
	float fSide1 = pow((p1.x - p2.x), 2.f) + pow((p1.y - p2.y), 2.f) + pow((p1.z - p2.z), 2.f);	// 점 1,2
	float fSide2 = pow((p2.x - p3.x), 2.f) + pow((p2.y - p3.y), 2.f) + pow((p2.z - p3.z), 2.f);	// 점 2,3
	float fSide3 = pow((p3.x - p1.x), 2.f) + pow((p3.y - p1.y), 2.f) + pow((p3.z - p1.z), 2.f);	// 점 3,1
																								// 점 1,2로 만든 변이 가장 길면 
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
	// 2배곱 변수
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

	// 만약 VIBuffer의 정보를 오브젝트가 들고 있지 않다면
	if (nullptr == _pVIBuffer)
		// 없다고 반환한다.
		return false;

	// VIBuffer 타입에 따라 다르게 만들어진 폴리곤을 picking 처리한다.
	map<_float, pair<POLYGON, CGameObject*>> lstColPolygon;
	POLYGON polygon{};

	// 기본적으로 움직이는 녀셕이라면 움직임과 관련된 컴포넌트가 존재한다.
	CTransform* pTransform = dynamic_cast<CTransform*>(pObject->Get_Component(Component_Transform));
	const CTransform::TRANSFORMDESC& TransformDesc = pTransform->Get_TransformState();

	// 타입이 큐브라면
	if (VIB_CUBE == _pVIBuffer->Get_Type()) {

		// 버텍스 정보를 들고온다.
		VTXCUBETEX* pVertices = (VTXCUBETEX*)_pVIBuffer->Get_Vertices();
		FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
		_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
		_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

		// 해당 VIB의 모든 폴리곤의 갯수만큼 돌아가며 눌렸는지 여부를 확인한다.
		for (int i = 0; i < iNumPrimitives; ++i) {
			polygon.p1 = pVertices[pIndicies[i]._0].vPosition;
			polygon.p2 = pVertices[pIndicies[i]._1].vPosition;
			polygon.p3 = pVertices[pIndicies[i]._2].vPosition;

			D3DXVec3TransformCoord(&polygon.p1, &polygon.p1, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
			D3DXVec3TransformCoord(&polygon.p2, &polygon.p2, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));
			D3DXVec3TransformCoord(&polygon.p3, &polygon.p3, &(TransformDesc.m_LocalMatrix * pTransform->Get_WorldMatrix()));

			//MatchPrecisionPolygon(polygon);
			_float fDist = pGameInstance->Pick(polygon.p1, polygon.p2, polygon.p3);

			// 만약 피킹이 성공했다면
			if (-1 != fDist) {
				//MatchPrecisionPolygon(polygon);
				// 레이 포스와 피킹된 지점까지의 거리와 피킹된 폴리곤의 정보를 푸쉬한다.
				// m_lstColPolygon[fDist] = polygon;
				lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
			}
		}
	}
	// 오브젝트를 선택했을 경우
	else if (VIB_OBJECT == _pVIBuffer->Get_Type()) {
		list<pair<CVIBuffer*, size_t>> lstVIBuffer = ((CVIBuffer_Object*)_pVIBuffer)->Get_lstVIBuffer();

		for (auto& VIBuffer : lstVIBuffer) {
			CVIBuffer* _pVIBuffer = VIBuffer.first;

			// 버텍스 정보를 들고온다.
			VTXCUBETEX* pVertices = (VTXCUBETEX*)_pVIBuffer->Get_Vertices();
			FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
			_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
			_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

			// 해당 VIB의 모든 폴리곤의 갯수만큼 돌아가며 눌렸는지 여부를 확인한다.
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

				// 만약 피킹이 성공했다면
				if (-1 != fDist) {
					//MatchPrecisionPolygon(polygon);
					// 레이 포스와 피킹된 지점까지의 거리와 피킹된 폴리곤의 정보를 푸쉬한다.
					// m_lstColPolygon[fDist] = polygon;
					lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
				}
			}
		}
	}
	// 큐브와 오브젝트를 제외한 나머지라면
	else {

		// 버텍스 정보를 들고온다.
		VTXTEX* pVertices = (VTXTEX*)_pVIBuffer->Get_Vertices();
		FACELISTINDICES16* pIndicies = (FACELISTINDICES16*)_pVIBuffer->Get_Indices();
		_uint	iNumVertices = _pVIBuffer->Get_NumVertices();
		_uint	iNumPrimitives = _pVIBuffer->Get_NumPrimitives();

		// 해당 VIB의 모든 폴리곤의 갯수만큼 돌아가며 눌렸는지 여부를 확인한다.
		for (int i = 0; i < iNumPrimitives; ++i) {
			polygon.p1 = pVertices[pIndicies[i]._0].vPosition;
			polygon.p2 = pVertices[pIndicies[i]._1].vPosition;
			polygon.p3 = pVertices[pIndicies[i]._2].vPosition;

			//MatchPrecisionPolygon(polygon);
			_float fDist = pGameInstance->Pick(polygon.p1, polygon.p2, polygon.p3);

			// 만약 피킹이 성공했다면
			if (-1 != fDist) {
				// 레이 포스와 피킹된 지점까지의 거리와 피킹된 폴리곤의 정보를 푸쉬한다.
				// m_lstColPolygon[fDist] = polygon;
				lstColPolygon.emplace(fDist, make_pair(polygon, pObject));
			}
		}
	}

	// 피킹된 값이 존재하다면
	if (!lstColPolygon.empty()) {
		// 가장 가까운 거리를 가진 폴리곤의 정보를 보내고
		m_lstColPolygon.emplace(*lstColPolygon.begin());

		// 나머지는 없앤다.
		lstColPolygon.clear();
		return true;
	}

	// 피킹된 값이 존재하지 않는다면 없다고 한다.
	return false;
}

CGameObject * CPick_Manager::Picking_Layer(_uint _iNumLevel, const _tchar * _tagLayer)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	// 인자로 받은 레벨에 있는 레이어를 찾는다.
	CLayer*		pLayer = pGameInstance->Find_Layer(_iNumLevel, _tagLayer);

	// 레이어가 없다면 없다고 반환한다.
	if (nullptr == pLayer)
		return nullptr;

	// 레이어를 찾고, 찾은 레이어에서 오브젝트 리스트를 가지고 온다. 
	list<CGameObject*> ObjectList = pLayer->Get_ObjectList();

	// 오브젝트가 가지고 있는 VIBuffer의 Polygon을 전부 확인해주고
	for (auto& Object : ObjectList) {
		CheckObjectPicked(Object);
	}

	// 만약 충돌 처리가 된 폴리곤이 있다면, 해당 폴리곤을 가지고 있는 오브젝트의 정보를 반환한다.
	if (!m_lstColPolygon.empty()) {
		return m_lstColPolygon.begin()->second.second;
	}

	// 충돌처리가 된 폴리곤이 없다면 없다고 반환한다.
	return nullptr;
}

void CPick_Manager::Free()
{

}