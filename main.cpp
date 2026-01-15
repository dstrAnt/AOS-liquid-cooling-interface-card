
#include <cstdio>
#include "miosix.h"
#include <thread>
// #include <e20/e20.h>

using namespace std;
using namespace miosix;

#define PWM (0x7UL << (4U))

// FixedEventQueue<10> queue;

class PumpDriver
{
public:
    PumpDriver()
    {
        printf("**************\nWelcome to Miosix-based computer liquid cooling interface v1.0!!\n**************\n");

        pa8::mode(Mode::ALTERNATE);
        pa8::alternateFunction(2);

        pb14::mode(Mode::ALTERNATE);
        pb14::alternateFunction(2);

        {
            FastInterruptDisableLock dLock;
            RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
            RCC_SYNC();
        }

        TIM1->CNT = 0;
        TIM1->PSC = 48 - 1; // Prescaler clocked at 48MHz, timer incremented every 1us
        TIM1->ARR = 0xffff;
        TIM1->EGR = TIM_EGR_UG; // Update ARR shadow register
        TIM1->SR = 0;           // Clear interrupt flag caused by setting UG

        TIM1->CCMR1 = 0b110 << 4 | TIM_CCMR1_OC1PE | 0b110 << 12 | TIM_CCMR1_OC2PE;

        TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2NE;
        TIM1->BDTR = TIM_BDTR_MOE;

        TIM1->CR1 = TIM_CR1_CEN; // Start timer at first edge
    }

    void setChannelDuty(uint32_t channel, uint32_t duty)
    {
        if (channel < 1 || channel > 2 || duty < 0 || duty > 100)
        {
            printf("Invalid input, try again\n");
            return;
        }

        duty = ((float)duty / 100) * 0xffff;

        switch (channel)
        {
        case 1:
            TIM1->CCR1 = duty;
            // TIM1->EGR = TIM_EGR_UG;
            // printf("TIM1->CCR1=%d\n", TIM1->CCR1);
            break;
        case 2:
            TIM1->CCR2 = duty;
            // printf("TIM1->CCR2=%d\n", TIM1->CCR2);

            break;
        }
    }

    ~PumpDriver()
    {
        TIM1->CR1 &= ~TIM_CR1_CEN; // Stop timer

        pa8::mode(Mode::INPUT);

        pb14::mode(Mode::INPUT);
    }

private:
    using pa8 = Gpio<GPIOA_BASE, 8>;
    using pb14 = Gpio<GPIOB_BASE, 14>;
};

int main()
{
    PumpDriver driver;
    uint32_t channel=0, duty=0;
    char line[10];

    // while (1)
    // {
    //     driver.setChannel(2, 100-duty);
    //     driver.setChannel(1, duty++);
    //     this_thread::sleep_for(0.5s);
    // }

    while (1)
    {
        printf("Specify channel [1-2] and the duty cycle [0-100]\nIn this form: 'channel duty_cycle'\n");
        fgets(line, 10, stdin);
        sscanf(line, "%lu %lu", &channel, &duty);
        // cin >> channel >> duty;

        if (channel < 1 || channel > 2 || duty < 0 || duty > 100)
        {
            printf("Invalid input, try again\n");
            continue;
        }

        // TIM1->SR = 0; // Clear interrupt flag

        // duty = ((float)duty / 100) * 0xffff;

        printf("received: %lu, %lu\n", channel, duty);
        driver.setChannelDuty(channel, duty);
    }
}

// int main3()
// {
//     // thread t([]{ queue.run(); });
//     printf("Welcome to Miosix-based computer liquid cooling interface v0.3!!\n");

//     using pa8 = Gpio<GPIOA_BASE, 8>;
//     using pa11 = Gpio<GPIOA_BASE, 11>;

//     // pa8::mode(Mode::OUTPUT);
//     // pa8::high();
//     // for(;;) ;
//     // int i=5;
//     // queue.post([=]{
//     //     printf("Hello %d\n");
//     // });

//     // FastInterruptDisableLock dLock;
//     pa8::mode(Mode::ALTERNATE);
//     pa8::alternateFunction(2);

//     pa11::mode(Mode::ALTERNATE);
//     pa11::alternateFunction(2);

//     {
//         FastInterruptDisableLock dLock;
//         RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
//         RCC_SYNC();
//     }

//     TIM1->CNT = 0;
//     TIM1->PSC = 48 - 1; // Prescaler clocked at 84MHz, timer incremented every 1us
//     TIM1->ARR = 0xffff;
//     TIM1->EGR = TIM_EGR_UG; // Update ARR shadow register
//     TIM1->SR = 0;           // Clear interrupt flag caused by setting UG
//     // TIM1->CCMR1 = TIM_CCMR1_CC1S_0;
//     // TIM1->CCER = TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC1E;
//     // TIM1->DIER = TIM_DIER_CC1IE | TIM_DIER_UIE;
//     // TIM1->CR1 = TIM_CR1_OPM; // One pulse mode, timer not started yet

//     // for(;;)
//     // {
//     //     printf("%d\n",TIM1->CNT);
//     //     this_thread::sleep_for(1s);
//     // }

//     // TIM1->CR1 = 0;
//     // TIM1->CR1 |= TIM_CR1_CEN;

//     TIM1->CCMR1 = 0b110 << 4 | TIM_CCMR1_OC1PE;
//     TIM1->CCMR2 = 0b110 << 12 | TIM_CCMR2_OC4PE;

//     TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC4E;
//     TIM1->BDTR = TIM_BDTR_MOE;

//     TIM1->CR1 = TIM_CR1_CEN; // Start timer at first edge

//     // TIM1->CNT = 0;

//     while (1)
//     {
//         printf("Specify a channel [1-2] and the duty cycle [0-100]\n");
//         uint32_t channel, duty;
//         scanf("%lu:%lu", &channel, &duty);
//         // cin >> channel >> duty;

//         if (channel < 1 || channel > 2 || duty < 0 || duty > 100)
//         {
//             printf("Invalid input, try again\n");
//             continue;
//         }

//         TIM1->SR = 0; // Clear interrupt flag

//         duty = ((float)duty / 100) * 0xffff;

//         printf("received: %lu, %lu\n", channel, duty);
//         switch (channel)
//         {
//         case 1:
//             TIM1->CCR1 = duty;
//             // TIM1->EGR = TIM_EGR_UG;
//             // printf("TIM1->CCR1=%d\n", TIM1->CCR1);
//             break;
//         case 2:
//             TIM1->CCR4 = duty;
//             printf("TIM1->CCR4=%d\n", TIM1->CCR4);

//             break;
//         }
//     }
// }

// int main2()
// {

//     // // pins
//     // using p1 = Gpio<GPIOB_BASE, 7>;
//     // p1::mode(Mode::OUTPUT);
//     // p1::high();

//     using pa8 = Gpio<GPIOA_BASE, 8>;

//     // FastInterruptDisableLock dLock;
//     pa8::mode(Mode::ALTERNATE);
//     pa8::alternateFunction(2);

//     for (;;)
//     {
//         cout << "Specify a channel [1-4] and the duty cycle [0-100]\n";
//         // string line;
//         // getline(cin, line);
//         uint32_t channel, duty;
//         cin >> channel >> duty;

//         cout << "received: " << channel << ", " << duty << endl;

//         // p1::low();
//         // p1::high();
//     }
// }