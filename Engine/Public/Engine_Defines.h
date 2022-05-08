#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4996)

#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include "Engine_Struct.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"


#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <time.h>
#include <Shlwapi.h>
#include <set>

using namespace std;



/* ¸¯Ã¼Å© */

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW		
#endif 
#endif // _DEBUG

namespace Engine {

#define	Prototype_Compo_Renderer			TEXT("Prototype_Component_Renderer")

#define Prototype_Compo_VIB_Rect			TEXT("Prototype_Component_VIBuffer_Rect")
#define Prototype_Compo_VIB_Cube			TEXT("Prototype_Component_VIBuffer_Cube")
#define Prototype_Compo_VIB_Terrain			TEXT("Prototype_Component_VIBuffer_Terrain")
#define Prototype_Compo_VIB_Object			TEXT("Prototype_Component_VIBuffer_Object")

#define Prototype_Compo_Texture_Default		TEXT("Prototype_Component_Texture_Default")
#define Prototype_Compo_Texture_Cube		TEXT("Prototype_Component_Texture_Cube")
#define Prototype_Compo_Texture_Terrain		TEXT("Prototype_Component_Texture_Terrain")

#define Prototype_Compo_Transform			TEXT("Prototype_Component_Transform")

#define Prototype_Object_Terrain			TEXT("Prototype_GameObject_Terrain")
#define Prototype_Object_Player				TEXT("Prototype_GameObject_Player")
#define Prototype_Object_CubeObject			TEXT("Prototype_GameObject_CubeObject")
#define Prototype_Object_PickingObject		TEXT("Prototype_GameObject_PickingOject")

#define Prototype_Object_Camera_Default		TEXT("Prototype_GameObject_Camera_Default")
#define Prototype_Object_Camera_Observer	TEXT("Prototype_GameObject_Camera_Observer")

#define Layer_Camera						TEXT("Layer_Camera")
#define Layer_Camera_Map					TEXT("Layer_Camera_Map")
#define Layer_Camera_Object					TEXT("Layer_Camera_Object")
#define Layer_Camera_Player					TEXT("Layer_Camera_Player")
#define Layer_Camera_Effect					TEXT("Layer_Camera_Effect")
#define Layer_Player						TEXT("Layer_Player")
#define Layer_Terrain						TEXT("Layer_Terrain")

#define Layer_Map							TEXT("Layer_Map")
#define Layer_Object						TEXT("Layer_Object")

#define Component_Transform					TEXT("Com_Transform")
#define Component_Renderer					TEXT("Com_Renderer")
#define Component_VIBuffer					TEXT("Com_VIBuffer")
#define Component_Texture					TEXT("Com_Texture")

}

using namespace Engine;

