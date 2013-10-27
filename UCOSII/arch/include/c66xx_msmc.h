
#ifndef C66XX_MSMC_H_
#define C66XX_MSMC_H_

#define MSM_ADDR	0x0c000000
#define	MSM_SIZE	0x00400000

#define NON_PREFETCH_BARRIER_SIZE	128

#define MSM_ENTRY_NUM				32
#define MSM_ENTRY_RAW_SIZE			(128*1024 - 128)
#define MSM_ENTRY_SIZE				128*1024
#define ID_TO_MEMORY(id)			(MSM_ADDR + (id) * MSM_ENTRY_SIZE)
#define MEMORY_TO_ID(addr)			(((addr) - MSM_ADDR)/MSM_ENTRY_SIZE)

void *BSP_AllocShareMemoryById(unsigned long id);
void *BSP_AllocFreeShareMemory(void);
void BSP_FreeShareMemory(void *addr);

#endif /* C66XX_MSMC_H_ */
