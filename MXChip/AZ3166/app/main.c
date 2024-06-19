#include "board_init.h"
#include "screen.h"
#include "tx_api.h" 
#include <stdio.h>

#define DEBOUNCE_DELAY 50 

void display_counts(int count_a, int count_b)
{
    char count_a_str[16];
    char count_b_str[16];

    sprintf(count_a_str, "CountA: %d", count_a);
    sprintf(count_b_str, "CountB: %d", count_b);

    screen_printl(count_a_str, count_b_str, L1, L2);
}

int main(void)
{
    board_init();

    int count_a = 0;
    int count_b = 0;

    int prev_button_a_state = 0;
    int prev_button_b_state = 0;

    while (1)
    {
        int button_a_state = BUTTON_A_IS_PRESSED;
        int button_b_state = BUTTON_B_IS_PRESSED;

        if (button_a_state && !prev_button_a_state)
        {
            count_a++;
            tx_thread_sleep(DEBOUNCE_DELAY);  
        }

        if (button_b_state && !prev_button_b_state)
        {
            count_b++;
            tx_thread_sleep(DEBOUNCE_DELAY);  
        }

        prev_button_a_state = button_a_state;
        prev_button_b_state = button_b_state;

        display_counts(count_a, count_b);
        screenUpdate();

        tx_thread_sleep(10); // Small delay to avoid tight loop
    }

    return 0;
}
