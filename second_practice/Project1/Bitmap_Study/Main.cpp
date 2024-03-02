#include <Windows.h>
#include "resource.h" //표준 라이브러리가 아님 ""

HINSTANCE g_hinstance;


// DrawBitmap이라는 함수가 메모장에 적혀있는데, 
// 이게 hdc를 복사했다가 파괴하는 과정을 반복함. 
// 성능적으로 이슈가 없는지 검색하기


LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	static BITMAP bitmapInfo;
	PAINTSTRUCT paintStruct;
	static HBITMAP hbitmap;

	switch (messageType)
	{
	case WM_CREATE:
	{
		hbitmap = LoadBitmap(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP1));
		
		GetObject(hbitmap,sizeof(BITMAP), &bitmapInfo); // 매개변수 중간 c는 capacity의 약자
		

		return S_OK;
	}

	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &paintStruct);

		HDC fullScreenDC = GetDC(NULL);

		BitBlt(hdc, 50, 50, 200, 200, fullScreenDC, 0, 0 ,SRCCOPY);
		ReleaseDC(NULL, fullScreenDC);


		//HDC memDC = CreateCompatibleDC(hdc);
		//SelectObject(memDC, hbitmap);
		//BitBlt(hdc, 50, 50, bitmapInfo.bmWidth, bitmapInfo.bmHeight, memDC, 0, 0, SRCCOPY);
		//DeleteDC(memDC);

		EndPaint(hWnd, &paintStruct);

		return 0;
	}
	//case WM_KEYDOWN:

	//{
	//	HDC hdc = GetDC(hWnd);
	//	HBITMAP hBitMap = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP); // 핸들을 가져온다.


	//	BITMAP bitmap;

	//	GetObject(hBitMap, sizeof(BITMAP), &bitmap);


	//	ReleaseDC(hWnd, hdc);
	//}


	//	return S_OK;

	case WM_DESTROY:

		PostQuitMessage(0);
		DeleteObject(hbitmap);

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