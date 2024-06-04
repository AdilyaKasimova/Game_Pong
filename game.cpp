#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp; // Положение и скорость
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;

internal void 
simulate_game(Input* input, float dt){
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // Арена

	float player_1_ddp = 0.f; // Ускорение
	if (is_down(BUTTON_UP)) player_1_ddp += 1000;
	if (is_down(BUTTON_DOWN)) player_1_ddp -= 1000;

	float player_2_ddp = 0.f;
	if (is_down(BUTTON_W)) player_2_ddp += 1000;
	if (is_down(BUTTON_S)) player_2_ddp -= 1000;

	player_1_ddp -= player_1_dp * 10.f; // Сила трения
	player_1_p = player_1_p + player_1_dp * dt + player_1_ddp * dt * dt * .5f; // Уравнения для равноускоренного движения
	player_1_dp = player_1_dp + player_1_ddp * dt;

	if (player_1_p + player_half_size_y > arena_half_size_y) { // Столкновение с верхней границей арены 
		player_1_p = arena_half_size_y - player_half_size_y;
		player_1_dp = 0;
	}
	else if (player_1_p - player_half_size_y < -arena_half_size_y) { // Столкновение с нижней границей арены 
		player_1_p = -arena_half_size_y + player_half_size_y;
		player_1_dp = 0;
	}

	player_2_ddp -= player_2_dp * 10.f; 
	player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f;
	player_2_dp = player_2_dp + player_2_ddp * dt;

	if (player_2_p + player_half_size_y > arena_half_size_y) { // Столкновение с верхней границей арены 
		player_2_p = arena_half_size_y - player_half_size_y;
		player_2_dp = 0;
	}
	else if (player_2_p - player_half_size_y < -arena_half_size_y) { // Столкновение с нижней границей арены 
		player_2_p = -arena_half_size_y + player_half_size_y;
		player_2_dp = 0;
	}

	draw_rect(0, 0, 1, 1, 0xffffff); // Мяч

	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); // Игрок правый
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // Игрок левый

}