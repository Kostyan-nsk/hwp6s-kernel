

/*****************************************************************************
  1 头文件包含
  *****************************************************************************/
#include <linux/module.h>
#include <linux/mtd/nve_interface.h>
#include <linux/kernel.h>
#include "DrvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/
    

BSP_S32 DRV_NVE_ACCESS(NVE_INFO_S *nve)
{
#ifdef CONFIG_K3V2_NVE
    int ret;	
    struct nve_info_user  info;	

    if (!nve) {
    	printk(KERN_ERR "BSP_NVE_DirectAccess input is null!");
    	return BSP_ERROR;
    }    
    
    info.nv_operation = nve->nv_operation;
    info.nv_number = nve->nv_number;
    info.valid_size = nve->valid_size;
    memcpy(info.nv_name,nve->nv_name,BSP_NVE_NAME_LENGTH);
    
    /*write operation*/
    if (!nve->nv_operation)
        memcpy(info.nv_data,nve->nv_data,nve->valid_size);
        
    ret = nve_direct_access((struct nve_info_user *)&info);	        
    
    if (!ret) {
    	/*read operation*/
        if (nve->nv_operation)
            memcpy(nve->nv_data,info.nv_data,nve->valid_size);
            
        return BSP_OK;
    }
    else
        return BSP_ERROR;
#else
    return BSP_ERROR;
#endif
}
    
EXPORT_SYMBOL(DRV_NVE_ACCESS);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

