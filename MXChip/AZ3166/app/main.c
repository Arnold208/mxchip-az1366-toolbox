#include "board_init.h"
#include "screen.h"
#include "tx_api.h" // Include the ThreadX API header

void update_screen_and_leds( char* line1,  char* line2, int red, int green, int blue)
{
    screen_printl(line1,line2, L0,L2);
    RGB_LED_SET_R(red);
    RGB_LED_SET_G(green);
    RGB_LED_SET_B(blue);
}

int main(void)
{
    board_init();

    while (1)
    {
        if (BUTTON_A_IS_PRESSED)
        {
            update_screen_and_leds("MxChip", "A", 0, 255, 0);
        }
        else if (BUTTON_B_IS_PRESSED)
        {
            update_screen_and_leds("MxChip", "B", 0, 0, 255);
        }
        else
        {
            update_screen_and_leds("MxChip", "No Press", 255, 0, 0);
        }

        tx_thread_sleep(10); // Adjust the delay as necessary
        screenUpdate(); 
    }

    return 0;
}
