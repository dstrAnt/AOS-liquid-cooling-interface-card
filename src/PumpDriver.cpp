#include "PumpDriver.h"

using namespace miosix;

// frequency in kHz from 0.5kHz to 128kHz
PumpDriver::PumpDriver(float freq)
{
    // Rollback to 1kHz
    if (freq < 0.5 || freq > 128) {
        freq = 1;
    }
    // 0xffff is 0.483kHz, 0x7d00 is 32000, 1khz
    counter_max = 0x7d00 / freq;

    pa8::mode(Mode::ALTERNATE); // Set PA8 pin to alternate function
    pa8::alternateFunction(2);  // Set alternate function 2 (PA8 => TIM1_CH1)

    pb14::mode(Mode::ALTERNATE); // Set PB14 pin to alternate function
    pb14::alternateFunction(2);  // Set alternate function 2 (PB14 => TIM1_CH2N)

    {
        FastInterruptDisableLock dLock;
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable TIM1 clock timer
        RCC_SYNC();
    }

    TIM1->CNT = 0; // Reset counter
    // TIM1->PSC = 0 - 1;     // Prescaler clocked at 48MHz, timer incremented every 1us
    TIM1->ARR = counter_max; // Set counter period to max (16 bit timer)
    TIM1->EGR = TIM_EGR_UG;  // Generate an update event to reload the prescaler value immediately
    TIM1->SR = 0;            // Clear interrupt flag caused by setting UG (and all other flags)

    TIM1->CCMR1 = 0b110 << 4 | TIM_CCMR1_OC1PE | 0b110 << 12 | TIM_CCMR1_OC2PE; // enable PWM mode on CH1 and CH2; preload enable

    TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2NE; // Enable CH1 and CH2 outputs (CH2 with complementary output)
    TIM1->BDTR = TIM_BDTR_MOE;                   // Main output enable

    TIM1->CR1 = TIM_CR1_CEN; // Start timer at first edge
}

void PumpDriver::setChannelDuty(uint32_t channel, uint32_t duty)
{
    if (channel < 1 || channel > 2 || duty < 0 || duty > 100)
    {
        return;
    }

    duty = ((float)duty / 100) * counter_max; // Scale to 0-65535

    switch (channel)
    {
    case 1:
        TIM1->CCR1 = duty; // Set duty cycle on channel 1
        break;
    case 2:
        TIM1->CCR2 = duty; // Set duty cycle on channel 2
        break;
    }
}

PumpDriver::~PumpDriver()
{
    TIM1->CR1 &= ~TIM_CR1_CEN; // Stop timer

    pa8::mode(Mode::INPUT);  // Set PA8 pin back to input
    pb14::mode(Mode::INPUT); // Set PB14 pin back to input
}
