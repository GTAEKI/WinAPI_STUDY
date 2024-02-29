#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

void DrawLine(HDC hdc, int startX, int startY, int endX, int endY )
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

void DrawRect(HDC hdc, RECT rt)
{
	Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
}

void DrwaEllipse(HDC hdc, int x, int y, int radWidth, int radHeight)
{
	Ellipse(hdc, x - radWidth, y - radHeight, x + radWidth, y + radHeight);
}


LRESULT CALLBACK WNDProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC hdc;
	PAINTSTRUCT paintStruct; //PAINTSTRUCT 검색해보기

	int myAge = 10;

	TCHAR str[256];
	_stprintf(str, TEXT("안녕 내 나이는 %d야"), myAge); // TOString이 없는 상황에서, printf처럼 사용할수있게.

	switch (msg) 
	{
		case WM_KEYDOWN:
			if (MessageBox(hWnd, TEXT("종료하시겠습니까."), TEXT("알림"), MB_YESNO | MB_ICONINFORMATION) == IDYES)
			{
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			}
			break;

		case WM_PAINT:	

			// BeginPaint, End는 WM_Paint만 쓸수있다.
			hdc = BeginPaint(hWnd ,&paintStruct);
			//for (int y = 0; y <= 255; ++y)  //ctrl kc ctrl cu
			//{
			//	for (int x = 0; x <= 255; ++x) 
			//	{
			//		SetPixel(hdc, 50+x, 50+y, RGB(x, y, 0)); // 느림
			//		//COLORREF 검색해보기
			//	}
			//}

			//LineTo(hdc, 50, 50); // 그냥 쓰면 UPDATECP와 같다. 계속 이어져서 그려짐 , start가 없다. 함수에
			//LineTo(hdc, 70, 100); // 50, 50 -> 70, 100
			//{
			//	POINT oldPos;
			//	MoveToEx(hdc, 80, 80, NULL); // oldPos에 현재 커서위치값 저장해주고, 80,80으로 커서 옮김 즉 현재위치 저장하고 움직이는것
			//	//MoveToEx(hdc, 80, 80, &oldPos); // oldPos에 현재 커서위치값 저장해주고, 80,80으로 커서 옮김 즉 현재위치 저장하고 움직이는것

			//}
			//LineTo(hdc, 90, 200);

			DrawLine(hdc, 0, 0, 50, 50);
			DrawLine(hdc, 0, 0, 70, 100);
			DrawLine(hdc, 0, 0, 90, 200);

			SelectObject(hdc, GetStockObject(DC_BRUSH)); // winAPI가 발전하며 나온 편한 방식
			SetDCBrushColor(hdc, RGB(255, 0, 0));
			Rectangle(hdc, 20, 30, 40, 60);

			Ellipse(hdc, 50, 50, 100, 800);

			

			SetTextAlign(hdc, TA_LEFT); // Printf TA_UPDATECP -> 프린트 에프처럼 쓸 수 있게 해주는 옵션
			TextOut(hdc, 30, 50, str, _tcslen(str));
			TextOut(hdc, 30, 60, str, _tcslen(str));
			
			EndPaint(hWnd, &paintStruct);

			

			// Get과 Release 한쌍, Begin과 End가 한쌍
			//hdc = GetDC(hWnd);
			//TextOut(hdc, 50, 50, TEXT("Hello_World"), _tcslen(TEXT("Hello_World")));
			//ReleaseDC(hWnd, hdc);

			return 0;

		// 윈도우 창 소멸 이벤트
		case WM_DESTROY:
			// WM_QUIT 메세지 생성
			PostQuitMessage(0); // 큇 메시지를 포스트하다. wparam이라는 멤버변수를 0으로 초기화하겠다.
			return S_OK;
			//return 0;
			//return E_FAIL
	}
	return DefWindowProc(hWnd, msg, wParam, lParam); //Default Window Procdure
}


/* window 에서는 winMain을 써야함*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	/*API ENTRY -> 보라색은 매크로를 뜻함, 마우스 가져다 대서 키워드 나오는거 검색*/

	WNDCLASS winClass;
	
	winClass.lpszClassName = TEXT("First_Practice");
	winClass.lpszMenuName = NULL; /* 관련된 에디터도 있음, 검색해보기, 맵 툴같은거 만들때는 필요한데 게임은 창만있으면 되다보니 설정 안함 NULL*/
	
	winClass.hInstance = hInstance; /* 프로그램이 종료되면 기억해둔 핸들값을 통해 연결된 창의 등록을 모두 해제*/
	winClass.lpfnWndProc = WNDProc;// 함수 포인터 변수이다_ 함수를 담을 수 있는 변수_ 우리가 프로시저를 정의만 해주면 winClass에서 알아서 메시지 받을때마다 호출해줌*/
	
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.hIcon = LoadIcon(NULL,IDI_APPLICATION); // 알아서 a로갈지 b로갈지 됨, 어플리케이션 아이콘의 ID( IDI )이다.
	/*직접 독특한 아이콘 만들고싶으면 매크로를 재정의 해줘야함*/
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 화살표 커서이다. 핸드 등 여러가지가 있음
	winClass.style = CS_HREDRAW | CS_VREDRAW; // 클래스 스타일 리드로우, 
	
	// 여분 메모리 2개
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;

	// 여기까지 템플릿을 만든것, 이제 등록을 함_ 템플릿 주소값을 전달해주면 됨
	RegisterClass(&winClass);

	HWND windowHandle = CreateWindow(winClass.lpszClassName, winClass.lpszClassName, WS_OVERLAPPEDWINDOW, 
									CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, 
									NULL, NULL, hInstance, NULL);

	ShowWindow(windowHandle,nCmdShow);
	
	HDC hdc = GetDC(windowHandle); //DC 완성 -> 동적으로 생성하는것이라 사실 new와 같다.
	ReleaseDC(windowHandle, hdc); // DC를 다 사용해줬다면 해제





	MSG msg; // 메세지
	// 메세지 루프 ->
	while (TRUE) 
	{
		// 프로그램 종료 전까지 계속 메세지 받아옴
		if (GetMessage(&msg, NULL, 0, 0) == FALSE)  // GETMessage에서 메시지 큐가 empty라면 대기를 무한정 한다. 운영체제로부터 값이 안들어오면 멈춰있음, 블러킹함수라고 한다.
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg); /* 프로시저에 보내주는(알려주는) 함수*/

		//{ //바탕화면에 색을 그릴 수 있음
		//	HDC screenDC = GetDC(NULL); // 주의사항 : GETDC했으면 항상 Release를 바로 밑에 써주고 해야함. 이거 못하면 주니어
		//	RECT rect;
		//	rect.left = 10;
		//	rect.top = 10;
		//	rect.right = 50;
		//	rect.bottom = 50;
		//	DrawRect(screenDC, rect);
		//	ReleaseDC(NULL, screenDC);

		//}
	}
	// <- 메세지 루프


	return msg.wParam;
}