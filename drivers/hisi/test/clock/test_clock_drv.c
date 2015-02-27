

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include <linux/clk.h>
#include <linux/module.h>
#include <linux/init.h>
#include <mach/clock.h>
#include <linux/err.h>
#include  "test_acore.h"
#include  "test_clock_drv.h"


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

int test_clock_get(void)
{
    struct clk *test_clk ;


    test_clk = clk_get(NULL,"clk_g2d");

     if (IS_ERR(test_clk))
     {
         printk(KERN_ERR "the function clk_get return value : %ld", PTR_ERR(test_clk));
         return ERROR;
     }
     clk_put(test_clk);

     return OK;
}

int test_clock_enable(void)
{
    struct clk *test_clk ;
    int ret;

    test_clk = clk_get(NULL,"clk_g2d");
     if (IS_ERR(test_clk))
     {
         printk(KERN_ERR "the function clk_get return value : %ld", PTR_ERR(test_clk));
         return ERROR;
     }
    
    ret = clk_enable(test_clk);
     if (ret < 0)
     {
         printk(KERN_ERR "the function clk_enable return value : %d",ret);
         clk_put(test_clk); 
         return ERROR;
     }
     clk_disable(test_clk);
     clk_put(test_clk);
     return OK;
}










#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

