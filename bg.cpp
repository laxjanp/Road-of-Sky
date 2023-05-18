//=============================================================================
// 背景用ソースコード『bg.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "bg.h"
#include "polygon.h"
#include "Texture.h"


//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_BG		L"data/texture/sky_002.jpg"
#define POS_X_BG	0.0f
#define POS_Y_BG	0.0f
#define WIDTH_BG	SCREEN_WIDTH
#define HEIGHT_BG	SCREEN_HEIGHT
#define MAX_BG 10

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static XMFLOAT2	g_vPos;		// 位置
static int g_nDir;
static int g_nBg;

//初期化
HRESULT InitBG()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_BG, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//変数の初期化
	g_vPos.x = POS_X_BG;
	g_vPos.y = POS_Y_BG;
	
	return hr;
}


//終了
void UninitBG()
{
	//テクスチャの解放
	SAFE_RELEASE(g_pTexture);
}


//更新
void UpdateBG()
{
	//何もしない
	
}


//描画
void DrawBG()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);	//ポリゴンのサイズ
	SetPolygonPos(g_vPos.x, g_vPos.y);		//ポリゴンの座標
	SetPolygonTexture(g_pTexture);			//ポリゴンの背景
	SetPolygonColor(1.0f, 1.0f, 1.0f);		//色
	DrawPolygon(pDC);

}


