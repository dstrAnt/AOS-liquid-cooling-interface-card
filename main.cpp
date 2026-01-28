
#include "miosix.h"
#include "src/PumpDriver.h"

void welcome()
{
    printf("+------------------------------------------------------------+\n");
    printf("|  Welcome to Computer liquid-cooling interface card v1.0!   |\n");
    printf("+------------------------------------------------------------+\n\n");
}

void usage()
{
    printf("Usage: channel duty_cycle\n");
    printf("- channel must be 1 or 2\n");
    printf("- duty_cycle must be within 0 and 100\n");
    printf("Remember: PA8 => channel 1, PB14 => channel 2\n\n");
}

int main()
{
    PumpDriver driver(1);
    uint32_t channel = 0, duty = 0, read;
    char line[10];

    welcome();
    usage();

    while (1)
    {
        fgets(line, 10, stdin);
        read = sscanf(line, "%lu %lu", &channel, &duty);

        if (read != 2 || channel < 1 || channel > 2 || duty < 0 || duty > 100)
        {
            printf("Invalid input\n");
            usage();
            continue;
        }

        printf("Applying %lu%% to channel %lu\n\n", duty, channel);
        driver.setChannelDuty(channel, duty);
    }
}
