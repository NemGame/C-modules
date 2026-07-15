#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>

/* IMPORTANT: HOW TO USE
 * 1. Create a Canvas (e.g. `Canvas canvas(800, 600, L"My Canvas", Canvas::Color::Transparent);`)
 * 2. You must call `canvas.keepRunning()` in a loop to keep the window open and responsive. (e.g. `while (canvas.keepRunning()) { ... }`)
 * 3. You must call `canvas.drawAll()` to update the window with the current pixel buffer. (e.g. `canvas.drawAll();`)
 * You can use the `Canvas::Color` struct to access predefined colors or create your own using the provided static methods.
 * 
 * === VERY IMPORTANT!!! ===
 * You must link GDI32.lib on Windows and X11 libraries on Linux when compiling this project.
 * Examples:
 *   Windows: g++ -std=c++17 -o my_program test.cpp -lgdi32
 *   Linux: g++ -std=c++17 -o my_program test.cpp -lX11
 * 
 * If used on any other platform, the code will throw an error during compilation and define the macro `CANVAS_ERROR_UNSUPPORTED_PLATFORM`.
 * 
 * Example program:
```cpp
    #include "canvas.hpp"

    #include <iostream>

    int main() {
        Canvas canvas(800, 600, L"My Canvas", Canvas::Color::Transparent);
        for(int x = 0; x < 800; ++x) {
            canvas.drawPixel(x, 300, Canvas::Color::Green); // Green horizon line
        }
        canvas.drawLine(0, 1, 500, 300, Canvas::Color::Red); // Red diagonal line

        canvas.drawPixel(400, 300, Canvas::Color::Blue); // Blue center pixel

        std::wcout << L"Canvas Version: " << CANVAS_VERSION << std::endl;

        std::cout << "Pixel at (400, 300): 0x" << std::hex << canvas.getPixel(400, 300) << std::dec << std::endl;
        // Main loop
        while (canvas.keepRunning()) {
            canvas.drawAll();
        }

        return 0;
    }
```
*/
#define CANVAS_VERSION Canvas::version

#if not defined(_WIN32) && not defined(__linux__)
    #error "The canvas.hpp library only supports Windows and Linux platforms."
    #define CANVAS_ERROR_UNSUPPORTED_PLATFORM
#endif

// Platform-specific headers
#if defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #pragma comment(lib, "gdi32.lib")
#elif defined(__linux__)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <sys/time.h>
#endif

namespace std {
    template<typename T>
    T clamp(T v, T lo, T hi) {
        return (v < lo) ? lo : (v > hi) ? hi : v;
    }
}

