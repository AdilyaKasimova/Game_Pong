#include "gtest/gtest.h"
#include "../src/win32_platform.cpp"

TEST(SimulatePlayerTest, CorrectPositionAndVelocityUpdate) {
    float p = 0.0f;
    float dp = 10.0f;
    float ddp = -5.0f;
    float dt = 0.1f;

    simulate_player(&p, &dp, ddp, dt);

    // Check if position and velocity are updated correctly
    EXPECT_FLOAT_EQ(p, 0.475f);
    EXPECT_FLOAT_EQ(dp, -0.5f);
}

TEST(SimulatePlayerTest, CollisionWithUpperBoundary) {
    float p = 40.0f;
    float dp = 10.0f;
    float ddp = 5.0f;
    float dt = 0.1f;

    simulate_player(&p, &dp, ddp, dt);

    // Check if collision with upper boundary is handled correctly
    EXPECT_FLOAT_EQ(p, 33.0f); // Expected position after collision
    EXPECT_FLOAT_EQ(dp, 0.0f); // Velocity should be reset to 0
}

// Tests for aabb_vs_aabb function
TEST(AABBvsAABBTest, BallCollisionWithPlayer) {
    float ball_p_x = 0.0f;
    float ball_p_y = 0.0f;
    float ball_hs = 1.0f;
    float player_p_x = 2.0f;
    float player_p_y = 0.0f;
    float player_hs_x = 2.5f;
    float player_hs_y = 12.0f;

    bool collision = aabb_vs_aabb(ball_p_x, ball_p_y, ball_hs, player_p_x, player_p_y, player_hs_x, player_hs_y);

    // Check if collision with player is detected correctly
    EXPECT_TRUE(collision);
}

// Add more tests for other aspects of aabb_vs_aabb function if needed

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
