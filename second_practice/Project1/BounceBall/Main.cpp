#include <Windows.h>
#include <time.h>
#include "resource.h"

HINSTANCE g_hinstance;
typedef struct
{
	COLORREF color;
	POINT pos;
	POINT velocity;
	int radius;
} ball_t;

ball_t balls[200];
int ball_size = sizeof(balls) / sizeof(balls[0]);
HBITMAP hbitmap;
BITMAP bitmapInfo;
int x;
int y;

// bitmap = static_cast<HBITMAP>(LoadImage(nullptr, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
// 경로로 이미지 불러오기 -> bmp확장자로만 바꿔주면 그냥 쓸수있게됨. 
// win API에서는 비트맵만 지원하기때문.
// exe에서 클릭하면 exe로부터 경로가 시작되고, 디버깅모드에서 실행하면 프로젝트에서 경로가 시작됨.
// 즉 베포할때는 exe파일 옆에다 두고 압축해서 배포하면 된다.

LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{

	switch (messageType)
	{
	case WM_CREATE:
	{
		hbitmap = LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP1));
		bitmapInfo;
		GetObject(hbitmap, sizeof(BITMAP), &bitmapInfo);


		srand((unsigned int)time(NULL)); // 랜덤에 cd값 넣어서 세팅?
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		
		for(int i = 0; i < ball_size; ++i)
		{
			balls[i].pos.x = rand() % clientRect.right;
			balls[i].pos.y = rand() % clientRect.bottom;

			balls[i].velocity.x = (rand() % 5 + 1) * (rand() % 2 == 0 ? -1 : 1);
			balls[i].velocity.y = (rand() % 5 + 1) * (rand() % 2 == 0 ? -1 : 1);

			balls[i].radius = rand() % 5 * 5 + 10;

			balls[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
		}

		SetTimer(hWnd, 1, 25, NULL);


		return S_OK;
	}

	case WM_KEYDOWN:

		switch(wParam)
		{
		case VK_LEFT:
			
			x = x - 5;

			break;
		case VK_RIGHT:

			x = x + 5;
			break;

		case VK_UP:

			y = y - 5;

			break;
		case VK_DOWN:

			y = y + 5;
			break;
		}


		return 0;

	case WM_TIMER:
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		for(int i = 0; i < ball_size; ++i)
		{
			{
				if(balls[i].pos.x <= balls[i].radius)
				{
					balls[i].pos.x = balls[i].radius;
					balls[i].velocity.x *= -1;
				}

				if(balls[i].pos.x >= clientRect.right - balls[i].radius)
				{
					balls[i].pos.x = clientRect.right - balls[i].radius;
					balls[i].velocity.x *= -1;
				}

				if(balls[i].pos.y <= balls[i].radius)
				{
					balls[i].pos.y = balls[i].radius;
					balls[i].velocity.y *= -1;
				}

				if (balls[i].pos.y >= clientRect.bottom - balls[i].radius)
				{
					balls[i].pos.y = clientRect.bottom - balls[i].radius;
					balls[i].velocity.y *= -1;
				}

			}

			balls[i].pos.x += balls[i].velocity.x;
			balls[i].pos.y += balls[i].velocity.y;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return S_OK;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HDC bitmapDC = CreateCompatibleDC(hdc);
		SelectObject(bitmapDC, hbitmap);

		HDC memDC = CreateCompatibleDC(hdc);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);



		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		HBITMAP rollbackBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

		FillRect(memDC, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		for(int i = 0; i < ball_size; ++i)
		{
			SelectObject(memDC, GetStockObject(DC_BRUSH));
			SetDCBrushColor(memDC, balls[i].color);

			Ellipse(memDC, balls[i].pos.x - balls[i].radius, balls[i].pos.y - balls[i].radius,
				balls[i].pos.x + balls[i].radius, balls[i].pos.y + balls[i].radius);
		}

		BitBlt(memDC, x, y, bitmapInfo.bmWidth, bitmapInfo.bmHeight, bitmapDC, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memDC, rollbackBitmap));
		DeleteDC(bitmapDC);
		DeleteDC(memDC);


		EndPaint(hWnd, &ps);


		return 0;
	}
	case WM_DESTROY:

		KillTimer(hWnd, 1);
		DeleteObject(hbitmap);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, messageType, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hinstance = hInstance;
	//WNDCLASSEX windowClass;
	WNDCLASS windowClass;

	windowClass.lpszClassName = TEXT("Second Project");
	windowClass.lpszMenuName = NULL;

	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;

	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	RegisterClass(&windowClass);

	HWND hWND = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWND, nCmdShow);

	MSG msg;

	while (TRUE)
	{
		if (GetMessage(&msg, NULL, 0, 0) == FALSE)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}