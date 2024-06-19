#include "board_init.h"
#include "screen.h"
#include "tx_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BOARD_WIDTH 128
#define BOARD_HEIGHT 64
#define DINO_HEIGHT 10
#define DINO_WIDTH 10
#define MAX_OBSTACLE_WIDTH 5
#define OBSTACLE_HEIGHT 10
#define GRAVITY 1
#define JUMP_STRENGTH -10
#define GROUND_LEVEL (BOARD_HEIGHT - DINO_HEIGHT)
#define INITIAL_OBSTACLE_SPEED 2
#define SPEED_INCREMENT 1
#define DEBOUNCE_DELAY 50
#define GAME_LOOP_DELAY 30

typedef struct {
    int x, y;
    int width, height;
    int dy;
} Dinosaur;

typedef struct {
    int x, y;
    int width, height;
    int is_flying;
} Obstacle;

Dinosaur dino;
Obstacle obstacle;
int score = 0;
int is_jumping = 0;
int obstacle_speed = INITIAL_OBSTACLE_SPEED;

void clear_board() {
    ssd1306_Fill(Black);
}

void draw_dino(Dinosaur *d) {
    // Simple T-Rex shape (adjust to fit your display)
    static const char *t_rex[] = {
        "  ##  ",
        " #### ",
        "######",
        "## ## ",
        "  ##  ",
    };

    for (int i = 0; i < DINO_HEIGHT / 2; i++) {
        for (int j = 0; j < DINO_WIDTH; j++) {
            if (t_rex[i][j] == '#') {
                ssd1306_DrawPixel(d->x + j, d->y + i, White);
            }
        }
    }
}

void draw_obstacle(Obstacle *o) {
    for (int i = 0; i < o->height; i++) {
        for (int j = 0; j < o->width; j++) {
            ssd1306_DrawPixel(o->x + j, o->y + i, White);
        }
    }
}

void draw_score() {
    char score_str[16];
    sprintf(score_str, "Score: %d", score);
    ssd1306_SetCursor(0, 0); // Start score display at the left side
    ssd1306_WriteString(score_str, Font_11x18, White); // Use a smaller font
}

void draw_board() {
    clear_board();
    draw_dino(&dino);
    draw_obstacle(&obstacle);
    draw_score();
    ssd1306_UpdateScreen();
}

void jump(Dinosaur *d) {
    if (!is_jumping) {
        d->dy = JUMP_STRENGTH;
        is_jumping = 1;
    }
}

void update_dino(Dinosaur *d) {
    d->y += d->dy;
    d->dy += GRAVITY;

    if (d->y >= GROUND_LEVEL) {
        d->y = GROUND_LEVEL;
        d->dy = 0;
        is_jumping = 0;
    }
}

void update_obstacle(Obstacle *o) {
    o->x -= obstacle_speed;

    if (o->x < 0) {
        o->x = BOARD_WIDTH;
        o->width = 5 + rand() % (MAX_OBSTACLE_WIDTH - 5 + 1); // Random width between 5 and MAX_OBSTACLE_WIDTH
        o->is_flying = rand() % 2; // Randomly determine if the obstacle is flying
        o->y = o->is_flying ? (GROUND_LEVEL - DINO_HEIGHT - 5) : GROUND_LEVEL;
        score++;
    }
}

int check_collision(Dinosaur *d, Obstacle *o) {
    return (d->x < o->x + o->width && d->x + d->width > o->x &&
            d->y < o->y + o->height && d->y + d->height > o->y);
}

void display_message(const char *message, int show_score) {
    clear_board();
    int message_x = (BOARD_WIDTH - strlen(message) * 11) / 2; // Center the message horizontally
    int message_y = (BOARD_HEIGHT - 18) / 2 - 9; // Center the message vertically, adjusting for font height

    ssd1306_SetCursor(message_x, message_y); 
    ssd1306_WriteString((char*)message, Font_11x18, White); // Cast to char*

    if (show_score) {
        char score_str[16];
        sprintf(score_str, "Score: %d", score);
        int score_x = 0; // Start the score display at the left side
        int score_y = message_y + 18; // Position the score just below the message
        ssd1306_SetCursor(score_x, score_y); 
        ssd1306_WriteString(score_str, Font_11x18, White);
    }

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
    const char pre_game_message[] = "IoT Tuesday";
    display_message(pre_game_message, 0);
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2); // Wait for 2 seconds

    // Initialize dinosaur
    dino.x = 10;
    dino.y = GROUND_LEVEL;
    dino.width = DINO_WIDTH;
    dino.height = DINO_HEIGHT;
    dino.dy = 0;

    // Initialize obstacle
    obstacle.x = BOARD_WIDTH;
    obstacle.y = GROUND_LEVEL;
    obstacle.width = 5 + rand() % (MAX_OBSTACLE_WIDTH - 5 + 1); // Random width between 5 and MAX_OBSTACLE_WIDTH
    obstacle.height = OBSTACLE_HEIGHT;
    obstacle.is_flying = rand() % 2; // Randomly determine if the obstacle is flying
    if (obstacle.is_flying) {
        obstacle.y = GROUND_LEVEL - DINO_HEIGHT - 5; // Adjust y-position for flying obstacle
    }

    int prev_button_a_state = 0;
    int prev_button_b_state = 0;

    while (1)
    {
        int button_a_state = BUTTON_A_IS_PRESSED;
        int button_b_state = BUTTON_B_IS_PRESSED;

        if (button_a_state && !prev_button_a_state) {
            jump(&dino);
            tx_thread_sleep(DEBOUNCE_DELAY);
        }

        if (button_b_state && !prev_button_b_state) {
            obstacle_speed += SPEED_INCREMENT; // Increase the obstacle speed
            tx_thread_sleep(DEBOUNCE_DELAY);
        }

        prev_button_a_state = button_a_state;
        prev_button_b_state = button_b_state;

        update_dino(&dino);
        update_obstacle(&obstacle);

        if (check_collision(&dino, &obstacle)) {
            // Game over
            display_message("Game Over", 1);
            while (!BUTTON_B_IS_PRESSED) {
                tx_thread_sleep(10);
            }
            tx_thread_sleep(DEBOUNCE_DELAY); // Debounce after reset
            score = 0;
            dino.y = GROUND_LEVEL;
            obstacle.x = BOARD_WIDTH;
            obstacle_speed = INITIAL_OBSTACLE_SPEED;
            display_message(pre_game_message, 0);
            tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2); // Wait for 2 seconds
        }

        draw_board();
        tx_thread_sleep(GAME_LOOP_DELAY); // Adjust the delay for game speed
    }

    return 0;
}
