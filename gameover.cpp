//=============================================================================
// ゲームオーバーの描画用『gameover.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define POS_X_BG	0.0f
#define POS_Y_BG	0.0f
#define WIDTH_BG	SCREEN_WIDTH
#define HEIGHT_BG	SCREEN_HEIGHT

#define TEX_BG		0
#define MAX_TEXTURE			1

#define TIMER_GAMEOVER		10*60

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {{L"data/texture/gameover001.png"}};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];

// 初期化処理
HRESULT CGameover::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// 変数初期化
	m_Timer = TIMER_GAMEOVER;

	return hr;
}

// 終了処理
void CGameover::Uninit()
{
	// テクスチャ解放
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

// 更新処理
void CGameover::Update()
{
	// 終了
	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE))
	{
		StartFadeOut(SCENE_TITLE);
		return;
	}

	// タイマー更新
	--m_Timer;
	if (m_Timer <= 0)
	{
		StartFadeOut(SCENE_TITLE);
	}
}


// 描画処理
void CGameover::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
}

//初期化
//HRESULT InitGameOver()
//{
//	HRESULT hr = S_OK;
//	ID3D11Device* pDevice = GetDevice();
//
//	// テクスチャ読込
//	for (int i = 0; i < MAX_TEXTURE; ++i)
//	{
//		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
//		if (FAILED(hr))
//		{
//			return hr;
//		}
//	}
//
//	// 変数初期化
//	g_nTimer = TIMER_GAMEOVER;
//
//	return hr;
//}


//終了
//void UninitGameOver()
//{
//	// テクスチャ解放
//	for (int i = 0; i < MAX_TEXTURE; ++i)
//	{
//		SAFE_RELEASE(g_pTexture[i]);
//	}
//
//}

//更新
//void UpdateGameOver()
//{
//	// 終了
//	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE))
//	{
//		StartFadeOut(SCENE_TITLE);
//		return;
//	}
//
//	// タイマー更新
//	--g_nTimer;
//	if (g_nTimer <= 0)
//	{
//		StartFadeOut(SCENE_TITLE);
//	}
//}


//描画
//void DrawGameOver()
//{
//	ID3D11DeviceContext* pDC = GetDeviceContext();
//	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
//	SetPolygonPos(POS_X_BG, POS_Y_BG);
//	SetPolygonTexture(g_pTexture[TEX_BG]);
//	DrawPolygon(pDC);
//}


