#include "gtest/gtest.h"
#include "../src/win32_platform.cpp"

// Тесты для функции simulate_player
TEST(SimulatePlayerTest, CorrectPositionAndVelocityUpdate) {
    float p_1 = 0.0f, p_2 = 10.0f;
    float dp_1 = 10.0f, dp_2 = 15.0f;
    simulate_player(&p_1, &dp_1, -5.0f, 0.1f);
    simulate_player(&p_2, &dp_2, 5.0f, 0.5f);
    // Проверка правильности вычисления положения игрока и его скорости
    EXPECT_FLOAT_EQ(p_1, 0.475f);
    EXPECT_FLOAT_EQ(dp_1, -0.5f);
    EXPECT_FLOAT_EQ(p_2, -0.625f);
    EXPECT_FLOAT_EQ(dp_2, -57.5f);
}

// Тесты для функции simulate_player
TEST(SimulatePlayerTest, CollisionWithUpperBoundary) {
    float p_1 = 40.0f, p_2 = 20.0f;
    float dp_1 = 10.0f, dp_2 = 5.0f;
    simulate_player(&p_1, &dp_1, 5.0f, 0.1f);
    simulate_player(&p_2, &dp_2, 5.0f, 0.1f);
    // Проверка правильности обрабатывания столкновения с верхней границей
    EXPECT_FLOAT_EQ(p_1, 33.0f);
    EXPECT_FLOAT_EQ(dp_1, 0.0f);
    EXPECT_FLOAT_EQ(p_2, 20.275f);
    EXPECT_FLOAT_EQ(dp_2, 0.5f);
}

// Тесты для функции aabb_vs_aabb
TEST(AABBvsAABBTest, BallCollisionWithPlayer) {
    bool collision_1 = aabb_vs_aabb(.0f, .0f, 1.0f, 2.0f, .0f, 2.5f, 12.0f);
    bool collision_2 = aabb_vs_aabb(10.0f, .0f, 1.0f, 13.0f, .0f, 2.1f, 12.0f);
    bool collision_3 = aabb_vs_aabb(10.0f, 14.0f, 1.0f, 13.0f, .0f, 2.1f, 12.0f);
    bool collision_4 = aabb_vs_aabb(10.0f, 8.0f, 1.0f, 14.0f, .0f, 2.1f, 12.0f);
    // Проверка на то, что столкновение мяча с игроком было обнаружено верно
    EXPECT_TRUE(collision_1);
    EXPECT_TRUE(collision_2);
    EXPECT_FALSE(collision_3);
    EXPECT_FALSE(collision_4);
}

// Вспомогательная функция для инициализации памяти состояния рендеринга
void init_render_state(int width, int height) {
    render_state.width = width;
    render_state.height = height;
    render_state.memory = new u32[width * height];
}

// Вспомогательная функция для очистки памяти состояния рендеринга
void clear_render_state() {
    delete[] static_cast<u32*>(render_state.memory);
}


// Испытательное приспособление
class RenderTest : public ::testing::Test {
protected:
    void SetUp() override {
        init_render_state(100, 100); // Initialize a 100x100 render state
    }

    void TearDown() override {
        clear_render_state();
    }
};

// Тест для функции clear_screen
TEST_F(RenderTest, TestClearScreen) {
    u32 clear_color = 0x12345678;
    clear_screen(clear_color);

    u32* pixel = static_cast<u32*>(render_state.memory);
    for (int i = 0; i < render_state.width * render_state.height; ++i) {
        ASSERT_EQ(pixel[i], clear_color);
    }
}

// Тест для функции draw_rect_in_pixels
TEST_F(RenderTest, TestDrawRectInPixels) {
    u32 color = 0x87654321;
    draw_rect_in_pixels(10, 10, 20, 20, color);

    u32* pixel = static_cast<u32*>(render_state.memory);
    for (int y = 10; y < 20; ++y) {
        for (int x = 10; x < 20; ++x) {
            ASSERT_EQ(pixel[x + y * render_state.width], color);
        }
    }
}

// Тест для функции draw_rect
TEST_F(RenderTest, TestDrawRect) {
    u32 color = 0xabcdef12;
    draw_rect(0.0f, 0.0f, 10.0f, 10.0f, color);

    u32* pixel = static_cast<u32*>(render_state.memory);
    int x0 = 40;
    int x1 = 60;
    int y0 = 40;
    int y1 = 60;
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            ASSERT_EQ(pixel[x + y * render_state.width], color);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
