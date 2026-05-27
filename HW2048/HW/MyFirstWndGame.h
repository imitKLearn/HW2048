#pragma once
#include "NzWndBase.h"
#include "GameTimer.h"

#include <iostream>
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
};
