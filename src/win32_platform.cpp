#define process_button(b, vk) case vk: {input.buttons[b].is_down = is_down; input.buttons[b].changed = true;} break;

#include "utils.cpp"
#include <windows.h>

// Здесь прописано создание игрового окна

static bool running = true;

/**
 * \brief Structure representing the rendering state.
 */
struct Render_State {
    int height, width; /**< Height and width of the render. */
    void *memory; /**< Pointer to render memory. */
    BITMAPINFO bitmap_info; /**< Bitmap information. */
};

static Render_State render_state;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

/**
 * \brief Window callback procedure.
 *
 * \param hwnd The window handle.
 * \param uMsg The window message.
 * \param wParam Additional message data.
 * \param lParam Additional message data.
 * \return The result of message processing.
 *
 * Window procedure callback. Handles window messages such as window closure and resizing.
 */
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) {
    // Это окно обратного вызова, которое обрабатывает сообщения
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: { // Закрытие игры
            running = false;
        }
            break;

        case WM_SIZE: { // Здесь происходит учет размера окна
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;

            int size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory)
                VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(
                    0,
                    size,
                    MEM_COMMIT | MEM_RESERVE,
                    PAGE_READWRITE
            );

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
        }
            break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

/**
 * \brief Windows application entry point.
 *
 * \param hInstance The application instance handle.
 * \param hPrevInstance Unused handle to the previous instance.
 * \param lpCmdLine Command line arguments.
 * \param nShowCmd Flags for displaying the window.
 * \return The application return code.
 *
 * WinMain function serves as the entry point for the Windows application. It creates a window, handles input,
 * and calls functions for simulation and rendering.
 */
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    ShowCursor(FALSE); // Прячем курсор

    // Create Window Class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = TEXT("Game Window Class");
    window_class.lpfnWndProc = window_callback;

    // Register Class
    RegisterClass(&window_class);

    // Create Window
    HWND window = CreateWindow(window_class.lpszClassName, TEXT("Pong"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

    HDC hdc = GetDC(window);

    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency; // Функция частоты обновления запросов, она возвращает количество циклов работы процессора, которые можно выполнить за один 2-ой цикл
    LARGE_INTEGER perf;
    QueryPerformanceFrequency(&perf);
    performance_frequency = (float) perf.QuadPart;

    while (running) {
        // Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) // Обнуляем значение changed в начале каждого кадра
            input.buttons[i].changed = false;

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) { // Обрабатывааем события ввода
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32) message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);
                    switch (vk_code) {
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_W, 'W');
                        process_button(BUTTON_S, 'S');
                        process_button(BUTTON_LEFT, VK_LEFT);
                        process_button(BUTTON_RIGHT, VK_RIGHT);
                        process_button(BUTTON_ENTER, VK_RETURN);
                        process_button(BUTTON_ESCAPE, VK_ESCAPE);
                    }
                }
                    break;

                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }

        // Simulate
        simulate_game(&input, delta_time);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width,
                      render_state.height, 0, 0, render_state.width,
                      render_state.height, render_state.memory, &render_state.bitmap_info,
                      DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        // Вычислим скорость, выраженную в секундах на кадр
        delta_time = (float) (frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }
    return 0;
}