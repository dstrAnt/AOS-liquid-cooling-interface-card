
#include "miosix.h"
#include "src/PumpDriver.h"

int main()
{
    PumpDriver driver;
    uint32_t channel = 0, duty = 0;
    char line[10];

    while (1)
    {
        printf("**************\nWelcome to Miosix-based computer liquid cooling interface v1.0!!\n**************\n");

        printf("Specify channel [1-2] and the duty cycle [0-100]\nIn this form: 'channel duty_cycle'\n");
        fgets(line, 10, stdin);
        sscanf(line, "%lu %lu", &channel, &duty);
        // cin >> channel >> duty;

        if (channel < 1 || channel > 2 || duty < 0 || duty > 100)
        {
            printf("Invalid input, try again\n");
            continue;
        }

        printf("received: %lu, %lu\n", channel, duty);
        driver.setChannelDuty(channel, duty);
    }
}
