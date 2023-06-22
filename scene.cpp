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

// クラスを用意した変数を用意
CGame game;				// ゲームクラス
CTitle title;			// タイトルクラス
CGameover gameover;		// ゲームオーバークラス


//初期化
HRESULT CScene::Init()
{
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE);	//最初はタイトル画面に
	InitFade();				//フェードの初期化をする
	return hr;
}

// 終了処理
void CScene::Uninit()
{
	SetScene(SCENE_NOME);	//現在のシーンを終了する処理
	UninitFade();			//フェードの終了
}

// 更新処理
void CScene::Update()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		//タイトル画面の呼び出し
		title.Update();		//この関数の呼び出しをする
		break;

	case SCENE_GAME:		//ゲーム画面の呼び出し
		game.Update();
		break;

	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		gameover.Update();
		break;

	default:
		break;
	}
	UpdateFade();			//フェードの更新
}

// 描画処理
void CScene::Draw()
{
	switch (g_eScene)
	{

	case SCENE_TITLE:		//タイトル画面の呼び出し
		title.Draw();		//この関数の呼び出しをする
		break;

	case SCENE_GAME:		//ゲーム画面の呼び出し
		game.Draw();
		break;

	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		gameover.Draw();
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
void CScene::SetScene(EScene eScene)
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//タイトル画面の呼び出し
		title.Uninit();		//この関数の呼び出しをする
		break;
	
	case SCENE_GAME:		//ゲーム画面の呼び出し
		game.Uninit();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		gameover.Uninit();
		break;

	}
	
	//シーン切替
	g_eScene = eScene;

	//次の画面へ以降
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//タイトル画面の呼び出し
		title.Init();		//この関数の呼び出しをする	
		break;
	
	case SCENE_GAME:		//ゲーム画面の呼び出し
		game.Init();
		break;
	
	case SCENE_GAMEOVER:	//ゲームオーバーの呼び出し
		gameover.Init();
		break;
	}

}