
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/module.h>

#include "pmic_common.h"
#include "pmic_reg.h"
#include "pmic_ssi.h"
#include "pmic_driver.h"
#include "pmicirq_driver.h"


extern PMUIRQ_DATA pmuirq_desc;

extern BSP_S32 BSP_PMU_RegGet( BSP_U32 regAddr, BSP_U32 * getData );


BSP_S32 pmic_reg_read(COMMON_REG_CFG* reg_cfg, BSP_U32 *value)
{
    BSP_S32 ret=BSP_ERROR;
    BSP_U32 reg_value=0;
    /*�Ƿ�����ж�*/
    if ((NULL == reg_cfg)||(NULL == value))
    {
       pr_err("PRARM NULL %s %d  \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*����baseaddr��value��Ϊ��ε���ssi���ӿ�ִ�ж��Ĵ�������*/
    ret=BSP_PMU_RegGet(reg_cfg->baseaddr,&reg_value);
    if (BSP_OK != ret)
    {
       pr_err("BSP_PMU_RegGet err %s %d  \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    *value = BSP_REG_GETBITS(&reg_value,0,reg_cfg->bits_offset,reg_cfg->bits_num);

    /*����*/
    return BSP_OK;
}
BSP_S32 pmic_reg_write(COMMON_REG_CFG* reg_cfg, BSP_U32 value)
{
    BSP_S32 ret=BSP_ERROR;
    BSP_U32 reg_value=0;

    /*�Ƿ�����ж�*/
    if (NULL == reg_cfg)
    {
       pr_err("PRARM NULL %s %d  \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*��üĴ���ֵ*/
    ret=BSP_PMU_RegGet(reg_cfg->baseaddr,&reg_value);
    if (BSP_OK != ret)
    {
       pr_err("BSP_PMU_RegGet err %s %d  \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*����µļĴ���ֵ*/
    BSP_REG_SETBITS(&reg_value,0,reg_cfg->bits_offset,reg_cfg->bits_num,value);
    /*����baseaddr��value��Ϊ��ε���ssiд�ӿ�ִ��д�Ĵ�������*/
    ret=BSP_PMU_RegSet(reg_cfg->baseaddr,reg_value);
    if (BSP_OK != ret)
    {
       pr_err("BSP_PMU_RegSet err %s %d  \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*����*/
    return BSP_OK;
}
BSP_S32 pmu_status_reg_read(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id)
{
    BSP_S32 ret=BSP_ERROR;
    BSP_U32 reg_id=0;
    BSP_U32 reg_size=0;
    BSP_U32*reg_addr=(BSP_U32*)NULL;
    BSP_U32*stat_reg_buf=(BSP_U32*)NULL;
    PMUIRQ_STAT* cur_stat = (PMUIRQ_STAT*)NULL;

    /*�Ƿ�����ж�*/
    if (NULL == pmu_res_ctrl)
    {
       pr_err("%s %d  pmu_res_ctrl null\n",__func__, __LINE__);
       return BSP_ERROR;
    }

    cur_stat = pmic_get_pmustat();

    /*����reg_type_id��ò�������stat_reg[reg_type_id]*/
    /*ͨ��ID��ȡ�ж�״̬�Ĵ�����ַ*/
    reg_addr = pmu_res_ctrl->stat_reg[reg_type_id];
    /*�Ĵ�������*/
    reg_size = pmu_res_ctrl->stat_reg_size[reg_type_id];

    stat_reg_buf = cur_stat->stat_reg_buf[reg_type_id];
    memset((void*)stat_reg_buf,0,reg_size);

    /*ѭ������ssi���ӿڶ������������мĴ���*/
    /*���ҽ���Ϣ������dev->stat��stat_reg_buf[reg_type_id]��*/
    for(reg_id = 0;reg_id < reg_size; ++reg_id)
    {
        ret=BSP_PMU_RegGet(reg_addr[reg_id],&(stat_reg_buf[reg_id]));
#ifdef PMIC_DEBUG
        printk("pmu register addr 0x%0x  data 0x0%0x\n",reg_id,stat_reg_buf[reg_id]);
#endif
    }

    /*����*/
    return ret;

}

BSP_S32 pmic_status_reg_read(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id,BSP_U32* data_buf)
{
    BSP_S32 ret=BSP_ERROR;
    BSP_U32 reg_id=0;
    BSP_U32 reg_size=0;
    BSP_U32*reg_addr=(BSP_U32*)NULL;
    BSP_U32 stat_reg_buf[PMIC_BUF_MAX_SIZE] = {0};

    /*�Ƿ�����ж�*/
    if (NULL == pmu_res_ctrl)
    {
       pr_err("%s %d  pmu_res_ctrl null\n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*����reg_type_id��ò�������stat_reg[reg_type_id]*/
    /*ͨ��ID��ȡ�ж�״̬�Ĵ�����ַ*/
    reg_addr = pmu_res_ctrl->stat_reg[reg_type_id];
    /*�Ĵ�������*/
    reg_size = pmu_res_ctrl->stat_reg_size[reg_type_id];

    memset((void*)stat_reg_buf,0,reg_size);

    /*ѭ������ssi���ӿڶ������������мĴ���*/
    /*���ҽ���Ϣ������dev->stat��stat_reg_buf[reg_type_id]��*/
    for(reg_id = 0;reg_id < reg_size; ++reg_id)
    {
        ret=BSP_PMU_RegGet(reg_addr[reg_id],&(stat_reg_buf[reg_id]));
        printk("pmu register addr 0x%0x  data 0x0%0x\n",reg_id,stat_reg_buf[reg_id]);
    }

    memcpy((void*)data_buf,stat_reg_buf,reg_size*sizeof(unsigned int));

    /*����*/
    return ret;

}


BSP_S32 pmu_status_reg_clear(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id)
{
    BSP_S32 ret=BSP_ERROR;
    BSP_U32 reg_id=0;
    BSP_U32 reg_size=0;
    BSP_U32*reg_addr=(BSP_U32*)NULL;

    /*�Ƿ�����ж�*/
    if(PMU_STATUS_REG_TYPE_ID_IS_INVALID(reg_type_id))
    {
       pr_err("%s %d reg_type_id \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    if(NULL == pmu_res_ctrl)
    {
       pr_err("%s %d pmu_res_ctrl null \n",__func__, __LINE__);
       return BSP_ERROR;
    }

    /*����reg_type_id��ò�������stat_reg[reg_type_id]*/
    reg_addr = pmu_res_ctrl->stat_reg[reg_type_id];
    /*�Ĵ�������*/
    reg_size = pmu_res_ctrl->stat_reg_size[reg_type_id];
    /*ѭ������ssiд�ӿ���������������мĴ���*/
    for(reg_id=0;reg_id<reg_size;++reg_id)
    {
        ret=BSP_PMU_RegSet(reg_addr[reg_id],(BSP_U32)PMU_IRQ_VALUE_CLEAR);/*W1C*/
    }

    /*����*/
    return ret;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
