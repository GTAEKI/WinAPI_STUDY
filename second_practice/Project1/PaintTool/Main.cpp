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
	static int oldMouseX;
	static int oldMouseY;
	
	HDC hdc;
	PAINTSTRUCT paintStruct;

	switch (messageType)
	{
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		isDrawing = TRUE;
		return 0;
	case WM_MOUSEMOVE:
		
		{
		int nowMouseX = LOWORD(lParam);
		int nowMouseY = HIWORD(lParam);
		if(isDrawing)
		{
			hdc = GetDC(hWnd);
			DrawLine(hdc, oldMouseX, oldMouseY, nowMouseX, nowMouseY);
			ReleaseDC(hWnd, hdc);
		}
		oldMouseX = nowMouseX;
		oldMouseY = nowMouseY;
		return 0;
		}
	case WM_LBUTTONUP:
		isDrawing = FALSE;
		return 0;
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

// 타이머
/* case WM_CREATE:
*  SetTimer(hWnd, 1, 1000, NULL);
*  return 0;
* */
