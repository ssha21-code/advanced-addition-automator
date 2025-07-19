#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define GENERATE_SUM 10

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND, LPARAM);
unsigned long long Sum(unsigned long long num1, unsigned long long num2);

HWND hStatic1;
HWND hStatic2;
HWND hStatic3;
HWND hStatic4;
HWND hEdit1;
HWND hEdit2;
HWND hEdit3;
HWND hGenerateButton;
HBRUSH hBrushBackground;
HBRUSH hBrushFrame;
const int frame_color = RGB(71, 16, 125);
const int background_color = RGB(18, 26, 36);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    SetProcessDPIAware();
    WNDCLASSW wc = {0};
    wc.hInstance = hInst;
    wc.hbrBackground = CreateSolidBrush(background_color);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"WINDOW";

    if (!RegisterClassW(&wc)) {
        return -1;
    }

    HWND hWnd = CreateWindowW(L"WINDOW", L"My Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, 
        400, 100, 700, 700, NULL, NULL, NULL, NULL
    );

    MSG msg;

    while(GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Draw your frame (colored rectangle)
            HBRUSH frameBrush = CreateSolidBrush(frame_color);
            RECT frameRect = { 10, 200, 685, 600 }; // match old frame1 coords
            FillRect(hdc, &frameRect, frameBrush);
            DeleteObject(frameBrush);

            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_CREATE:
            // Initialize brushes once
            hBrushBackground = CreateSolidBrush(background_color);
            hBrushFrame = CreateSolidBrush(frame_color);
            AddControls(hWnd, lp);
            return 0;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wp;
            HWND hWindow = (HWND)lp;

            // Use control IDs instead of HWNDs (more flexible and safe)
            int ctrlId = GetDlgCtrlID(hWindow);

            if (ctrlId == 2) { // hFrame1
                SetBkMode(hdcStatic, OPAQUE);
                SetBkColor(hdcStatic, frame_color);
                return (INT_PTR)hBrushFrame;
            } else if (ctrlId == 1) { // hStatic1
                SetTextColor(hdcStatic, RGB(255, 255, 255));
                SetBkMode(hdcStatic, TRANSPARENT);
                return (INT_PTR)GetStockObject(NULL_BRUSH);
            } else if (ctrlId == 3) { 
                SetTextColor(hdcStatic, RGB(255, 255, 255));
                SetBkMode(hdcStatic, TRANSPARENT);
                return (INT_PTR)GetStockObject(NULL_BRUSH);
            }
            return (INT_PTR)hBrushBackground;
        }
        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wp;
            SetBkColor(hdc, RGB(71, 16, 125));
            SetTextColor(hdc, RGB(255, 255, 255));
            static HBRUSH hBrushBtn = CreateSolidBrush(RGB(71, 16, 125));
            return (INT_PTR)hBrushBtn;
        }
case WM_COMMAND: 
    switch (wp) {
        case GENERATE_SUM: {
            wchar_t num1_s[15];
            wchar_t num2_s[15];
            unsigned long long num1;
            unsigned long long num2;
            unsigned long long sum;

            GetWindowTextW(hEdit1, num1_s, 15);
            GetWindowTextW(hEdit2, num2_s, 15);

            num1 = _wcstoui64(num1_s, NULL, 10);
            num2 = _wcstoui64(num2_s, NULL, 10);

            sum = Sum(num1, num2);

            wchar_t buffer[32]; // enough for 64-bit number string
            swprintf(buffer, 32, L"%llu", sum);  // use %llu for unsigned long long

            SetWindowTextW(hEdit3, buffer);
            break;
        }
    }
    return 0;
        case WM_DESTROY:
            // Cleanup
            DeleteObject(hBrushBackground);
            DeleteObject(hBrushFrame);
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddControls(HWND hWnd, LPARAM lp) {
    
    HFONT hFont1 = CreateFont(
        35,                // Height in logical units (24 for ~16pt)
        0,                 // Width (0 = auto)
        0, 0,              // Escapement, Orientation
        FW_BOLD,           // Weight (FW_NORMAL, FW_BOLD, etc.)
        FALSE,             // Italic
        FALSE,             // Underline
        FALSE,             // StrikeOut
        DEFAULT_CHARSET,   // Charset
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        TEXT("Segoe UI")   // Font name (or "Arial", "Consolas", etc.)
    );
    HFONT hFont2 = CreateFont(
        28,                // Height in logical units (24 for ~16pt)
        0,                 // Width (0 = auto)
        0, 0,              // Escapement, Orientation
        FW_BOLD,           // Weight (FW_NORMAL, FW_BOLD, etc.)
        FALSE,             // Italic
        FALSE,             // Underline
        FALSE,             // StrikeOut
        DEFAULT_CHARSET,   // Charset
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        TEXT("Segoe UI")   // Font name (or "Arial", "Consolas", etc.)
    );
    hStatic1 = CreateWindowW(L"STATIC", L"Advanced Addition Automator", WS_VISIBLE | WS_CHILD | SS_CENTER, 
        0, 50, 700, 50, hWnd, (HMENU)1, ((LPCREATESTRUCT)lp)->hInstance, NULL
    );
    SendMessageW(hStatic1, WM_SETFONT, (WPARAM)hFont1, TRUE);

    hStatic2 = CreateWindowW(L"STATIC", L"Enter the First Number: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 
        50, 270, 250, 30, hWnd, (HMENU)3, ((LPCREATESTRUCT)lp)->hInstance, NULL
    );
    SendMessageW(hStatic2, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hStatic3 = CreateWindowW(L"STATIC", L"Enter the Second Number: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 
        50, 340, 250, 30, hWnd, (HMENU)3, ((LPCREATESTRUCT)lp)->hInstance, NULL
    );
    SendMessageW(hStatic3, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hEdit1 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 
        400, 270, 150, 30, hWnd, (HMENU)4, NULL, NULL
    );
    SendMessageW(hEdit1, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hEdit2 = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 
        400, 340, 150, 30, hWnd, NULL, NULL, NULL
    );
    SendMessageW(hEdit2, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hStatic4 = CreateWindowW(L"STATIC", L"Result: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 
        50, 420, 250, 30, hWnd, (HMENU)3, ((LPCREATESTRUCT)lp)->hInstance, NULL
    );
    SendMessageW(hStatic4, WM_SETFONT, (WPARAM)hFont2, TRUE);
    hEdit3 = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY, 
        400, 420, 170, 30, hWnd, (HMENU)4, NULL, NULL
    );
    SendMessageW(hEdit3, WM_SETFONT, (WPARAM)hFont2, TRUE);

    hGenerateButton = CreateWindowW(L"BUTTON", L"Generate!", WS_VISIBLE | WS_CHILD | BS_CENTER | BS_PUSHBUTTON, 
        700/2-300/2, 490, 300, 50, hWnd, (HMENU)GENERATE_SUM, NULL, NULL
    );
    SendMessageW(hGenerateButton, WM_SETFONT, (WPARAM)hFont2, TRUE);

    SendMessageW(hWnd, WM_CHANGEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
}

unsigned long long Sum(unsigned long long num1, unsigned long long num2) {
    return num1 + num2;
}