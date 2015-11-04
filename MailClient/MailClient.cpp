// MailClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MailClient.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                               // current instance
char szTitle[MAX_LOADSTRING];                  // The title bar text
char szWindowClass[MAX_LOADSTRING];            // the main window class name
const int wndWidth = 640;
const int wndHight = 480;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Connect(HWND, UINT, WPARAM, LPARAM);
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	AllocConsole();
	freopen("CON", "wt", stdout);
	freopen("CON", "wt", stderr);

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MAILCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAILCLIENT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAILCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_MAILCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // Lock handle on Desktop
   const HWND hDesktop = GetDesktopWindow();
   RECT desktop;

   // Get screen resolution
   GetWindowRect(hDesktop, &desktop);

   // Calculate screen offset
   int horizontal = (desktop.right / 2) - (wndWidth / 2);
   int vertical = (desktop.bottom / 2) - (wndHight / 2);

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   horizontal, vertical, wndWidth, wndHight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_CLIENT_CONNECT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONNECT), hWnd, Connect);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

struct MessageFile {
	const char *filename;
	FILE *stream;
	std::string rawMsg;
};

INT_PTR CALLBACK Connect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char username[MAX_LOADSTRING];
	char password[MAX_LOADSTRING];
	char imapserv[MAX_LOADSTRING];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), "imap.server.com");
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), "email@domain.com");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK)
		{			
			CURL *curl;
			CURLcode res = CURLE_OK;

			struct MessageFile msgfile = {
				"imap-Message.txt", // name to store the file as if successful
				NULL
			};

			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();

			if (curl) 
			{
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), "email@mail.ru");
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), "password");

				// Get username and password
				GetWindowText(GetDlgItem(hDlg, IDC_EDIT3), username, MAX_LOADSTRING);
				GetWindowText(GetDlgItem(hDlg, IDC_EDIT4), password, MAX_LOADSTRING);
				GetWindowText(GetDlgItem(hDlg, IDC_EDIT4), imapserv, MAX_LOADSTRING);
				std::string server = imapserv;
				server = "imaps://" + server;

				// Set username and password
				curl_easy_setopt(curl, CURLOPT_USERNAME, username);
				curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

				// Using SSL to connect to IMAP
				server = server + "/INBOX";
				// imaps://imap.mail.ru/INBOX/;UID=779	/;SECTION=TEXT
				curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.mail.ru/INBOX/;UID=780");
				//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE INBOX");		// - get Information
				//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST \"\" *");

				// Define our callback to get called when there's data to be written
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &msgfile);

				curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

				// Turn on debug information
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

				// Perform the fetch
				res = curl_easy_perform(curl);

				// Check for errors
				if (res != CURLE_OK)
					fprintf(stderr, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));
				else
				{
					printf("%s\n", msgfile.rawMsg.c_str());
				}

				// Always cleanup
				curl_easy_cleanup(curl);
			}

			curl_global_cleanup();
		}
		break;
	}
	return (INT_PTR)FALSE;
}

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct MessageFile *out = (struct MessageFile *)stream;
	if (out && !out->stream) 
	{
		/* open file for writing */
		out->stream = fopen(out->filename, "wb");
		out->rawMsg += (char *)buffer;
		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}

	return fwrite(buffer, size, nmemb, out->stream);
}