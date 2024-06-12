// Здесь прописаны средства визуализации

/**
 * \brief Clears the screen with a specified color.
 *
 * \param color The color to fill the screen with in a 32-bit unsigned integer format.
 *
 * This function fills the entire screen with the specified color. It iterates over each pixel
 * in the render state's memory and sets its color to the provided value.
 */
void clear_screen(u32 color) {
    u32 *pixel = (u32 *) render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}

/**
 * \brief Draws a filled rectangle in pixel coordinates on the screen.
 *
 * \param x0 The X-coordinate of the left edge of the rectangle.
 * \param y0 The Y-coordinate of the top edge of the rectangle.
 * \param x1 The X-coordinate of the right edge of the rectangle.
 * \param y1 The Y-coordinate of the bottom edge of the rectangle.
 * \param color The color of the rectangle in a 32-bit unsigned integer format.
 *
 * This function draws a filled rectangle on the screen using pixel coordinates. The coordinates
 * are clamped to ensure they are within the bounds of the screen. The rectangle is then drawn by
 * setting the color of the pixels within the specified bounds.
 */
void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {

    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);


    for (int y = y0; y < y1; y++) {
        u32 *pixel = (u32 *) render_state.memory + x0 + y * render_state.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

static float render_scale = 0.01f;

/**
 * \brief Draws a rectangle on the screen.
 *
 * \param x X-coordinate of the center of the rectangle.
 * \param y Y-coordinate of the center of the rectangle.
 * \param half_size_x Half of the rectangle's width.
 * \param half_size_y Half of the rectangle's height.
 * \param color Color of the rectangle in a 32-bit unsigned integer format.
 *
 * This function draws a rectangle on the screen, centered at (x, y) with the specified half-sizes.
 * The coordinates and sizes are scaled based on the current render state and then converted to
 * pixel values. The rectangle is drawn using the `draw_rect_in_pixels` function, which takes the
 * pixel coordinates of the rectangle's corners and the color.
 */
void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
    x *= render_state.height * render_scale;
    y *= render_state.height * render_scale;
    half_size_x *= render_state.height * render_scale;
    half_size_y *= render_state.height * render_scale;

    x += render_state.width / 2.f;
    y += render_state.height / 2.f;

    // Change to pixels
    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    draw_rect_in_pixels(x0, y0, x1, y1, color);
}

const char *letters[][7] = {
        " 00",
        "0  0",
        "0  0",
        "0000",
        "0  0",
        "0  0",
        "0  0",

        "000",
        "0  0",
        "0  0",
        "000",
        "0  0",
        "0  0",
        "000",

        " 000",
        "0",
        "0",
        "0",
        "0",
        "0",
        " 000",

        "000",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "000",

        "0000",
        "0",
        "0",
        "0000",
        "0",
        "0",
        "0000",

        "0000",
        "0",
        "0",
        "000",
        "0",
        "0",
        "0",

        " 00",
        "0  0",
        "0",
        "0 00",
        "0  0",
        "0  0",
        " 00",

        "0  0",
        "0  0",
        "0  0",
        "0000",
        "0  0",
        "0  0",
        "0  0",

        "000",
        " 0",
        " 0",
        " 0",
        " 0",
        " 0",
        "000",

        "  00",
        "   0",
        "   0",
        "   0",
        "   0",
        "0  0",
        " 00",

        "0  0",
        "0  0",
        "0 0",
        "00",
        "0 0",
        "0  0",
        "0  0",

        "0",
        "0",
        "0",
        "0",
        "0",
        "0",
        " 000",

        "0   0",
        "0   0",
        "00 00",
        "0 0 0",
        "0   0",
        "0   0",
        "0   0",

        "0   0",
        "0   0",
        "00  0",
        "0 0 0",
        "0  00",
        "0   0",
        "0   0",

        " 00",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        " 00",

        "000",
        "0  0",
        "0  0",
        "000",
        "0",
        "0",
        "0",

        " 00",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0 00",
        " 00 0",

        "000",
        "0  0",
        "0  0",
        "000",
        "0 0",
        "0  0",
        "0  0",

        " 000",
        "0",
        "0",
        " 00",
        "   0",
        "   0",
        "000",

        "00000",
        "  0",
        "  0",
        "  0",
        "  0",
        "  0",
        "  0",

        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        "0  0",
        " 00",

        "0   0",
        "0   0",
        "0   0",
        " 0 0",
        " 0 0",
        " 0 0",
        "  0",

        "0   0",
        "0   0",
        "0   0",
        "0   0",
        "0 0 0",
        "0 0 0",
        "00 00",

        "0   0",
        "0   0",
        " 0 0",
        "  0",
        " 0 0",
        "0   0",
        "0   0",

        "0   0",
        "0   0",
        " 0 0",
        "  0",
        "  0",
        "  0",
        "  0",

        "00000",
        "    0",
        "   0",
        "  0",
        " 0",
        "0",
        "00000",
};

