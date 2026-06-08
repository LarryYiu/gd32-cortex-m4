#include "internal_flash.h"
#include <stdio.h>
#include <string.h>
#include "gd32f30x.h"

static uint8_t backupBuffer[FLASH_PAGE_SIZE];

static void __Unlock()
{
    __disable_irq();
    fmc_unlock();
}

static void __Lock()
{
    fmc_lock();
    __enable_irq();
}

static bool __IsAddrValid(uint32_t addr, uint32_t lenOperation)
{
    return (addr + lenOperation) <= FLASH_DEADLINE_ADDRESS;
}

bool FLASH_Read(uint32_t addr, uint8_t* buffer, uint32_t lenReading)
{
    if(!__IsAddrValid(addr, lenReading))
    {
        printf("[ERROR: Flash Read] addr out of range\n");
        return false;
    }
    memcpy(buffer, (const void*)addr, lenReading);
    return true;
}

static fmc_state_enum __Erase(uint32_t addr)
{
    printf("[Flash Erase] erasing 0x%08x\r\n", addr);
    __Unlock();
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
    fmc_state_enum fmcState = fmc_page_erase(addr);
    __Lock();
    printf("[Flash Erase] erasing 0x%08x done\r\n", addr);
    return fmcState;
}

/**
 * @brief Handle erasing of a single uneven page, inlcudes backup and resote
 *
 * @param addr the starting address of the area to be erased
 * @param addrOffset the offset of the starting address relative to the page starting address
 * @param lenErasing the length to be erased
 * @param backupBuffer the address of the array backup the page
 *
 * @return erase success or fail
 */
static bool __ProcessEraseSinglePage(uint32_t addrPageStart, uint32_t addrEraseStart, uint32_t lenErasing)
{
    if(addrPageStart == addrEraseStart && lenErasing == FLASH_PAGE_SIZE)
    {
        printf("[Flash Erase] Case whole page\r\n");
        return __Erase(addrPageStart) == FMC_READY;
    }
    else
    {
        /* Back up the pgae */
        memcpy(backupBuffer, (const void*)addrPageStart, FLASH_PAGE_SIZE);
        printf("[Flash Backup] backup finished\r\n");

        /* Process the page data, remove erased data */
        memset(backupBuffer + (addrEraseStart - addrPageStart), 0xff, lenErasing);

        /* Erase the page */
        __Erase(addrEraseStart);

        /* Restore the page */
        bool res = FLASH_Write(addrPageStart, backupBuffer, FLASH_PAGE_SIZE);
        printf("[Flash Backup] restore finished\r\n");
        return res;
    }
}

bool FLASH_Erase(uint32_t addr, uint32_t lenErasing) // better not to do recurrsion as limited resources
{
    if(!__IsAddrValid(addr, lenErasing))
    {
        printf("[Error: Flash Erase] addr out of range\r\n");
        return false;
    }
    else
    {
        // | ------------------|----------------------------------|---------|
        // page 0          erase start      (len remain)        page 2

        uint32_t addrEraseStart = addr % FLASH_PAGE_SIZE;
        uint32_t lenRemain      = FLASH_PAGE_SIZE - addrEraseStart;
        if(lenErasing > lenRemain)
        {
            // deal with the first page
            if(!__ProcessEraseSinglePage(addr - addrEraseStart, addr, lenRemain))
            {
                goto erase_err;
            }
            lenErasing -= lenRemain;
            uint32_t addrPage = addr - addrEraseStart + FLASH_PAGE_SIZE;
            // deal with the middle pages
            while(lenErasing > FLASH_PAGE_SIZE)
            {
                if(!__ProcessEraseSinglePage(addrPage, addrPage, FLASH_PAGE_SIZE))
                {
                    goto erase_err;
                }
                addrPage += FLASH_PAGE_SIZE;
                lenErasing -= FLASH_PAGE_SIZE;
            }
            // deal with the leftoever
            if(lenErasing > 0)
            {
                if(!__ProcessEraseSinglePage(addrPage, addrPage, lenErasing))
                {
                    goto erase_err;
                }
            }
            else
            {
                __Lock(); // for insurence
                return true;
            }
        }
        else
        {
            printf("[Flash Erase]: single page\r\n");
            if(!__ProcessEraseSinglePage(addr - addrEraseStart, addr, lenErasing))
            {
                goto erase_err;
            }
        }
        return true;
    }
erase_err:
    __Lock();
    return false;
}
#define WRITE_DEBUG false
bool FLASH_Write(uint32_t addr, uint8_t* buffer, uint32_t lenWriting)
{
    if((addr + lenWriting) > FLASH_DEADLINE_ADDRESS)
    {
        printf("[ERROR: Flash Write] addr out of range\n");
        return false;
    }

    if(addr % 2 == 1) // addr must matches a half-word
    {
        printf("[ERROR: Flash Write] addr must be half-word aligned\n");
        return false;
    }

    printf("[Flash Writing]: Writing Start\n");
#if WRITE_DEBUG
    uint8_t validateBuffer[2];
#endif
    __Unlock();
    for(uint32_t i = 0; i < lenWriting / 2; i++)
    {
#if WRITE_DEBUG
        printf("[Flash Writing Data]: %02x %02x\r\n", buffer[0], buffer[1]);
#endif
        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
        if(fmc_halfword_program(addr, *(uint16_t*)buffer) != FMC_READY)
        {
            __Lock();
            printf("[Error: Flash Writing]: invalid writing, FMC_READY not ready\r\n");
            return false;
        }
#if WRITE_DEBUG
        __Lock();
        memcpy(validateBuffer, (const void*)addr, 2);
        printf("[Flash Writing Validate]: %02x %02x\r\n", validateBuffer[0], validateBuffer[1]);
        __Unlock();
#endif
        buffer += 2;
        addr += 2;
    }
    /* Handle the case that the last piece writing is a byte instead of a half-word */
    if(lenWriting % 2 != 0)
    {
        fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
        uint16_t last = *buffer | 0xff00; // fill in the MSBs
        if(fmc_halfword_program(addr, last) != FMC_READY)
        {
            __Lock();
            printf("[ERROR: Flash Writing]: Writing Failed\n");
            return false;
        }
    }
    __Lock();
    printf("[Flash Writing]: Writing Finished\n");
    return true;
}
