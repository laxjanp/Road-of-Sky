#pragma once
//=============================================================================
// スコア表示専用コード『score.h』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// プロトタイプ宣言(関数呼び出し)
//*****************************************************************************
HRESULT InitScore(void);	//初期化
void UninitScore(void);		//終了
void DrawScore(void);		//描画
void AddScore(int);		//更新
int GetScore(void);			//更新