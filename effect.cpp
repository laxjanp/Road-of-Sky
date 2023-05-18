//=============================================================================
// 爆発エフェクト用ソースコード『effect.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "effect.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_EFFECT_TEXTURE L"data/texture/effect.png"
#define POS_X_EFFECT 0.0f
#define POS_Y_EFFECT 0.0f
#define EFFECT_WIDTH 64
#define EFFECT_HEIGHT 64

#define MAX_EFFECT 100		//最大数

#define COUNT_X_EFFECT 7	//横枠数
#define COUNT_Y_EFFECT 2	//縦枠数

//*****************************************************************************
// 構造体定義(struct)
//*****************************************************************************
struct EFFECT
{
	XMFLOAT2 vPos;			// 位置
	int nFrame;				// 枠No
	int nCount;				// フレーム数
};

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static EFFECT g_effect[MAX_EFFECT];		// エフェクト情報
static int g_nEffect;					// 使用中エフェクト数

// 初期化
HRESULT InitEffect()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_EFFECT_TEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// 初期化
	g_nEffect = 0;
	return hr;
}

// 終了処理
void UninitEffect()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

// 更新
void UpdateEffect()
{
	EFFECT* pEffect;
	for (int i = g_nEffect - 1; i >= 0; --i)
	{
		pEffect = &g_effect[i];
		
		// アニメーション更新
		pEffect->nCount--;
		if (pEffect->nCount <= 0)
		{
			pEffect->nFrame++;
			if (pEffect->nFrame > 13)
			{
				// 消去
				--g_nEffect;
				if (g_nEffect > 0)
				{
					*pEffect = g_effect[g_nEffect];
				}
				continue;
			}
			pEffect->nCount = 3;
		}
	}
}

// 描画
void DrawEffect()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(EFFECT_WIDTH, EFFECT_HEIGHT);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / COUNT_X_EFFECT, 1.0f / COUNT_Y_EFFECT);
	EFFECT* pEffect = g_effect;
	for (int i = 0; i < g_nEffect; ++i, ++pEffect)
	{
		SetPolygonUV((pEffect->nFrame % COUNT_X_EFFECT) / (float)COUNT_X_EFFECT,
			(pEffect->nFrame / COUNT_X_EFFECT) / (float)COUNT_Y_EFFECT);
		SetPolygonPos(pEffect->vPos.x, pEffect->vPos.y);
		DrawPolygon(pDC);
	}
	// 元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

}

//========================================
//StartEffect関数
//内容:
//戻り値:
//========================================
int StartEffect(XMFLOAT2 vPos)
{
	if (g_nEffect < MAX_EFFECT)
	{
		EFFECT* pEffect = &g_effect[g_nEffect];
		pEffect->vPos = vPos;
		pEffect->nFrame = 0;
		pEffect->nCount = 3;
		++g_nEffect;
		return g_nEffect;
	}

	return 0;
}