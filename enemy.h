#pragma once
//=============================================================================
// wi`ๆwenemy.hx
// ง์า:ผ{bพ
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// vg^Cv้พ
//*****************************************************************************
HRESULT InitEnemy(void);	//๚ป
void UninitEnemy(void);		//Cน
void UpdateEnemy(void);		//XV
void DrawEnemy(void);		//`ๆ
int CollisionEnemy(XMFLOAT2 vCenter, XMFLOAT2 vBBox, float fDamage);//ฝ่(ีห)ป่