/**
 * \brief Draws text on the screen.
 *
 * \param text Pointer to the string containing the text to be drawn.
 * \param x X-coordinate of the starting position of the text.
 * \param y Y-coordinate of the starting position of the text.
 * \param size Size of the text to be drawn.
 * \param color The color of the text in a 32-bit unsigned integer format.
 *
 * This function draws text on the screen starting from the specified position (x, y). It iterates
 * over each character in the input string and draws its corresponding representation using
 * rectangles. The size parameter determines the scale of the text, and the color parameter sets
 * the color of the text.
 */
void draw_text(const char *text, float x, float y, float size, u32 color) { // Прорисовка надписи
    float half_size = size * .5f;
    float original_y = y;

    while (*text) {
        if (*text != 32) {
            const char **a_letter = letters[*text - 'A'];
            float original_x = x;

            for (int i = 0; i < 7; i++) {
                const char *row = a_letter[i];
                while (*row) {
                    if (*row == '0') draw_rect(x, y, half_size, half_size, color);
                    x += size;
                    row++;
                }
                y -= size;
                x = original_x;
            }
        }
        text++;
        x += size * 6.f;
        y = original_y;
    }
}

/**
 * \brief Draws a number on the screen.
 *
 * \param number The number to be drawn.
 * \param x X-coordinate of the starting position of the number.
 * \param y Y-coordinate of the starting position of the number.
 * \param size Size of the number to be drawn.
 * \param color The color of the number in a 32-bit unsigned integer format.
 *
 * This function draws a number on the screen starting from the specified position (x, y). It
 * iterates over each digit of the input number and draws its representation using rectangles.
 * The size parameter determines the scale of the number, and the color parameter sets the color
 * of the number.
 */
void draw_number(int number, float x, float y, float size, u32 color) {
    float half_size = size * .5f;

    bool drew_number = false;
    while (number || !drew_number) {
        drew_number = true;
        int digit = number % 10;
        number = number / 10;
        switch (digit) {
            case 0:
                draw_rect(x - size, y, half_size, 2.5f * size, color);
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                draw_rect(x, y + size * 2.f, half_size, half_size, color);
                draw_rect(x, y - size * 2.f, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 1:
                draw_rect(x + size, y, half_size, 2.5f * size, color);
                x -= 4.f * size;
                break;

            case 2:
                draw_rect(x, y + size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x, y, size * 1.5f, half_size, color);
                draw_rect(x, y - size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x + size, y + size, half_size, half_size, color);
                draw_rect(x - size, y - size, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 3:
                draw_rect(x, y + size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x, y, size * 1.5f, half_size, color);
                draw_rect(x, y - size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x + size, y + size, half_size, half_size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 4:
                draw_rect(x, y, half_size, half_size, color);
                draw_rect(x - size, y + size, half_size, size * 1.5f, color);
                draw_rect(x + size, y, half_size, size * 2.5f, color);
                x -= 4.f * size;
                break;

            case 5:
                draw_rect(x, y + size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x, y, size * 1.5f, half_size, color);
                draw_rect(x, y - size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x - size, y + size, half_size, half_size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 6:
                draw_rect(x, y + size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x, y, size * 1.5f, half_size, color);
                draw_rect(x, y - size * 2.f, size * 1.5f, half_size, color);
                draw_rect(x - size, y + size, half_size, half_size, color);
                draw_rect(x + size, y - size, half_size, half_size, color);
                draw_rect(x - size, y - size, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 7:
                draw_rect(x + size, y, half_size, size * 2.5f, color);
                draw_rect(x - half_size, y + 2.f * size, size, half_size, color);
                x -= 4.f * size;
                break;

            case 8:
                draw_rect(x + size, y, half_size, size * 2.5f, color);
                draw_rect(x - size, y, half_size, size * 2.5f, color);
                draw_rect(x, y + 2.f * size, half_size, half_size, color);
                draw_rect(x, y, half_size, half_size, color);
                draw_rect(x, y - 2.f * size, half_size, half_size, color);
                x -= 4.f * size;
                break;

            case 9:
                draw_rect(x + size, y, half_size, size * 2.5f, color);
                draw_rect(x, y + 2.f * size, half_size, half_size, color);
                draw_rect(x, y, half_size, half_size, color);
                draw_rect(x - half_size, y - 2.f * size, size, half_size, color);
                draw_rect(x - size, y + size, half_size, size * 1.5f, color);
                x -= 4.f * size;
                break;
        }
    }
}