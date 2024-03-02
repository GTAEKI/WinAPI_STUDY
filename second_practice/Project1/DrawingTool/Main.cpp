#include<Windows.h>
#include<tchar.h>

void DrawLine(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	static BOOL isDrawing = FALSE;
	static int startX, startY;
	static int oldX, oldY;

	switch (messageType)
	{
	case WM_LBUTTONDOWN:
	{
		isDrawing = TRUE;
		HDC hdc = GetDC(hWnd);

		startX = LOWORD(lParam);
		startY = HIWORD(lParam);

		oldX = startX;
		oldY = startY;

		//반전 모드로 처음 직선을 그림
		SetROP2(hdc, R2_NOT);
		HPEN oldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);
		DeleteObject(SelectObject(hdc, oldPen));
		ReleaseDC(hWnd, hdc);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if(isDrawing == FALSE)
		{
			return 0;
		}

		HDC hdc = GetDC(hWnd);
		
		int nowX = LOWORD(lParam);
		int nowY = HIWORD(lParam);

		// 이전에 그렸던 직선을 반전 모드로 다시 그림
		SetROP2(hdc, R2_NOT);
		HPEN oldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);

		// 반전 모드로 새 직선을 그림
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, nowX, nowY);
		DeleteObject(SelectObject(hdc, oldPen));

		oldX = nowX;
		oldY = nowY;

		ReleaseDC(hWnd, hdc);
		return 0;
	}

	case WM_LBUTTONUP:
	{
		isDrawing = FALSE;
		HDC hdc = GetDC(hWnd);

		// 마우스를 놓는 순간, 선을 다시 덮어쓰므로 빈 여백이 채워진다.
		HPEN oldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);
		DeleteObject(SelectObject(hdc, oldPen));

		ReleaseDC(hWnd, hdc);
		return 0;
	}
	

	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hWnd, messageType, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{

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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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