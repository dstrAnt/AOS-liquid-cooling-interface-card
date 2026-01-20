#include "PumpDriver.h"

using namespace miosix;

PumpDriver::PumpDriver()
{

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

void PumpDriver::setChannelDuty(uint32_t channel, uint32_t duty)
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

PumpDriver::~PumpDriver()
{
    TIM1->CR1 &= ~TIM_CR1_CEN; // Stop timer

    pa8::mode(Mode::INPUT);

    pb14::mode(Mode::INPUT);
}
