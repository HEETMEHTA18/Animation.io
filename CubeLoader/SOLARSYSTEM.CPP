#include <windows.h>
#include <cmath>
#include <vector>
#include <cstdlib>

// Structure to hold star positions.
struct Star {
    int x;
    int y;
};

std::vector<Star> stars;
const int numStars = 200;
float earthAngle = 0.0f;
float marsAngle  = 0.0f;
RECT clientRect;

// Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_CREATE: {
            // Get window client dimensions.
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;
            // Generate star positions.
            for (int i = 0; i < numStars; ++i) {
                Star s;
                s.x = rand() % width;
                s.y = rand() % height;
                stars.push_back(s);
            }
            // Set a timer (ID 1, ~16 ms interval for ~60 FPS).
            SetTimer(hwnd, 1, 16, NULL);
            return 0;
        }
        case WM_TIMER: {
            // Update orbit angles.
            earthAngle += 0.05f;
            marsAngle  += 0.03f;
            // Force window to repaint.
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Fill background with black.
            HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &ps.rcPaint, hBrushBlack);
            DeleteObject(hBrushBlack);
            
            // Draw stars as small white circles.
            HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
            for (const auto& s : stars) {
                // Draw a 2x2 pixel ellipse.
                Ellipse(hdc, s.x, s.y, s.x + 2, s.y + 2);
            }
            DeleteObject(hBrushWhite);
            
            // Determine the center of the window.
            int cx = (clientRect.right - clientRect.left) / 2;
            int cy = (clientRect.bottom - clientRect.top) / 2;
            
            // Draw the Sun (gold circle).
            HBRUSH hBrushSun = CreateSolidBrush(RGB(255, 215, 0));
            int sunRadius = 30;
            Ellipse(hdc, cx - sunRadius, cy - sunRadius, cx + sunRadius, cy + sunRadius);
            DeleteObject(hBrushSun);
            
            // Draw Earth (blue circle) orbiting at radius 100.
            int earthOrbitRadius = 100;
            int earthRadius = 10;
            int earthX = cx + static_cast<int>(earthOrbitRadius * cos(earthAngle));
            int earthY = cy + static_cast<int>(earthOrbitRadius * sin(earthAngle));
            HBRUSH hBrushEarth = CreateSolidBrush(RGB(0, 0, 255));
            Ellipse(hdc, earthX - earthRadius, earthY - earthRadius, earthX + earthRadius, earthY + earthRadius);
            DeleteObject(hBrushEarth);
            
            // Draw Mars (red circle) orbiting at radius 150.
            int marsOrbitRadius = 150;
            int marsRadius = 8;
            int marsX = cx + static_cast<int>(marsOrbitRadius * cos(marsAngle));
            int marsY = cy + static_cast<int>(marsOrbitRadius * sin(marsAngle));
            HBRUSH hBrushMars = CreateSolidBrush(RGB(255, 0, 0));
            Ellipse(hdc, marsX - marsRadius, marsY - marsRadius, marsX + marsRadius, marsY + marsRadius);
            DeleteObject(hBrushMars);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry point: WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "SolarSystemWindowClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&wc);
    
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Solar System Simulation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        hInstance,
        NULL
    );
    
    if (hwnd == NULL) {
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
    // Message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
