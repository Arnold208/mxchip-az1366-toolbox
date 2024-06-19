#include "board_init.h"
#include "screen.h"
#include "sensor.h"
#include "tx_api.h"
#include <stdio.h>

void delay(uint32_t delay_in_ms)
{
    // Delay for the specified time in milliseconds
    tx_thread_sleep(delay_in_ms * TX_TIMER_TICKS_PER_SECOND / 1000);
}

int main(void)
{
    // Initialize the board
    board_init();
    // Initialize the screen
    ssd1306_Init();
    screen_print("IOT Tuesday", L1);

    printf("Starting LED pulsing...\n");

    while (1)
    {
        // Gradually increase the intensity of the blue LED
        for (int i = 0; i <= 255; i++)
        {
            RGB_LED_SET_B(i);
            printf("Increasing: %d\n", i);
            delay(500); // Small delay to create the pulsing effect
        }

        // Gradually decrease the intensity of the blue LED
        for (int i = 255; i >= 0; i--)
        {
            RGB_LED_SET_B(i);
            printf("Decreasing: %d\n", i);
            delay(500); // Small delay to create the pulsing effect
        }
    }

    return 0;
}