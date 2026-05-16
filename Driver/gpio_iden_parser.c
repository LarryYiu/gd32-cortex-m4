#include "gpio_iden_parser.h"

uint32_t GPIO_GetPeriphAddr(uint8_t iden)
{
    return GPIO_BASE + ((uint32_t)(iden >> 4) * (0x00000400));
}

uint32_t GPIO_GetPinAddr(uint8_t iden)
{
    return BIT(iden & 0x0f);
}

rcu_periph_enum GPIO_GetRcuPeriph(uint8_t iden)
{
    return (rcu_periph_enum)RCU_REGIDX_BIT(APB2EN_REG_OFFSET, (uint32_t)((iden >> 4) + 2U));
}
