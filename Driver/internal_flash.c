#include "internal_flash.h"
#include <stdio.h>
#include <string.h>
#include "gd32f30x.h"

static void __BackupPage(uint32_t addr, uint8_t* backupBuffer)
{
    memcpy(backupBuffer, (const void*)addr, FLASH_PAGE_SIZE);
}

static void __RestorePage(uint32_t addr, uint8_t* backupBuffer)
{
    for(uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 4)
    {
        memcpy((void*)addr, backupBuffer + i, 4);
    }
}

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
    if((addr + lenOperation) > FLASH_DEADLINE_ADDRESS)
    {
        return false;
    }
    return true;
}

bool FLASH_Read(uint32_t addr, uint8_t* buffer, uint32_t lenReading)
{
    if(!__IsAddrValid(addr, lenReading))
    {
        printf("[ERROR: Flash Read] addr out of range\n");
        return false;
    }

    uint32_t _addr = addr;
    for(uint32_t i = 0; i < lenReading; i++)
    {
        *buffer = *(uint8_t*)_addr;
        addr++;
        buffer++;
    }
    return true;
}

static bool __EraseSinglePage(uint32_t addr, uint32_t addrOffset, uint32_t lenErasing, uint8_t* backupBuffer)
{
    /* back up the content in the first page */
    __BackupPage(addr, backupBuffer);
    /* modify the buffer by placing one to the erase area */
    memset(backupBuffer + addrOffset, 0xFF, FLASH_PAGE_SIZE - addrOffset);
    /* erase the first page */
    fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
    /* erase then check if success */
    __Unlock();
    if(fmc_page_erase(addr) != FMC_READY)
    {
        __Lock();
        return false;
    }
    /* restore the content in the first page */
    __RestorePage(addr, backupBuffer);
    __Lock();
    return true;
}

bool FLASH_Erase(uint32_t addr, uint32_t lenErasing) // better not to do recurrsion as limited resources
{
    if(!__IsAddrValid(addr, lenErasing))
    {
        return false;
    }
    else
    {
        /**
         * example
         * if addr = 0x10001800 + 500;
         * then addrOffset = addr % FLASH_PAGE_SIZE = 500;
         * the page size - addrOffset = the room left to reach the next page
         * if lenErasing is larger, then it will go to then next page
         */
        uint32_t addrOffset = addr % FLASH_PAGE_SIZE;

        uint8_t backupBuffer[FLASH_PAGE_SIZE];

        /* calculate the gap size between the end of the erase area and the end of page */
        uint32_t tailSize = FLASH_PAGE_SIZE - addrOffset;

        if(lenErasing > tailSize) // case multiple pages
        {
            /* Handle the first page */
            if(!__EraseSinglePage(addr, addrOffset, tailSize, backupBuffer))
            {
                return false;
            }
            /* Handle the rest */
            uint32_t pageNum = lenErasing / FLASH_PAGE_SIZE;
            addr += tailSize;       // set addr to the start of next apge
            lenErasing -= tailSize; // update the len of erasing
            while(pageNum--)        // no backup-restore for whole pages
            {
                fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
                if(fmc_page_erase(addr) != FMC_READY)
                {
                    __Lock();
                    return false;
                }
                addr += FLASH_PAGE_SIZE;
            }
            if(lenErasing % FLASH_PAGE_SIZE != 0) // handle the last page
            {
                return __EraseSinglePage(addr, 0UL, lenErasing, backupBuffer);
            }
            __Lock();
            return true;
        }
        else // case single page
        {
            return __EraseSinglePage(addr, addrOffset, lenErasing, backupBuffer);
        }
    }
}
