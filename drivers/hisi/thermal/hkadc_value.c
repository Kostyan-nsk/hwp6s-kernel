
#include "drv_temp_cfg.h"
#include "hkadc_value.h"
#include "NvIdList.h"
#ifdef _DRV_LLT_
#include "stdio.h"
#endif
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
struct thermal_data_area *g_thermal_data_area =  
        (struct thermal_data_area *)(THERMAL_CORE_SHARE_PHY2VIRT(M3_HKADC_DDR_ADDR));

int g_hkadc_map_fill_flag = HKADC_FALSE;

struct drv_hkadc_map g_hkadc_chan_map[] = {
    {HKADC_TEMP_BATTERY,    HKADC_CHAN_INVALID,   en_NV_Item_BATTERY_TEMP_ADC, 28, 0, "battery"},
    {HKADC_TEMP_PA0,        HKADC_CHAN_INVALID,   en_NV_Item_PA_TEMP_SENSOR_TABLE, 32, 0, "pa0"},
    {HKADC_TEMP_PA1,        HKADC_CHAN_INVALID,   en_NV_Item_PA_TEMP_SENSOR_TABLE, 32, 1, "pa1"},
    {HKADC_TEMP_DCXO0,      HKADC_CHAN_INVALID,   en_NV_Item_XO_TEMP_SENSOR_TABLE,  32, 0, "dcxo_0"},
    {HKADC_TEMP_DCXO1,      HKADC_CHAN_INVALID,   en_NV_Item_XO_TEMP_SENSOR_TABLE,  32, 1, "dcxo_1"},
    {HKADC_TEMP_LCD,        HKADC_CHAN_INVALID,   0, 0, 0, "lcd"},
    {HKADC_TEMP_SIM_CARD,   HKADC_CHAN_INVALID,   0, 0, 0, "sim_card1"},
    {HKADC_TEMP_SYSTEMH,    HKADC_CHAN_INVALID,   0, 0, 0, "systemh"},
    {HKADC_TEMP_SYSTEML,    HKADC_CHAN_INVALID,   0, 0, 0, "systeml"},    
    {HKADC_TEMP_BATT_ID,    HKADC_CHAN_INVALID,   0, 0, 0, "batt_id"},    
    {HKADC_TEMP_CAMERA0_ID, HKADC_CHAN_INVALID,   0, 0, 0, "camera0_id"},
    {HKADC_TEMP_CAMERA1_ID, HKADC_CHAN_INVALID,   0, 0, 0, "camera1_id"},
    {HKADC_TEMP_DCXO0_LOW,  HKADC_CHAN_INVALID,   en_NV_Item_XO_LOW_TEMP_SENSOR_TABLE,  32, 0, "dcxo_0_low"},
    {HKADC_TEMP_DCXO1_LOW,  HKADC_CHAN_INVALID,   en_NV_Item_XO_TEMP_SENSOR_TABLE,  32, 1, "dcxo_1_low"},    
    {HKADC_TEMP_SCAMERA,    HKADC_CHAN_INVALID,   0, 0, 0, "scamera"},
    {HKADC_TEMP_FLASH,      HKADC_CHAN_INVALID,   0, 0, 0, "flash"},
    {HKADC_TEMP_BUTT,       HKADC_CHAN_INVALID,   0, 0, 0, "buttom"}
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : hkadc_fill_chan_map
 ��������  : ���g_hkadc_chan_map���е�����ͨ��ID����
 �������  : 
 �������  : ��
 �� �� ֵ  : BSP_S32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��13��
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
int hkadc_fill_chan_map(unsigned short *p2l_tbl)
{
    int logic_id;
    int phy_id;
    struct drv_hkadc_map * chan_map;
    int ret = 0;
    for (phy_id = HKADC_CHAN_0 ; phy_id < HKADC_CHAN_MAX; phy_id++)
    {
        logic_id = p2l_tbl[phy_id];
        chan_map=  bsp_hkadc_get_chan_map(logic_id);
        if (HKADC_NULL != chan_map)
        {
            HKADC_DEBUG(("hkadc_fill_chan_map logic_id->phyID: %d -> %d \n",
                            logic_id, phy_id));
            chan_map->phy_chan = phy_id;
            ret = HKADC_ERROR;
        }
        else
        {
            HKADC_DEBUG(( "hkadc_fill_chan_map get map ERROR: logic_id %d \n",
                            logic_id));
            ret = HKADC_ERROR;
        }
    }
    return ret;
}
int hkadc_chan_map_renew(unsigned short *l2p_tbl)
{
    int logic_id = 0;
    int phy_id = 0;
    struct drv_hkadc_map * chan_map = NULL;
    int ret = HKADC_OK;

    for (logic_id = HKADC_TEMP_TOP ; logic_id < HKADC_TEMP_BUTT; logic_id++)
    {
        phy_id = l2p_tbl[logic_id];
        if(INVALID_HKADC_PHYVALUE != phy_id)
        {
            chan_map=  bsp_hkadc_get_chan_map(logic_id);
            if (HKADC_NULL != chan_map)
            {
                HKADC_DEBUG(("hkadc_fill_chan_map logic_id->phyID: %d -> %d \n",
                                logic_id, phy_id));
                chan_map->phy_chan = phy_id;
            }
            else
            {
                HKADC_DEBUG(( "hkadc_fill_chan_map get map ERROR: logic_id %d \n", 
                                logic_id));
                ret = HKADC_ERROR;
            }
        }    
    }
    return ret;
}

/*****************************************************************************
 �� �� ��  : bsp_hkadc_get_chan_map
 ��������  : ��ȡĳ���߼�ͨ������������ӳ���
 �������  : 
            enMode : logic_chan �豸�߼�ͨ����
 �������  : 
 �� �� ֵ  : 
            struct drv_hkadc_map * ָ��ĳ��hkadcӳ����ָ��
*****************************************************************************/
struct drv_hkadc_map *bsp_hkadc_get_chan_map(int logic_chan)
{
    struct drv_hkadc_map *map = &g_hkadc_chan_map[0];