class Canvas {
    public:
    // Uses ARGB format for colors (0xAARRGGBB)
    struct Color {
        Color() = delete; // Prevent instantiation of the Color struct
        template<typename T>
        Color(const T&) = delete; // Prevent instantiation of the Color struct
        static constexpr uint32_t Red   = 0xFFFF0000;
        static constexpr uint32_t Green = 0xFF00FF00;
        static constexpr uint32_t Blue  = 0xFF0000FF;
        static constexpr uint32_t White = 0xFFFFFFFF;
        static constexpr uint32_t Black = 0xFF000000;
        static constexpr uint32_t Yellow = 0xFFFFFF00;
        static constexpr uint32_t Cyan = 0xFF00FFFF;
        static constexpr uint32_t Magenta = 0xFFFF00FF;
        static constexpr uint32_t Transparent = 0x00000000;
        inline static const uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
            return (static_cast<uint32_t>(a) << 24) |
                   (static_cast<uint32_t>(r) << 16) |
                   (static_cast<uint32_t>(g) << 8) |
                   static_cast<uint32_t>(b);
        }
        inline static const uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
            return rgba(r, g, b, 255);
        }
        inline static const uint32_t argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
            return (static_cast<uint32_t>(a) << 24) |
                   (static_cast<uint32_t>(r) << 16) |
                   (static_cast<uint32_t>(g) << 8) |
                   static_cast<uint32_t>(b);
        }
        inline static const uint32_t hsl(float h, float s, float l) {
            float c = (1 - std::abs(2 * l - 1)) * s;
            float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
            float m = l - c / 2;

            float r = 0, g = 0, b = 0;

            if (h >= 0 && h < 60) {
                r = c; g = x; b = 0;
            } else if (h >= 60 && h < 120) {
                r = x; g = c; b = 0;
            } else if (h >= 120 && h < 180) {
                r = 0; g = c; b = x;
            } else if (h >= 180 && h < 240) {
                r = 0; g = x; b = c;
            } else if (h >= 240 && h < 300) {
                r = x; g = 0; b = c;
            } else if (h >= 300 && h < 360) {
                r = c; g = 0; b = x;
            }

            return rgba(static_cast<uint8_t>((r + m) * 255),
                        static_cast<uint8_t>((g + m) * 255),
                        static_cast<uint8_t>((b + m) * 255));
        }
        inline static const uint32_t hsla(float h, float s, float l, float a) {
            return (static_cast<uint32_t>(a * 255) << 24) | (hsl(h, s, l) & 0x00FFFFFF);
        }
        inline static const uint32_t mix(uint32_t color1, uint32_t color2, float t) {
            t = std::clamp(t, 0.0f, 1.0f);
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            uint8_t a = static_cast<uint8_t>(a1 + t * (a2 - a1));
            uint8_t r = static_cast<uint8_t>(r1 + t * (r2 - r1));
            uint8_t g = static_cast<uint8_t>(g1 + t * (g2 - g1));
            uint8_t b = static_cast<uint8_t>(b1 + t * (b2 - b1));

            return rgba(r, g, b, a);
        }
        inline static const uint32_t invert(uint32_t color) {
            uint8_t a = (color >> 24) & 0xFF;
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            return rgba(255 - r, 255 - g, 255 - b, a);
        }
        inline static const uint32_t grayscale(uint32_t color) {
            uint8_t a = (color >> 24) & 0xFF;
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            uint8_t gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
            return rgba(gray, gray, gray, a);
        }
        inline static const uint32_t blend(uint32_t color1, uint32_t color2) {
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            // Simple average blending
            uint8_t a = static_cast<uint8_t>((a1 + a2) / 2);
            uint8_t r = static_cast<uint8_t>((r1 + r2) / 2);
            uint8_t g = static_cast<uint8_t>((g1 + g2) / 2);
            uint8_t b = static_cast<uint8_t>((b1 + b2) / 2);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t add(uint32_t color1, uint32_t color2) {
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            // Additive blending with clamping
            uint8_t a = std::min(255, a1 + a2);
            uint8_t r = std::min(255, r1 + r2);
            uint8_t g = std::min(255, g1 + g2);
            uint8_t b = std::min(255, b1 + b2);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t subtract(uint32_t color1, uint32_t color2) {
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            // Subtractive blending with clamping
            uint8_t a = std::max(0, a1 - a2);
            uint8_t r = std::max(0, r1 - r2);
            uint8_t g = std::max(0, g1 - g2);
            uint8_t b = std::max(0, b1 - b2);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t multiply(uint32_t color1, uint32_t color2) {
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            // Multiplicative blending
            uint8_t a = static_cast<uint8_t>((a1 * a2) / 255);
            uint8_t r = static_cast<uint8_t>((r1 * r2) / 255);
            uint8_t g = static_cast<uint8_t>((g1 * g2) / 255);
            uint8_t b = static_cast<uint8_t>((b1 * b2) / 255);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t divide(uint32_t color1, uint32_t color2) {
            uint8_t a1 = (color1 >> 24) & 0xFF;
            uint8_t r1 = (color1 >> 16) & 0xFF;
            uint8_t g1 = (color1 >> 8) & 0xFF;
            uint8_t b1 = color1 & 0xFF;

            uint8_t a2 = (color2 >> 24) & 0xFF;
            uint8_t r2 = (color2 >> 16) & 0xFF;
            uint8_t g2 = (color2 >> 8) & 0xFF;
            uint8_t b2 = color2 & 0xFF;

            // Avoid division by zero
            auto safeDivide = [](uint8_t num, uint8_t denom) -> uint8_t {
                return denom == 0 ? 255 : std::min(255, static_cast<int>(num * 255 / denom));
            };

            uint8_t a = safeDivide(a1, a2);
            uint8_t r = safeDivide(r1, r2);
            uint8_t g = safeDivide(g1, g2);
            uint8_t b = safeDivide(b1, b2);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t lighten(uint32_t color, float factor) {
            factor = std::clamp(factor, 0.0f, 1.0f);
            uint8_t a = (color >> 24) & 0xFF;
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            r = static_cast<uint8_t>(r + (255 - r) * factor);
            g = static_cast<uint8_t>(g + (255 - g) * factor);
            b = static_cast<uint8_t>(b + (255 - b) * factor);

            return rgba(r, g, b, a);
        }
        inline static const uint32_t darken(uint32_t color, float factor) {
            factor = std::clamp(factor, 0.0f, 1.0f);
            uint8_t a = (color >> 24) & 0xFF;
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;

            r = static_cast<uint8_t>(r * (1.0f - factor));
            g = static_cast<uint8_t>(g * (1.0f - factor));
            b = static_cast<uint8_t>(b * (1.0f - factor));

            return rgba(r, g, b, a);
        }
    };
    const std::vector<uint32_t>& getPixelBuffer() const {
        return pixelBuffer;
    }
    const std::pair<int, int> getSize() const {
        return {width, height};
    }
private:
    int width;
    int height;
    std::vector<uint32_t> pixelBuffer;
    bool isRunning = true;

    // --- Platform Specific Window Members ---
#if defined(_WIN32)
    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        Canvas* canvas = reinterpret_cast<Canvas*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        
        switch (uMsg) {
            case WM_CLOSE: // User clicked the X button
                if (canvas) canvas->isRunning = false;
                DestroyWindow(hwnd);
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0); // Safely push a WM_QUIT to the loop
                return 0;
        }
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
#elif defined(__linux__)
    Display* display = nullptr;
    Window window;
    GC gc;
    Visual* visual = nullptr;
#endif

public:
    static constexpr const uint32_t version = 1;
    Canvas(int w, int h, const wchar_t* title, uint32_t defaultBackgroundColor = Canvas::Color::Black) : width(w), height(h) {
        pixelBuffer.resize(w * h, defaultBackgroundColor);
#if defined(_WIN32)// 1. Initialize Windows Window using explicit Wide-character APIs
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASSW wc = {}; // Notice the W
        wc.lpfnWndProc = Canvas::WindowProc; // Notice the W
        wc.hInstance = hInstance;
        wc.lpszClassName = L"CanvasClass"; // Use L macro for wide string literal
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassW(&wc); // Notice the W

        // Use CreateWindowExW
        hwnd = CreateWindowExW(0, L"CanvasClass", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);
        hdc = GetDC(hwnd);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

#elif defined(__linux__)// 2. Initialize X11 Window (Linux)
        display = XOpenDisplay(NULL);
        int screen = DefaultScreen(display);
        visual = DefaultVisual(display, screen);
        
        window = XCreateSimpleWindow(display, RootWindow(display, screen), 
                                     0, 0, w, h, 1, 
                                     BlackPixel(display, screen), BlackPixel(display, screen));
        
        // Convert wchar_t* to standard char* for X11 compatibility
        size_t titleLen = std::wcslen(title) + 1;
        std::vector<char> narrowTitle(titleLen);
        std::wcstombs(narrowTitle.data(), title, titleLen);

        XStoreName(display, window, narrowTitle.data());
        XSelectInput(display, window, ExposureMask | KeyPressMask);
        XMapWindow(display, window);
        gc = XCreateGC(display, window, 0, NULL);
#endif
    }

    ~Canvas() {
#if defined(_WIN32)
        if (hdc) ReleaseDC(hwnd, hdc);
#elif defined(__linux__)
        XFreeGC(display, gc);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
#endif
    }

    uint32_t getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixelBuffer[y * width + x];
        }
        return 0xFF000000; // Return black for out-of-bounds
    }
    // --- The Core API (Stays completely identical for the user) ---
    void drawPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixelBuffer[y * width + x] = color;
        }
    }
    void drawCircle(int xc, int yc, int r, uint32_t color) {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r; // Initial decision parameter

        // Helper lambda to mirror pixels across all 8 octants
        auto plot8 = [&](int x, int y) {
            drawPixel(xc + x, yc + y, color);
            drawPixel(xc - x, yc + y, color);
            drawPixel(xc + x, yc - y, color);
            drawPixel(xc - x, yc - y, color);
            drawPixel(xc + y, yc + x, color);
            drawPixel(xc - y, yc + x, color);
            drawPixel(xc + y, yc - x, color);
            drawPixel(xc - y, yc - x, color);
        };

        plot8(x, y);

        while (y >= x) {
            x++;
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else d = d + 4 * x + 6;
            plot8(x, y);
        }
    }
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);

        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;

        int err = dx - dy;

        while (true) {
            drawPixel(x1, y1, color);

            if (x1 == x2 && y1 == y2) break;

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
    void drawRect(int x, int y, int w, int h, uint32_t color) {
        for (int i = 0; i < w; ++i) {
            drawPixel(x + i, y, color); // Top edge
            drawPixel(x + i, y + h - 1, color); // Bottom edge
        }
        for (int j = 0; j < h; ++j) {
            drawPixel(x, y + j, color); // Left edge
            drawPixel(x + w - 1, y + j, color); // Right edge
        }
    }
    void drawFilledRect(int x, int y, int w, int h, uint32_t color) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                drawPixel(x + i, y + j, color);
            }
        }
    }
    void clear(uint32_t color = 0xFF000000) {
        std::fill(pixelBuffer.begin(), pixelBuffer.end(), color);
    }

    void drawAll() {
#if defined(_WIN32)
        // Windows GDI Blit
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // Top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        StretchDIBits(hdc, 0, 0, width, height, 
                      0, 0, width, height, 
                      pixelBuffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);

#elif defined(__linux__)
        // Linux X11 Blit
        // Create an XImage wrapping our raw std::vector data
        XImage* image = XCreateImage(display, visual, 24, ZPixmap, 0, 
                                     reinterpret_cast<char*>(pixelBuffer.data()), width, height, 32, 0);
        
        XPutImage(display, window, gc, image, 0, 0, 0, 0, width, height);
        
        // Destroy the image wrapper, but NOT our vector data
        image->data = nullptr; 
        XDestroyImage(image);
#endif
    }

    bool keepRunning() {
        if (!isRunning) return false;

#if defined(_WIN32)
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return isRunning;
#elif defined(__linux__)
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);
            if (event.type == ClientMessage) { // Typical close event mapping
                isRunning = false;
                return false;
            }
        }
        return isRunning;
#endif
    }
};