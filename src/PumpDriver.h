#include "miosix.h"

using namespace miosix;

class PumpDriver
{
public:
    PumpDriver(float freq);
    void setChannelDuty(uint32_t channel, uint32_t duty);
    ~PumpDriver();

private:
    uint16_t counter_max;
    using pa8 = Gpio<GPIOA_BASE, 8>;
    using pb14 = Gpio<GPIOB_BASE, 14>;
};
