#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp; // player_p = положение, player_dp = скорость
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;

internal void
simulate_player(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f; // Сила трения
	*p = *p + *dp * dt + ddp * dt * dt * .5f; // Уравнения для равноускоренного движения
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) { // Столкновение игрока с верхней границей арены 
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) { // Столкновение игрока с нижней границей арены 
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float ball_p_x, float ball_p_y, float ball_hs,
	float player_p_x, float player_p_y, float player_hs_x, float player_hs_y) { // Проверяет факт столкновения мяча с игроком
	return (ball_p_x + ball_hs > player_p_x - player_hs_x &&
		ball_p_x - ball_hs < player_p_x + player_hs_x &&
		ball_p_y + ball_hs > player_p_y - player_hs_y &&
		ball_p_y + ball_hs < player_p_y + player_hs_y);
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void 
simulate_game(Input* input, float dt){
	clear_screen(0x606C38);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xFEFAE0); // Арена

	if (current_gamemode == GM_GAMEPLAY) { // ИГРОВОЙ ПРОЦЕСС
		float player_1_ddp = 0.f; // player_ddp = ускорение

		if (!enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_1_ddp += 1500;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= 1500;
		}
		else {
			// if (ball_p_y > player_1_p + 2.f) player_1_ddp += 1300;
			// if (ball_p_y < player_1_p - 2.f) player_1_ddp -= 1300;
			player_1_ddp = (ball_p_y - player_1_p) * 100;
			if (player_1_ddp > 1300) player_1_ddp = 1300;
			if (player_1_ddp < -1300) player_1_ddp = -1300;
		}
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += 1500;
		if (is_down(BUTTON_S)) player_2_ddp -= 1500;

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);


		// Simulate Ball
		{
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;


			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) { // Столкновение мяча с правым игроком
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * .75f;
			}
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) { // Столкновение мяча с левым игроком
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
				player_2_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) { // Столкновение мяча с левой границей арены
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
		}

		draw_number(player_2_score, -10, 40, 1.f, 0xDDA15E);
		draw_number(player_1_score, 10, 40, 1.f, 0xDDA15E);


		// Rendering
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0x283618); // Мяч

		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xBC6C25); // Игрок правый (управление - стрелки вверх/вниз)
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xBC6C25); // Игрок левый (управление - W/S)
	}
	else { // МЕНЮ
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) { // Выбор режима игры
			hot_button = !hot_button;
		}

		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}
		if (hot_button == 0) { // 0 - игра против компьютера
			draw_text("SINGLE PLAYER", -81, -10, 1, 0xBC6C25);
			draw_text("MULTIPLAYER", 18, -10, 1, 0xDDA15E);
		}
		else { // 1 - игра против второго пользователя
			draw_text("SINGLE PLAYER", -81, -10, 1, 0xDDA15E);
			draw_text("MULTIPLAYER", 18, -10, 1, 0xBC6C25);
		}

		draw_text("MAIN MENU", -45, 25, 1.7, 0x283618);

	}
}