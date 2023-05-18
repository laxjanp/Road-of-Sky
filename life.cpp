//=============================================================================
// ライフの表示専用コード『life.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "life.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_FRAMELIFE L"data/texture/frame_life.png"
#define WIDTH_FRAMELIFE 290.0f
#define HEIGHT_FRAMELIFE 80.0f
#define POS_X_FRAMELIFE ((SCREEN_WIDTH-WIDTH_FRAMELIFE)*-0.5f)
#define POS_Y_FRAMELIFE ((SCREEN_HEIGHT-HEIGHT_FRAMELIFE)*0.5f)

#define PATH_TEXTURE_LIFE L"data/texture/player001.png"
#define MAX_LIFE 6
#define SIZE_X_LIFE 40.0f
#define SIZE_Y_LIFE 40.0f
#define POS_X_LIFE (POS_X_FRAMELIFE-SIZE_X_LIFE*(MAX_LIFE*0.5f-0.5f))
#define POS_Y_LIFE (POS_Y_FRAMELIFE-6.0f)

#define TEX_FRAME 0
#define TEX_LIFE 1
#define MAX_TEXTURE 2

#define START_LIFE 3

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nLife;


// 初期化
HRESULT InitLife(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	LPCWSTR pszTexFName[] = { PATH_TEXTURE_FRAMELIFE , PATH_TEXTURE_LIFE };
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		hr = CreateTextureFromFile(pDevice, pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	
	// 変数初期化
	g_nLife = START_LIFE;
	
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife()
{
	// テクスチャ解放
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		SAFE_RELEASE(g_pTexture[i]);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// テクスチャと同じ色に設定
	SetPolygonColor(0.929412f, 0.152941f, 0.564706f);
	
	// ライフ枠表示
	SetPolygonSize(WIDTH_FRAMELIFE, HEIGHT_FRAMELIFE);
	SetPolygonPos(POS_X_FRAMELIFE + 350, POS_Y_FRAMELIFE);
	SetPolygonTexture(g_pTexture[TEX_FRAME]);
	DrawPolygon(pDC);
	
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	
	// ライフ表示
	SetPolygonSize(SIZE_X_LIFE, SIZE_Y_LIFE);
	SetPolygonTexture(g_pTexture[TEX_LIFE]);
	for(int i = 0; i < g_nLife; ++i) 
	{
		SetPolygonPos(POS_X_LIFE + i * SIZE_X_LIFE + 350, POS_Y_LIFE);
		DrawPolygon(pDC);
	}
	

}

//=============================================================================
//AddLife関数
//内容:ライフ表示加算処理
//引数:ライフを加算させるnLife
//戻り値:なし
//=============================================================================

void AddLife(int nLife)
{
	g_nLife += nLife;
	if (g_nLife > MAX_LIFE)
	{
		g_nLife = MAX_LIFE;
	}
}


//=============================================================================
//DelLife関数
//内容:ライフ減算処理
//引数:ライの減算させるnLife
//戻り値:なし
//=============================================================================
void DelLife(int nLife)
{
	g_nLife -= nLife;
	if (g_nLife < 0)
	{
		g_nLife = 0;
	}
}

//=============================================================================
//GetLife関数
//内容:ライフの取得処理
//引数:なし
//戻り値:スコアを取得するためのg_nLife
//=============================================================================
int GetLife() 
{
	return g_nLife;
}