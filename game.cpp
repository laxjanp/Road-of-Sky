//=============================================================================
// ゲーム画画『game.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "game.h"

#include "polygon.h"
#include "Sound.h"
#include "scene.h"
#include "fade.h"
#include "input.h"

#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "enemy2.h"
#include "score.h"
#include "timer.h"
#include "life.h"
#include "pause.h"

//==========================================
// グローバル変数(g_)
//==========================================
static bool g_bPause = false;


//初期化
HRESULT InitGame()
{
	HRESULT hr = S_OK;
	
	// プレイヤー初期化
	hr = InitPlayer();
	if (FAILED(hr)) 
	{
		MessageBox(GetMainWnd(), _T("プレイヤー初期化失敗"), NULL, MB_OK);
		return hr;
	}

	// 背景初期化
	hr = InitBG();
	if (FAILED(hr)) 
	{
		MessageBox(GetMainWnd(), _T("背景初期化失敗"), NULL, MB_OK);
		return hr;
	}

	// 弾初期化
	hr = InitBullet();
	if (FAILED(hr)) 
	{
		MessageBox(GetMainWnd(), _T("弾初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// エフェクト表示初期化
	hr = InitEffect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("エフェクトの初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//敵の初期化
	hr = InitEnemy();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("敵の初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	hr = InitEnemy2();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("敵の初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// スコア表示初期化
	hr = InitScore();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("スコア表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// タイマー表示初期化
	hr = InitTimer();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("タイマー表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ライフ表示初期化
	hr = InitLife();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("ライフの表示初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// 一時停止初期化
	hr = InitPause();
	if (FAILED(hr)) 
	{
		MessageBox(GetMainWnd(), _T("一時停止処理初期化失敗"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}
	g_bPause = false;

	// BGM再生
	CSound::Play(BGM_CYBER41);

	return hr;
}


//終了
void UninitGame()
{
	// BGM再生停止
	CSound::Stop(BGM_CYBER41);

	// 一時停止終了処理
	UninitPause();

	// ライフ表示終了処理
	UninitLife();
	
	// タイマー終了処理
	UninitTimer();
	
	// スコア表示終了処理
	UninitScore();
	
	// 敵終了処理
	UninitEnemy();
	UninitEnemy2();

	// エフェクト表示終了処理
	UninitEffect();
	
	// 弾終了処理
	UninitBullet();
	
	// 背景表示終了処理
	UninitBG();
	
	// プレイヤー終了処理
	UninitPlayer();

}


//更新
void UpdateGame()
{
	// 一時停止中
	if (g_bPause)
	{
		// 一時停止の更新
		UpdatePause();
	}
	else
	{
		// 背景更新
		UpdateBG();

		// プレイヤー更新
		UpdatePlayer();
		if (IsGameOver())
		{
			StartFadeOut(SCENE_GAMEOVER);
			return;
		}

		// 弾更新
		UpdateBullet();

		// 爆発エフェクト表示更新
		UpdateEffect();

		// 敵更新
		UpdateEnemy();
		UpdateEnemy2();

		// タイマー更新
		UpdateTimer();
	}

	// 一時停止ON/OFF
	if (GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE)) 
	{
		
		// フェードイン/アウト中でなければ
		if (GetFade() == FADE_NONE)
		{
			g_bPause = !g_bPause;	// 反転

			if (g_bPause) 
			{
				CSound::Play(SE_DECIDE);
				ResetPauseMenu();
			}
			else 
			{
				CSound::Play(SE_CANCEL);
				CSound::Resume();
			}
		}
	}

	// 一時停止中(かつフェードイン/アウトしていない)?
	if (g_bPause && GetFade() == FADE_NONE)
	{
		CSound::Stop(SE_DECIDE);
		CSound::Stop(SE_SELECT);

		// [Enter]が押された?
		if (GetKeyTrigger(VK_RETURN))
		{
			switch (GetPauseMenu())
			{	// 現在の選択項目
			case PAUSE_MENU_CONTINUE:
				g_bPause = false;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;

			case PAUSE_MENU_RETRY:
				CSound::Play(SE_DECIDE);
				StartFadeOut(SCENE_GAME);
				break;

			case PAUSE_MENU_QUIT:
				CSound::Play(SE_DECIDE);
				StartFadeOut(SCENE_TITLE);
				break;
			}
		}
	}
}


//描画
void DrawGame()
{
	// 背景描画
	DrawBG();
	
	// プレイヤー描画
	DrawPlayer();
	
	// 敵描画
	DrawEnemy();
	DrawEnemy2();
	
	// 弾描画
	DrawBullet();
	
	// 爆発エフェクト表示
	DrawEffect();
	
	// スコア表示
	DrawScore();
	
	// タイマー表示
	DrawTimer();
	
	// ライフ表示
	DrawLife();

	// 一時停止描画
	if (g_bPause) 
	{
		DrawPause();
	}

}