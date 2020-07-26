

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include  <linux/regulator/consumer.h>
#include  <linux/err.h>
#include  "test_acore.h"
#include  "test_regulator_drv.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
int test_regulator_get(void)
{
     struct regulator *g3d_regulator;
     g3d_regulator = regulator_get(NULL,"vcc_g3d");

     if (IS_ERR(g3d_regulator))
     {
         printk(KERN_ERR "the function regulator_get return value : %ld",PTR_ERR(g3d_regulator));
         return ERROR;
     }
     regulator_put(g3d_regulator);
     return OK;

}
int test_regulator_enable(void)
{
     struct regulator *g3d_regulator;
     int ret;
     g3d_regulator = regulator_get(NULL,"vcc_g3d");

     if (IS_ERR(g3d_regulator))
     {     
         printk(KERN_ERR "the regulator_get return value : %ld",PTR_ERR(g3d_regulator));
         return ERROR;
     }

     ret = regulator_enable(g3d_regulator);
     if (ret < 0)
     {
         printk(KERN_ERR "the regulator_enable return value : %d",ret);
         regulator_put(g3d_regulator);
         return ERROR;
     }

     ret = regulator_disable(g3d_regulator);
     regulator_put(g3d_regulator);
     return OK;

}

int test_regulator_disable(void)
{
     struct regulator *g3d_regulator;
     int ret;
     g3d_regulator = regulator_get(NULL,"vcc_g3d");

     if (IS_ERR(g3d_regulator))
     {     
         printk(KERN_ERR "the regulator_get return value : %ld",PTR_ERR(g3d_regulator));
         return ERROR;
     }

     ret = regulator_enable(g3d_regulator);
     if (ret < 0)
     {
         printk(KERN_ERR "the regulator_enable return value : %d",ret);
         regulator_put(g3d_regulator);
         return ERROR;
     }

     ret = regulator_disable(g3d_regulator);
     if (ret < 0)
     {
         printk(KERN_ERR "the regulator_disable return value : %d",ret);
         regulator_put(g3d_regulator);
         return ERROR;
     }
     regulator_put(g3d_regulator);
     return OK;

}











#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

