#pragma once
#include "NzWndBase.h"
#include "GameTimer.h"

#include <iostream>
#include <random> // 난수 라이브러리 포함
#include <string> // 보드 숫자
#pragma region 전방선언
class GameTimer;
#pragma endregion

class MyFirstWndGame : public NzWndBase
{
public:
	MyFirstWndGame() = default;
	~MyFirstWndGame() override = default;

	bool Initialize();
	void Run();
	void Finalize();
private:
	void _InitTimer();
	bool _InitWindow();

private:
	void _Init2048();
	void AddRandomTile();
	void RenderBoard();
	void RenderTile(int row, int col, int value);

private:
	void Update();
	void Render();

private:
	bool MoveLeft();
	bool MoveRight();
	bool MoveUp();
	bool MoveDown();

private:
	HDC m_hFrontDC = nullptr;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBitmap = nullptr;
	HBITMAP m_hDefaultBitmap = nullptr;

private:
	GameTimer* m_pGameTimer = nullptr;
	float m_fDeltaTime = 0.0f;
	float m_fFrameCount = 0.0f;
private:
	static const int BOARD_SIZE = 4;

	int m_board[BOARD_SIZE][BOARD_SIZE] = {};
	int m_score = 0;

	int m_boardStartX = 300;
	int m_boardStartY = 150;
	int m_cellSize = 90;
	int m_cellGap = 10;
};
