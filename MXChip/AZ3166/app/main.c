#include "board_init.h"
#include "screen.h"
#include "tx_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BOARD_WIDTH 128
#define BOARD_HEIGHT 64
#define PADDLE_HEIGHT 10
#define PADDLE_WIDTH 2
#define BALL_SIZE 2
#define PADDLE_SPEED 7
#define BALL_SPEED 1
#define DEBOUNCE_DELAY 50
#define GAME_LOOP_DELAY 30

typedef struct {
    int x, y;
    int width, height;
} Paddle;

typedef struct {
    int x, y;
    int dx, dy;
} Ball;

Paddle player_paddle, ai_paddle;
Ball ball;
int score = 0;

void clear_board() {
    ssd1306_Fill(Black);
}

void draw_paddle(Paddle *paddle) {
    for (int i = 0; i < paddle->height; i++) {
        for (int j = 0; j < paddle->width; j++) {
            ssd1306_DrawPixel(paddle->x + j, paddle->y + i, White);
        }
    }
}

void draw_ball(Ball *b) {
    for (int i = 0; i < BALL_SIZE; i++) {
        for (int j = 0; j < BALL_SIZE; j++) {
            ssd1306_DrawPixel(b->x + j, b->y + i, White);
        }
    }
}

void draw_score() {
    char score_str[16];
    sprintf(score_str, "Score: %d", score);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString(score_str, Font_11x18, White); // Using default font
}

void draw_board() {
    clear_board();
    draw_paddle(&player_paddle);
    draw_paddle(&ai_paddle);
    draw_ball(&ball);
    draw_score();
    ssd1306_UpdateScreen();
}

void move_paddle_up(Paddle *paddle) {
    if (paddle->y > 0) {
        paddle->y -= PADDLE_SPEED;
    }
}

void move_paddle_down(Paddle *paddle) {
    if (paddle->y < BOARD_HEIGHT - paddle->height) {
        paddle->y += PADDLE_SPEED;
    }
}

void move_ball() {
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Ball collision with top and bottom
    if (ball.y <= 0 || ball.y >= BOARD_HEIGHT - BALL_SIZE) {
        ball.dy = -ball.dy;
    }

    // Ball collision with paddles
    if ((ball.x <= player_paddle.x + player_paddle.width && ball.y >= player_paddle.y && ball.y <= player_paddle.y + player_paddle.height) ||
        (ball.x + BALL_SIZE >= ai_paddle.x && ball.y >= ai_paddle.y && ball.y <= ai_paddle.y + ai_paddle.height)) {
        ball.dx = -ball.dx;
        if (ball.dx > 0) {  // Hit by AI paddle
            score++;
            RGB_LED_SET_R(0);   // Turn off Red
            RGB_LED_SET_G(255); // Turn on Green
            RGB_LED_SET_B(0);   // Turn off Blue
        }
    }

    // Ball out of bounds (reset)
    if (ball.x <= 0 || ball.x >= BOARD_WIDTH - BALL_SIZE) {
        if (ball.x <= 0) {  // Missed by player
            score--;
            RGB_LED_SET_R(255); // Turn on Red
            RGB_LED_SET_G(0);   // Turn off Green
            RGB_LED_SET_B(0);   // Turn off Blue
        }
        ball.x = BOARD_WIDTH / 2;
        ball.y = BOARD_HEIGHT / 2;
        ball.dx = BALL_SPEED;
        ball.dy = BALL_SPEED;
    }
}

void ai_move_paddle() {
    if (ball.y < ai_paddle.y) {
        move_paddle_up(&ai_paddle);
    } else if (ball.y > ai_paddle.y + ai_paddle.height) {
        move_paddle_down(&ai_paddle);
    }
}

void display_message(char *message) {
    clear_board();
    ssd1306_SetCursor((BOARD_WIDTH - strlen(message) * 11) / 2, BOARD_HEIGHT / 2 - 9); // Center the message
    ssd1306_WriteString(message, Font_11x18, White);
    ssd1306_UpdateScreen();
}

int _gettimeofday(struct timeval *tv, struct timezone *tz) {
    return 0;
}

int main(void)
{
    board_init();
    srand(time(NULL));

    // Display pre-game message
    char pre_game_message[] = "IoT Tuesday";
    display_message(pre_game_message);
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2); // Wait for 2 seconds

    // Initialize paddles
    player_paddle.x = 0;
    player_paddle.y = BOARD_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    player_paddle.width = PADDLE_WIDTH;
    player_paddle.height = PADDLE_HEIGHT;

    ai_paddle.x = BOARD_WIDTH - PADDLE_WIDTH;
    ai_paddle.y = BOARD_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    ai_paddle.width = PADDLE_WIDTH;
    ai_paddle.height = PADDLE_HEIGHT;

    // Initialize ball
    ball.x = BOARD_WIDTH / 2;
    ball.y = BOARD_HEIGHT / 2;
    ball.dx = BALL_SPEED;
    ball.dy = BALL_SPEED;

    int prev_button_a_state = 0;
    int prev_button_b_state = 0;

    while (1)
    {
        int button_a_state = BUTTON_A_IS_PRESSED;
        int button_b_state = BUTTON_B_IS_PRESSED;

        if (button_a_state && !prev_button_a_state)
        {
            move_paddle_up(&player_paddle);
            tx_thread_sleep(DEBOUNCE_DELAY);
        }

        if (button_b_state && !prev_button_b_state)
        {
            move_paddle_down(&player_paddle);
            tx_thread_sleep(DEBOUNCE_DELAY);
        }

        prev_button_a_state = button_a_state;
        prev_button_b_state = button_b_state;

        move_ball();
        ai_move_paddle();
        draw_board();

        tx_thread_sleep(GAME_LOOP_DELAY); // Adjust the delay for game speed
    }

    return 0;
}
