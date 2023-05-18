//=============================================================================
// シーン画面『scene.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "scene.h"
#include "title.h"
#include "game.h"
#include "gameover.h"
#include "fade.h"

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static EScene g_eScene = SCENE_NOME;

//初期化
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE);	//最初はタイトル画面に
	InitFade();				//フェードの初期化をする
	return hr;
}

// 終了処理
void UninitScene()
{
	SetScene(SCENE_NOME);	//現在のシーンを終了する処理
	UninitFade();			//フェードの終了
}

// 更新
void UpdateScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		//タイトル画面の呼び出し
		UpdateTitle();		//この関数の呼び出しをする
		break;
	
	case SCENE_GAME:		//ゲーム画面の呼び出し
		UpdateGame();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		UpdateGameOver();
		break;

	default:
		break;
	}
	UpdateFade();			//フェードの更新
}

// 描画
void DrawScene()
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//タイトル画面の呼び出し
		DrawTitle();		//この関数の呼び出しをする
		break;

	case SCENE_GAME:		//ゲーム画面の呼び出し
		DrawGame();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		DrawGameOver();
		break;
	
	
	default:
		break;
	}
	DrawFade();				//フェードの描画
}

//=============================================================================
//SetScene関数
//内容:シーンの切り替えをする関数
//引数:シーン切り替えのeScene
//戻り値:なし
//=============================================================================
void SetScene(EScene eScene)
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//タイトル画面の呼び出し
		UninitTitle();		//この関数の呼び出しをする
		break;
	
	case SCENE_GAME:		//ゲーム画面の呼び出し
		UninitGame();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		UninitGameOver();
		break;

	}
	
	//シーン切替
	g_eScene = eScene;

	//次の画面へ以降
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//タイトル画面の呼び出し
		InitTitle();		//この関数の呼び出しをする
		break;
	
	case SCENE_GAME:		//ゲーム画面の呼び出し
		InitGame();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		InitGameOver();
		break;
	}

}