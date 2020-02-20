#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HIMAGELIST g_hImageList = NULL;

HWND CreateSimpleToolbar(HWND hWndParent)
{
    HINSTANCE hInst = GetModuleHandle(NULL);
    // Declare and initialize local constants.
    const int ImageListID    = 0;
    const int numButtons     = 3;
    const int bitmapSize     = 16;
    
    const DWORD buttonStyles = BTNS_AUTOSIZE | BTNS_DROPDOWN;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, 
        WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE, 0, 0, 0, 0, 
        hWndParent, NULL, hInst, NULL);
        
    if (hWndToolbar == NULL)
        return NULL;
    //SetWindowTheme(hWndToolbar, L" ", L" ");

    // Create the image list.
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
        ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
        numButtons, 0);

    // Set the image list.
    SendMessage(hWndToolbar, TB_SETIMAGELIST, 
        (WPARAM)ImageListID, 
        (LPARAM)g_hImageList);

    // Load the button images.
    SendMessage(hWndToolbar, TB_LOADIMAGES, 
        (WPARAM)IDB_STD_SMALL_COLOR, 
        (LPARAM)HINST_COMMCTRL);

    // Initialize button info.
    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

    SendMessage(hWndToolbar, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS);
    
    TBBUTTON tbButtons[numButtons] = 
        {
            { MAKELONG(STD_FILENEW,  ImageListID), 0,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"New" },
            { MAKELONG(STD_FILEOPEN, ImageListID), 0, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Open"},
            { MAKELONG(STD_FILESAVE, ImageListID), 0, 0,               buttonStyles, {0}, 0, (INT_PTR)L"Save"}
        };

    // Add buttons.
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS,       (WPARAM)numButtons,       (LPARAM)&tbButtons);

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0); 
    ShowWindow(hWndToolbar,  TRUE);
    
    return hWndToolbar;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    if (!CreateSimpleToolbar(hwnd))
        return 0;
    
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


