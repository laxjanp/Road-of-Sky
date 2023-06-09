//=============================================================================
// サンプル処理 [main.h]
// 制作者:松本恵太
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードガード(#include)
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "resource.h"

// 本来はヘッダに書かない方が良い
using namespace DirectX;

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

//*****************************************************************************
// 列挙体定義(#define)
//*****************************************************************************
enum EBlendState 
{
	BS_NONE = 0,	// 半透明合成無し
	BS_ALPHABLEND,	// 半透明合成
	BS_ADDITIVE,	// 加算合成
	BS_SUBTRACTION, // 減算合成
	MAX_BLENDSTATE
};

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

//*****************************************************************************
// プロトタイプ宣言(関数呼び出し)
//*****************************************************************************
HWND GetMainWnd();
HINSTANCE GetInstance();
ID3D11Device* GetDevice();
ID3D11DeviceContext* GetDeviceContext();
void SetBlendState(int nBlendState);