    /*���������飬������ֶ�Ӧ�߼�ͨ����ӳ���������ָ��*/
    while (HKADC_TEMP_BUTT != map->logic_chan) {
        if (map->logic_chan == logic_chan) {
            return map;
        }
        map++;
     }
        
    return HKADC_NULL;
}

void hkadc_chann_map_show(void)
{
    struct drv_hkadc_map *map = &g_hkadc_chan_map[0];
    HKADC_DEBUG(("\nlogic_chan  phy_chan  nv_id  nv_len  modem_id name \n"));
    while (HKADC_TEMP_BUTT != map->logic_chan)
    {
       HKADC_DEBUG(("%8d %8d %8d %8d %8d %8s\n",map->logic_chan,map->phy_chan,
       map->nv_id, map->nv_len,map->modem_id,map->name));
       map++;
    }
}
/*****************************************************************************
�� �� ��  : hkadc_volttemp_trans
��������  : 
�������  : 
                      ucChan       :ͨ��ֵ
                      usVoltage    :��ѹ
                      hkAdcTable   :��ѹ���������ݵĻ����
                      tempTableLen :hkadc��ѹ�¶�ת����ĳ���
�������  : 
           N/A
�� �� ֵ  : 
           �����������
*****************************************************************************/
short hkadc_volttemp_trans(unsigned char ucChan, unsigned short usVoltage, unsigned short *hkAdcTable, unsigned short tempTableLen)
{
    unsigned short usTemNum = 0x00;          /*��¼�¶ȸ���*/
    short RetVal = 0x00;

    struct hkadc_tem_value *pstTemVoltTAble = (struct hkadc_tem_value *)hkAdcTable;
    struct hkadc_tem_value *pstTemp = (struct hkadc_tem_value *)hkAdcTable;
    
    /*need convert*/
    if(!hkAdcTable)
    {
        /*ֱ�ӷ��ص�ѹ*/
        return TEMP_VALUE_INVALID;
    }

    for(usTemNum=0; usTemNum< tempTableLen;usTemNum++)
    {
        if((0x00 == pstTemp->volt) && (0x00 == pstTemp->temp))
        {
            break;
        }
        pstTemp++;
    }

    /*NV����Ч��������Чֵ*/
    if((0x00 == pstTemVoltTAble->temp) && (0x00 == pstTemVoltTAble->volt) )
    {
        HKADC_DEBUG(("HKADC ch %d NV invalid !\n", ucChan));
        return TEMP_VALUE_INVALID;
    }
    /*NV�����ݺϷ�*/

    /*�жϼ���ֵ*/
    if(usVoltage >= pstTemVoltTAble->volt)
    {
        RetVal =  pstTemVoltTAble->temp;
        return RetVal;
    }

    pstTemp =(struct hkadc_tem_value *)((pstTemVoltTAble + usTemNum) -1);
    if(usVoltage <= pstTemp->volt)
    {
        RetVal = pstTemp->temp;
        return RetVal;
    }

    /*���ݵ�ѹ���в�ֵ����*/
    while(usVoltage < pstTemVoltTAble->volt)
    {
        pstTemVoltTAble++;
    }

    pstTemp = (struct hkadc_tem_value *)(pstTemVoltTAble -1); /*ָ��ǰһ��Ԫ��*/

    RetVal = (short)pstTemp->temp + (short)((((pstTemp->volt -usVoltage) * (pstTemVoltTAble->temp - pstTemp->temp))) \
                    /(pstTemp->volt -pstTemVoltTAble->volt));

    return RetVal;

}


/*****************************************************************************
 �� �� ��  : bsp_hkadc_get_shared_value
 ��������  : ��ȡĳ��ͨ�����¶ȼ���ѹ��ֵ
 �������  : 
            struct drv_hkadc_map * ָ��ĳ��hkadcӳ����ָ��
 �������  : 
 �� �� ֵ  : 
*****************************************************************************/
int bsp_hkadc_get_shared_value(struct drv_hkadc_map *map, 
                    unsigned short* volt,
                    short* temp)
{ 
    struct hkadc_chan_out * chan_out = g_thermal_data_area->chan_out;
    struct hkadc_chan_config *chan_cfg = g_thermal_data_area->chan_cfg;
    short  volt_l;
    short  volt_r;
    short  temp_l;
    short  temp_r;
    unsigned int phy_chan;
    unsigned int count;
    int ret = HKADC_OK; 
    unsigned short voltvalue;
    short tempvalue;
    OUT_PROP_CONFIG outConfig;
    
    phy_chan = map->phy_chan;

    /*���ͨ��û�����ã�ֱ�ӷ���HKADC_ERROR*/
    if (chan_cfg[phy_chan].have_config == 0)
        return HKADC_ERROR;
        
    outConfig = chan_cfg[phy_chan].outcfg.bit_out_config;
    

    /*�Ƚϵ�ѹ��ԭʼֵ��ȡ��ֵ�����������򷵻�ԭʼֵ*/

    count = 5;        
    do {   
        volt_l = (short)chan_out[phy_chan].volt_l;
        volt_r = (short)chan_out[phy_chan].volt_r;
    } while ((count--) && (~volt_l != volt_r));

    if (~volt_l == volt_r) {
        voltvalue = volt_l;
    } else {
        return HKADC_ERROR;
    }
      
    if (volt) { 
        *volt = voltvalue;
    }
    
    /*�Ƚ��¶ȵ�ԭʼֵ��ȡ��ֵ�����������򷵻�ԭʼֵ*/
    if (temp) {
        /*�����������¶ȣ�ֱ�Ӵӹ����ڴ��ȡ�¶Ȳ�����*/
        if (outConfig.out_vol_temp == 1){
            count = 5;
            do {
                temp_l = chan_out[phy_chan].temp_l;
                temp_r = chan_out[phy_chan].temp_r;
            } while ((count--) && (~temp_l != temp_r));

            if(~temp_l == temp_r) {
                *temp = temp_l; 
            } else {
                ret = HKADC_ERROR;
            }
        }
        /*���û����������¶ȣ������ϲ���Ҫ�¶ȣ�����ͨ����ѹ�¶�ת����ֱ��ת��*/
        else{
            unsigned short temptablelen = g_thermal_data_area->chan_cfg[phy_chan].temp_data_len;
            tempvalue = hkadc_volttemp_trans(phy_chan, voltvalue, 
                (unsigned short *)g_thermal_data_area->chan_cfg[phy_chan].temp_table, temptablelen);
            if (tempvalue != TEMP_VALUE_INVALID){
                *temp = tempvalue;
            } else {
                ret = HKADC_ERROR;
            }
        }
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : DRV_HKADC_GET_TEMP
 ��������  : ����ϲ������enMode��Ч������V3R3��֧�֣������0xFFFF����ֱ�ӷ��ء�
             ���pusVolt!=NULL��������¶�
             ���psTemp!=NULL,�������ѹ
 �������  : 
            enMode : hkadc�豸  
 �������  : 
            pusVolt : ͨ����ѹ
            psTemp: ͨ���¶�
 �� �� ֵ  : 
            HKADC_OK/HKADC_ERROR
*****************************************************************************/
int DRV_HKADC_GET_TEMP(HKADC_TEMP_PROTECT_E log_chan, 
                unsigned short* volt,
                short* temp, HKADC_CONV_MODE_E ulMode)
{
    struct drv_hkadc_map *map = 0;
   
#if !defined(BOARD_ASIC)
    /*����ֵ*/
    if (volt)
        *volt = 927;

    if (temp)
        *temp = 448;
    return HKADC_OK;
#endif
    /*����ֵ*/
    if (volt)
        *volt = HKADC_VALUE_INVALID;

    if (temp)
        *temp = HKADC_VALUE_INVALID;

    /*��ȡͨ��ӳ���*/
    map = bsp_hkadc_get_chan_map(log_chan);

    /*����ж�*/
    if (((0 == volt) && (0 == temp)) || ( HKADC_NULL == map )
        || (map->phy_chan >= HKADC_CHAN_MAX)) {
        HKADC_DEBUG(("DRV_HKADC_GET_TEMP ERROR param %d, 0x%x, 0x%x \n", 
            (int)log_chan, (int)volt, (int)temp));
		return HKADC_ERROR;
    }    

    /*��ȡ�¶�ֵ*/
	return bsp_hkadc_get_shared_value(map, volt, temp);
}


