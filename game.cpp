#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp; // player_p = положение, player_dp = скорость
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;

internal void 
simulate_game(Input* input, float dt){
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // Арена

	float player_1_ddp = 0.f; // player_ddp = ускорение
	if (is_down(BUTTON_UP)) player_1_ddp += 1500;
	if (is_down(BUTTON_DOWN)) player_1_ddp -= 1500;

	float player_2_ddp = 0.f;
	if (is_down(BUTTON_W)) player_2_ddp += 1500;
	if (is_down(BUTTON_S)) player_2_ddp -= 1500;

	player_1_ddp -= player_1_dp * 10.f; // Сила трения
	player_1_p = player_1_p + player_1_dp * dt + player_1_ddp * dt * dt * .5f; // Уравнения для равноускоренного движения
	player_1_dp += player_1_ddp * dt;

	if (player_1_p + player_half_size_y > arena_half_size_y) { // Столкновение правого игрока с верхней границей арены 
		player_1_p = arena_half_size_y - player_half_size_y;
		player_1_dp = 0;
	}
	else if (player_1_p - player_half_size_y < -arena_half_size_y) { // Столкновение правого игрока с нижней границей арены 
		player_1_p = -arena_half_size_y + player_half_size_y;
		player_1_dp = 0;
	}

	player_2_ddp -= player_2_dp * 10.f; 
	player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f;
	player_2_dp += player_2_ddp * dt;

	if (player_2_p + player_half_size_y > arena_half_size_y) { // Столкновение левого игрока с верхней границей арены 
		player_2_p = arena_half_size_y - player_half_size_y;
		player_2_dp = 0;
	}
	else if (player_2_p - player_half_size_y < -arena_half_size_y) { // Столкновение левого игрока с нижней границей арены 
		player_2_p = -arena_half_size_y + player_half_size_y;
		player_2_dp = 0;
	}

	ball_p_x += ball_dp_x * dt;
	ball_p_y += ball_dp_y * dt;

	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff); // Мяч

	if (ball_p_x + ball_half_size > 80 - player_half_size_x && 
		ball_p_x - ball_half_size < 80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
		ball_p_y + ball_half_size < player_1_p + player_half_size_y) { // Столкновение мяча с правым игроком
		ball_p_x = 80 - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
	}
	else if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
		ball_p_x - ball_half_size < -80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
		ball_p_y + ball_half_size < player_2_p + player_half_size_y) { // Столкновение мяча с левым игроком
		ball_p_x = -80 + player_half_size_x + ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * .75f;
	}

	if (ball_p_y + ball_half_size > arena_half_size_y) { // Столкновение мяча с верхней границей арены
		ball_p_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}

	if (ball_p_y - ball_half_size < -arena_half_size_y) { // Столкновение мяча с нижней границей арены
		ball_p_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}

	if (ball_p_x + ball_half_size > arena_half_size_x) { // Столкновение мяча с правой границей арены
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}
	else if (ball_p_x - ball_half_size < -arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}

	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); // Игрок правый
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Игрок левый

}