#include "MyFirstWndGame.h"

bool MyFirstWndGame::Initialize()
{
	bool isOn = false;
	_InitTimer();
	isOn = _InitWindow();
	_Init2048();
	return isOn;
}

void MyFirstWndGame::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_LBUTTONDOWN
				//|| msg.message ==
				)
			{
				std::cout << "***" << __FUNCTION__ << std::endl;
			}
			else if (msg.message == WM_KEYDOWN)
			{
				bool moved = false;

				if (msg.wParam == VK_LEFT)
					moved = MoveLeft();
				else if (msg.wParam == VK_RIGHT)
					moved = MoveRight();
				else if (msg.wParam == VK_UP)
					moved = MoveUp();
				else if (msg.wParam == VK_DOWN)
					moved = MoveDown();

				if (moved)
				{
					//AddRandomTile();
				}
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Update();
			Render();
		}
	}
}

void MyFirstWndGame::Finalize()
{
	delete m_pGameTimer;
	m_pGameTimer = nullptr;
}

void MyFirstWndGame::_InitTimer()
{
	m_pGameTimer = new GameTimer();
	m_pGameTimer->Reset();
}

bool MyFirstWndGame::_InitWindow()
{
	const wchar_t* className = L"MyFirstWndGame";
	const wchar_t* windowName = L"MyFirstWndGame_HW2048";

	if (false == __super::Create(className, windowName, 1024, 720))
	{
		return false;
	}

	RECT rcClient = {};
	GetClientRect(m_hWnd, &rcClient);
	m_width = rcClient.right - rcClient.left;
	m_height = rcClient.bottom - rcClient.top;

	m_hFrontDC = GetDC(m_hWnd);
	m_hBackDC = CreateCompatibleDC(m_hFrontDC);
	m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);
	m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);
}

void MyFirstWndGame::_Init2048()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			m_board[r][c] = 0;
		}
	}

	m_score = 0;

	AddRandomTile();
	AddRandomTile();
}

void MyFirstWndGame::AddRandomTile()
{
	int emptyCount = 0;

	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			if (m_board[r][c] == 0)
			{
				++emptyCount;
			}
		}
	}

	if (emptyCount == 0)
		return;

	int randomIndex = rand() % emptyCount;
	int currentIndex = 0;

	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			if (m_board[r][c] == 0)
			{
				if (currentIndex == randomIndex)
				{
					m_board[r][c] = 2;
					return;
				}

				++currentIndex;
			}
		}
	}
}

void MyFirstWndGame::RenderBoard()
{
	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			RenderTile(r, c, m_board[r][c]);
		}
	}
}

void MyFirstWndGame::RenderTile(int row, int col, int value)
{
	int x = m_boardStartX + col * (m_cellSize + m_cellGap);
	int y = m_boardStartY + row * (m_cellSize + m_cellGap);

	Rectangle(
		m_hBackDC,
		x,
		y,
		x + m_cellSize,
		y + m_cellSize
	);

	if (value != 0)
	{
		std::wstring text = std::to_wstring(value);

		TextOut(
			m_hBackDC,
			x + 35,
			y + 35,
			text.c_str(),
			(int)text.length()
		);
	}
}

void MyFirstWndGame::Update()
{
	m_pGameTimer->Tick();

	//LogicUpdate();

	m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
	m_fFrameCount += m_fDeltaTime;

	while (m_fFrameCount >= 200.0f)
	{
		//	FixedUpdate();
		m_fFrameCount -= 200.0f;
	}
}

void MyFirstWndGame::Render()
{
	//Clear the back buffer
	::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

	RenderBoard();

	//메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
	BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

bool MyFirstWndGame::MoveLeft()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveRight()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveUp()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveDown()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}