
#ifndef C66XX_CIC_H
#define C66XX_CIC_H


/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/

struct cic_device	*BSP_GetCicChip(unsigned int channel);
void BSP_InitCic (void);

/**************************** 定义部分 *********************************/
#define CIC_CHIPS_NUM	2

#define CIC_BASE_ADDR		0x02600000
#define CIC_REG_BASE(chan)	(CIC_BASE_ADDR + (chan * 0x4000))


#define CIC_REVISION_REG(chan)				(CIC_REG_BASE(chan) + 0x0)
#define CIC_GLOBAL_ENABLE_HINT_REG(chan)	(CIC_REG_BASE(chan) + 0x10)
#define CIC_STATUS_SET_INDX_REG(chan)		(CIC_REG_BASE(chan) + 0x20)
#define CIC_STATUS_CLR_INDX_REG(chan)		(CIC_REG_BASE(chan) + 0x24)
#define CIC_ENABLE_SET_INDX_REG(chan)		(CIC_REG_BASE(chan) + 0x28)
#define CIC_ENABLE_CLR_INDX_REG(chan)		(CIC_REG_BASE(chan) + 0x2c)
#define CIC_HINT_ENABLE_SET_INDX_REG(chan)	(CIC_REG_BASE(chan) + 0x34)
#define CIC_HINT_ENABLE_CLR_INDX_REG(chan)	(CIC_REG_BASE(chan) + 0x38)

#define CIC_RAW_STATUS_REG_BASE(chan)		(CIC_REG_BASE(chan) + 0x200)
#define CIC_RAW_STATUS_REG(chan, num)		(CIC_RAW_STATUS_REG_BASE(chan) + (4 * num))

#define CIC_ENA_STATUS_REG_BASE(chan)		(CIC_REG_BASE(chan) + 0x280)
#define CIC_ENA_STATUS_REG(chan, num)		(CIC_ENA_STATUS_REG_BASE(chan) + (4 * num))

#define CIC_ENABLE_REG_BASE(chan)			(CIC_REG_BASE(chan) + 0x300)
#define CIC_ENABLE_REG(chan, num)			(CIC_ENABLE_REG_BASE(chan) + (4 * num))

#define CIC_ENABLE_CLR_REG_BASE(chan)		(CIC_REG_BASE(chan) + 0x380)
#define CIC_ENABLE_CLR_REG(chan, num)		(CIC_ENABLE_CLR_REG_BASE(chan) + (4 * num))

#define CIC_CH_MAP_REG_BASE(chan)			(CIC_REG_BASE(chan) + 0x400)
#define CIC_CH_MAP_REG(chan, num)			(CIC_CH_MAP_REG_BASE(chan) + (4 * num))

#define CIC_HINT_MAP_REG_BASE(chan)			(CIC_REG_BASE(chan) + 0x800)
#define CIC_HINT_MAP_REG(chan, num)			(CIC_HINT_MAP_REG_BASE(chan) + (4 * num))

#define CIC_ENABLE_HINT_REG_BASE(chan)		(CIC_REG_BASE(chan) + 0x1500)
#define CIC_ENABLE_HINT_REG(chan, num)		(CIC_ENABLE_HINT_REG_BASE(chan) + (4 * num))

/**************************** 结构体定义  *********************************/
struct	cic_device {
	int cic_id;
	int is_ready;
	void	(*cic_init)(int id, struct cic_device *cic_dev);

	void	(*cic_global_enable_interrupt)(struct cic_device *cic_dev);
	void	(*cic_global_disable_interrupt)(struct cic_device *cic_dev);
	int 	(*cic_global_interrupt_is_enable)(struct cic_device *cic_dev);

	void	(*cic_sys_interrupt_status_set)(int sysintnum, struct cic_device *cic_dev);
	void 	(*cic_sys_interrupt_status_clear)(int sysintnum, struct cic_device *cic_dev);
	int		(*cic_sys_interrupt_enable_status_get)(int sysintnum, struct cic_device *cic_dev);

	void	(*cic_sys_interrupt_raw_status_set)(int sysintnum, struct cic_device *cic_dev);
	int		(*cic_sys_interrupt_raw_status_get)(int sysintnum, struct cic_device *cic_dev);

	void	(*cic_sys_interrupt_enable)(int sysintnum, struct cic_device *cic_dev);
	void	(*cic_sys_interrupt_disable)(int sysintnum, struct cic_device *cic_dev);

	int		(*cic_sys_interrupt_channel_map)(int sysint,int channel_to_map, struct cic_device *cic_dev);
	int		(*cic_sys_interrupt_channel_get)(int channel, struct cic_device *cic_dev);
	int		(*cic_host_interrupt_map_get)(int channel_to_map, struct cic_device *cic_dev);

	void	(*cic_host_interrupt_enable)(int hostint, struct cic_device *cic_dev);
	void	(*cic_host_interrupt_disable)(int hostint, struct cic_device *cic_dev);
};



#endif /* CIC_H_ */
