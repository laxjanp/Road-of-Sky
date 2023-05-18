//=============================================================================
// フェードイン、フェードアウト用『fade.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "fade.h"			//フェードイン、アウトが必要のため用意
#include "polygon.h"		//ポリゴンの表示が必要のため用意
#include "Sound.h"			//BGMを入れるために用意

//*****************************************************************************
//マクロ定義(#define)
//*****************************************************************************
#define FADE_RATE 0.02f;	//フェードインの刻
#define FADE_RATE2 0.02f;	//フェードアウトの刻

//*****************************************************************************
//グローバル変数(g_)
//*****************************************************************************
static float g_fRed = 0.0f;				//フェードアウトの色(赤)
static float g_fGreen = 0.0f;			//フェードアウトの色(緑)
static float g_fBlue = 0.0f;			//フェードアウトの色(青)
static float g_fAlpha = 1.0f;			//フェードアウトの色(透明部分)
static EFade g_eFade = FADE_IN;			//今の状態
static EScene g_eNext = SCENE_TITLE;	//次のシーンへ

// 初期化
HRESULT InitFade() 
{
	g_fAlpha = 1.0f;
	g_eFade = FADE_IN;
	g_eNext = SCENE_TITLE;
	return S_OK;
}

// 終了処理
void UninitFade() 
{
	g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}

// 更新
void UpdateFade() 
{
	switch (g_eFade) 
	{
	case FADE_NONE:
		break;
	
	case FADE_OUT:
		g_fAlpha += FADE_RATE2; // 不透明度を増す
		if (g_fAlpha >= 1.0f) 
		{
			// フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
			
			// シーン切替
			SetScene(g_eNext);
		}
		
		// ボリュームもフェードアウト
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	
	case FADE_IN:
		g_fAlpha -= FADE_RATE; // 透明度を増す
		if (g_fAlpha <= 0.0f) 
		{
			// フェードイン終了
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		
		// ボリュームもフェードイン
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	}
}

// 描画
void DrawFade() 
{
	// 画面全体に半透明ポリゴンを描画
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());
	
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
}

// フェードアウト開始
void StartFadeOut(EScene eNext)
{
	if (g_eFade != FADE_OUT) 
	{
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
	}
}
// 状態取得
EFade GetFade() 
{
	return g_eFade;
}

// フェードアウト色設定
void SetFadeColor(float fR, float fG, float fB)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}
