#include "miosix.h"

// using namespace miosix;

class PumpDriver
{
public:
    PumpDriver(float freq);
    void setChannelDuty(uint32_t channel, uint32_t duty);
    ~PumpDriver();

private:
    uint16_t counter_max;
    using pa8 = miosix::Gpio<GPIOA_BASE, 8>;
    using pb14 = miosix::Gpio<GPIOB_BASE, 14>;
};
