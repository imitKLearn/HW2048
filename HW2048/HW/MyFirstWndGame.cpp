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
			//if (msg.message == WM_LBUTTONDOWN
			//	//|| msg.message ==
			//	)
			//{
			//	std::cout << "***" << __FUNCTION__ << std::endl;
			//}
			//else
			if (msg.message == WM_KEYDOWN)
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
					AddRandomTile();
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

	std::cout << __FUNCTION__ << std::endl;
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
	bool moved = false;

	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		int temp[BOARD_SIZE] = {};
		int index = 0;

		// 1. 0이 아닌 숫자만 왼쪽부터 모으기
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			if (m_board[r][c] != 0)
			{
				temp[index] = m_board[r][c];
				++index;
			}
		}

		// 2. 같은 숫자 합치기
		for (int i = 0; i < BOARD_SIZE - 1; ++i)
		{
			if (temp[i] != 0 && temp[i] == temp[i + 1])
			{
				temp[i] *= 2;
				m_score += temp[i];

				temp[i + 1] = 0;
			}
		}

		// 3. 합치고 생긴 0을 다시 제거해서 왼쪽으로 밀기
		int result[BOARD_SIZE] = {};
		index = 0;

		for (int i = 0; i < BOARD_SIZE; ++i)
		{
			if (temp[i] != 0)
			{
				result[index] = temp[i];
				++index;
			}
		}

		// 4. 원래 보드와 다르면 이동했다고 판단
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			if (m_board[r][c] != result[c])
			{
				moved = true;
			}

			m_board[r][c] = result[c];
		}
	}

	return true;
}

bool MyFirstWndGame::MoveRight()
{
	bool moved = false;

	for (int r = 0; r < BOARD_SIZE; ++r)
	{
		int temp[BOARD_SIZE] = {};
		int index = BOARD_SIZE - 1;

		// 1. 오른쪽부터 숫자 모으기
		for (int c = BOARD_SIZE - 1; c >= 0; --c)
		{
			if (m_board[r][c] != 0)
			{
				temp[index] = m_board[r][c];
				--index;
			}
		}

		// 2. 같은 숫자 합치기
		for (int i = BOARD_SIZE - 1; i > 0; --i)
		{
			if (temp[i] != 0 && temp[i] == temp[i - 1])
			{
				temp[i] *= 2;
				m_score += temp[i];

				temp[i - 1] = 0;
			}
		}

		// 3. 다시 오른쪽으로 밀기
		int result[BOARD_SIZE] = {};
		index = BOARD_SIZE - 1;

		for (int i = BOARD_SIZE - 1; i >= 0; --i)
		{
			if (temp[i] != 0)
			{
				result[index] = temp[i];
				--index;
			}
		}

		// 4. 보드 적용
		for (int c = 0; c < BOARD_SIZE; ++c)
		{
			if (m_board[r][c] != result[c])
			{
				moved = true;
			}

			m_board[r][c] = result[c];
		}
	}

	return true;
}

bool MyFirstWndGame::MoveUp()
{
	bool moved = false;

	for (int c = 0; c < BOARD_SIZE; ++c)
	{
		int temp[BOARD_SIZE] = {};
		int index = 0;

		// 1. 위쪽부터 숫자 모으기
		for (int r = 0; r < BOARD_SIZE; ++r)
		{
			if (m_board[r][c] != 0)
			{
				temp[index] = m_board[r][c];
				++index;
			}
		}

		// 2. 같은 숫자 합치기
		for (int i = 0; i < BOARD_SIZE - 1; ++i)
		{
			if (temp[i] != 0 && temp[i] == temp[i + 1])
			{
				temp[i] *= 2;
				m_score += temp[i];

				temp[i + 1] = 0;
			}
		}

		// 3. 합친 뒤 다시 위쪽으로 밀기
		int result[BOARD_SIZE] = {};
		index = 0;

		for (int i = 0; i < BOARD_SIZE; ++i)
		{
			if (temp[i] != 0)
			{
				result[index] = temp[i];
				++index;
			}
		}

		// 4. 보드 적용
		for (int r = 0; r < BOARD_SIZE; ++r)
		{
			if (m_board[r][c] != result[r])
			{
				moved = true;
			}

			m_board[r][c] = result[r];
		}
	}

	return true;
}

bool MyFirstWndGame::MoveDown()
{
	bool moved = false;

	for (int c = 0; c < BOARD_SIZE; ++c)
	{
		int temp[BOARD_SIZE] = {};
		int index = BOARD_SIZE - 1;

		// 1. 아래쪽부터 숫자 모으기
		for (int r = BOARD_SIZE - 1; r >= 0; --r)
		{
			if (m_board[r][c] != 0)
			{
				temp[index] = m_board[r][c];
				--index;
			}
		}

		// 2. 같은 숫자 합치기
		for (int i = BOARD_SIZE - 1; i > 0; --i)
		{
			if (temp[i] != 0 && temp[i] == temp[i - 1])
			{
				temp[i] *= 2;
				m_score += temp[i];

				temp[i - 1] = 0;
			}
		}

		// 3. 다시 아래쪽으로 밀기
		int result[BOARD_SIZE] = {};
		index = BOARD_SIZE - 1;

		for (int i = BOARD_SIZE - 1; i >= 0; --i)
		{
			if (temp[i] != 0)
			{
				result[index] = temp[i];
				--index;
			}
		}

		// 4. 보드 적용
		for (int r = 0; r < BOARD_SIZE; ++r)
		{
			if (m_board[r][c] != result[r])
			{
				moved = true;
			}

			m_board[r][c] = result[r];
		}
	}

	return moved;
}