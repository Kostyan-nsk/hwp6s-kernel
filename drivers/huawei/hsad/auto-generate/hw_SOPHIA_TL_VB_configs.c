/*The file is auto generated by tools, don't modify it manully.*/
#include <hsad/configdata.h>
#include <hsad/config_boardids.h>
config_pair  hw_SOPHIA_TL_VB_configs [] = {
    {"apds/apds_type", (unsigned int)8, E_CONFIG_DATA_TYPE_INT },
    {"atmel/slave_address", (unsigned int)51, E_CONFIG_DATA_TYPE_INT },
    {"audio/aud_para", (const unsigned int)(unsigned int*)"SOPHIA", E_CONFIG_DATA_TYPE_STRING },
    {"audio/audience", (const unsigned int)(unsigned int*)"NONE", E_CONFIG_DATA_TYPE_STRING },
    {"audio/comph_val", (unsigned int)142, E_CONFIG_DATA_TYPE_INT },
    {"audio/digital_mic", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"audio/dual_mic", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"audio/fm_type", (const unsigned int)(unsigned int*)"digital", E_CONFIG_DATA_TYPE_STRING },
    {"audio/gpio_codec_reset", (unsigned int)51, E_CONFIG_DATA_TYPE_INT },
    {"audio/gpio_es305_reset", (unsigned int)111, E_CONFIG_DATA_TYPE_INT },
    {"audio/gpio_es305_wakeup", (unsigned int)7, E_CONFIG_DATA_TYPE_INT },
    {"audio/gpio_intr", (unsigned int)7, E_CONFIG_DATA_TYPE_INT },
    {"audio/hs_4_pole_max_voltage", (unsigned int)2565, E_CONFIG_DATA_TYPE_INT },
    {"audio/hs_4_pole_min_voltage", (unsigned int)1350, E_CONFIG_DATA_TYPE_INT },
    {"audio/hs_keys", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"audio/hs_pa", (const unsigned int)(unsigned int*)"NONE", E_CONFIG_DATA_TYPE_STRING },
    {"audio/hsd_invert", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"audio/key_back_max_value", (unsigned int)680, E_CONFIG_DATA_TYPE_INT },
    {"audio/key_back_min_value", (unsigned int)500, E_CONFIG_DATA_TYPE_INT },
    {"audio/modem_algo", (const unsigned int)(unsigned int*)"NONE", E_CONFIG_DATA_TYPE_STRING },
    {"audio/modem_switch", (const unsigned int)(unsigned int*)"NONE", E_CONFIG_DATA_TYPE_STRING },
    {"audio/ril_type", (const unsigned int)(unsigned int*)"USE", E_CONFIG_DATA_TYPE_STRING },
    {"audio/spk_class", (const unsigned int)(unsigned int*)"SPK_DIG", E_CONFIG_DATA_TYPE_STRING },
    {"audio/spk_pa", (const unsigned int)(unsigned int*)"TFA9887", E_CONFIG_DATA_TYPE_STRING },
    {"audio/spk_route", (const unsigned int)(unsigned int*)"LINEOUT_L", E_CONFIG_DATA_TYPE_STRING },
    {"audio/tcxo_support", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"audio/type_1_vout_min_voltage", (unsigned int)2700000, E_CONFIG_DATA_TYPE_INT },
    {"battery/battery_type", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"bbp/have_bbp_gpio", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"bluetooth/gpio_bt_en", (unsigned int)31, E_CONFIG_DATA_TYPE_INT },
    {"bluetooth/gpio_bt_wake_host", (unsigned int)5, E_CONFIG_DATA_TYPE_INT },
    {"bluetooth/gpio_host_wake_bt", (unsigned int)127, E_CONFIG_DATA_TYPE_INT },
    {"board/board_name", (const unsigned int)(unsigned int*)"SOPHIA_TL_VB", E_CONFIG_DATA_TYPE_STRING },
    {"board/type", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"camera/SONYIMX179", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"camera/SONYIMX214", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"camera/faceIgnoreCount", (unsigned int)9, E_CONFIG_DATA_TYPE_INT },
    {"camera/iopower", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"camera/miniisp", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"camera/primary_sensor", (unsigned int)90, E_CONFIG_DATA_TYPE_INT },
    {"camera/primary_sensor_flip_type", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"camera/primary_sensor_raw_h", (unsigned int)3120, E_CONFIG_DATA_TYPE_INT },
    {"camera/primary_sensor_raw_w", (unsigned int)4160, E_CONFIG_DATA_TYPE_INT },
    {"camera/primary_sensor_zsl", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"camera/secondary_sensor", (unsigned int)270, E_CONFIG_DATA_TYPE_INT },
    {"camera/secondary_sensor_flip_type", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"camera/secondary_sensor_raw_h", (unsigned int)2464, E_CONFIG_DATA_TYPE_INT },
    {"camera/secondary_sensor_raw_w", (unsigned int)3280, E_CONFIG_DATA_TYPE_INT },
    {"camera/secondary_sensor_zsl", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"camera/zsl_enable", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"charger/ac_charger_up_directly", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"charger/charger_cd", (unsigned int)32, E_CONFIG_DATA_TYPE_INT },
    {"charger/charger_int", (unsigned int)11, E_CONFIG_DATA_TYPE_INT },
    {"charger/charger_otg", (unsigned int)33, E_CONFIG_DATA_TYPE_INT },
    {"charger/line_loss_resistance", (unsigned int)260, E_CONFIG_DATA_TYPE_INT },
    {"connectivity/bt", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"connectivity/fm", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"connectivity/gps", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"connectivity/wifi", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"cypress/attn_gpio", (unsigned int)30, E_CONFIG_DATA_TYPE_INT },
    {"cypress/flip_x", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"cypress/flip_y", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"cypress/irq_config", (unsigned int)3, E_CONFIG_DATA_TYPE_INT },
    {"cypress/is_incell", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"cypress/reset_gpio", (unsigned int)151, E_CONFIG_DATA_TYPE_INT },
    {"cypress/slave_address", (unsigned int)26, E_CONFIG_DATA_TYPE_INT },
    {"cypress/vbus_voltage", (unsigned int)1800000, E_CONFIG_DATA_TYPE_INT },
    {"cypress/vdd_voltage", (unsigned int)2800000, E_CONFIG_DATA_TYPE_INT },
    {"ddr/type", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"flash/lm3642", (unsigned int)3, E_CONFIG_DATA_TYPE_INT },
    {"gas_gauge/bq27510_enable", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"gas_gauge/charger_enable", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"gas_gauge/r_pcb", (unsigned int)5, E_CONFIG_DATA_TYPE_INT },
    {"gas_gauge/ss_coul_enable", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"gas_gauge/ss_use_pmu_ocv", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"gps/gpio_gps_en", (unsigned int)49, E_CONFIG_DATA_TYPE_INT },
    {"gps/gpio_gps_refclk", (unsigned int)91, E_CONFIG_DATA_TYPE_INT },
    {"i2c/aps", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/audience", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/battery", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/camera_main", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/camera_slave", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/charge", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/compass", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/dc_dc", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/g_sensor", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/gyroscope", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/hand_sensor", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/keypad", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/l_sensor", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"i2c/mhl", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/nct", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/nfc", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/ofn", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/speark", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"i2c/touch_panel", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"i2c/tps", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"i2c/usb_switch", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"irq_affinity/irq147", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"irq_affinity/irq76", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"irq_affinity/irq77", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"irq_affinity/irq78", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"irq_affinity/irq79", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"keyboard/key_volumn_down", (unsigned int)3, E_CONFIG_DATA_TYPE_INT },
    {"keyboard/key_volumn_up", (unsigned int)34, E_CONFIG_DATA_TYPE_INT },
    {"lcd/cabc_enable", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"lcd/devices", (const unsigned int)(unsigned int*)"mipi_jdi_OTM1902B", E_CONFIG_DATA_TYPE_STRING },
    {"lcd/full_range_bl_enable", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_enn", (unsigned int)107, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_enp", (unsigned int)54, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_id0", (unsigned int)36, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_id1", (unsigned int)37, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_pwm_en", (unsigned int)64, E_CONFIG_DATA_TYPE_INT },
    {"lcd/gpio_lcd_reset", (unsigned int)38, E_CONFIG_DATA_TYPE_INT },
    {"lcd/resolution_type", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"lcd/tps65132_enable", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"log/loglevel", (unsigned int)4, E_CONFIG_DATA_TYPE_INT },
    {"log/uart0", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/camera", (unsigned int)4, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/codec", (unsigned int)53248, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/fb", (unsigned int)24310, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/gpu", (unsigned int)1024, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/gralloc", (unsigned int)171568, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/overlay", (unsigned int)130048, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/ram", (unsigned int)64, E_CONFIG_DATA_TYPE_INT },
    {"memory_config/vpp", (unsigned int)5120, E_CONFIG_DATA_TYPE_INT },
    {"mhl/type", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"mmc/gpio_emmc_reset", (unsigned int)19, E_CONFIG_DATA_TYPE_INT },
    {"mmc/gpio_mmc_detect", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"modem/qsc6085", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"platform_info/platform_num", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"platform_info/provider", (const unsigned int)(unsigned int*)"HUAWEI_B", E_CONFIG_DATA_TYPE_STRING },
    {"platform_info/sub_platform_info", (const unsigned int)(unsigned int*)"V9R1", E_CONFIG_DATA_TYPE_STRING },
    {"pmu/gpio_pmu_irq", (unsigned int)8, E_CONFIG_DATA_TYPE_INT },
    {"pmu/gpio_power", (unsigned int)9, E_CONFIG_DATA_TYPE_INT },
    {"product/name", (const unsigned int)(unsigned int*)"SOPHIA_TL_VB", E_CONFIG_DATA_TYPE_STRING },
    {"product/product_name", (const unsigned int)(unsigned int*)"SOPHIA_TL", E_CONFIG_DATA_TYPE_STRING },
    {"pwc/afreq", (unsigned int)1996000, E_CONFIG_DATA_TYPE_INT },
    {"pwc/gfreq", (unsigned int)700000, E_CONFIG_DATA_TYPE_INT },
    {"sd_card/present_voltage", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sd_card/sdcard_clk_pin", (unsigned int)66, E_CONFIG_DATA_TYPE_INT },
    {"sd_card/sdcard_frequency", (unsigned int)100, E_CONFIG_DATA_TYPE_INT },
    {"sd_card/sdcard_support", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"sensor/ad7146_gpio", (unsigned int)0xFFFFFFFF, E_CONFIG_DATA_TYPE_INT },
    {"sensor/akm09911", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"sensor/akm8963", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sensor/cap_exist", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sensor/compass_gpio", (unsigned int)0xFFFFFFFF, E_CONFIG_DATA_TYPE_INT },
    {"sensor/compass_layout", (unsigned int)4, E_CONFIG_DATA_TYPE_INT },
    {"sensor/gsensor_gpio", (unsigned int)25, E_CONFIG_DATA_TYPE_INT },
    {"sensor/gyro_exist_info", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"sensor/hall_exist", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"sensor/hall_gpio_config_rn", (const unsigned int)(unsigned int*)"w_27", E_CONFIG_DATA_TYPE_STRING },
    {"sensor/hall_gpio_config_rs", (const unsigned int)(unsigned int*)"s_60", E_CONFIG_DATA_TYPE_STRING },
    {"sensor/hall_used_type", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"sensor/proximity_gpio", (unsigned int)29, E_CONFIG_DATA_TYPE_INT },
    {"sensor/rising_and_falling", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sensor/sensor_rgb_type", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sensor/sensor_type", (unsigned int)3, E_CONFIG_DATA_TYPE_INT },
    {"sim/sim1_detection_gpio", (unsigned int)22, E_CONFIG_DATA_TYPE_INT },
    {"sim/sim1_present_electrical_lvl", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"sim/sim2_detection_gpio", (unsigned int)0xffffffff, E_CONFIG_DATA_TYPE_INT },
    {"sim/sim2_present_electrical_lvl", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/attn_gpio", (unsigned int)30, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/flip_x", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/flip_y", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/irq_config", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/is_incell", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/reset_gpio", (unsigned int)151, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/slave_address", (unsigned int)112, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/vbus_voltage", (unsigned int)1800000, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/vdd_voltage", (unsigned int)3100000, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/x_res", (unsigned int)1080, E_CONFIG_DATA_TYPE_INT },
    {"synaptics/y_res", (unsigned int)1920, E_CONFIG_DATA_TYPE_INT },
    {"tech/tech_type", (unsigned int)23, E_CONFIG_DATA_TYPE_INT },
    {"touchscreen/bus_id", (unsigned int)2, E_CONFIG_DATA_TYPE_INT },
    {"touchscreen/bus_type", (const unsigned int)(unsigned int*)"i2c", E_CONFIG_DATA_TYPE_STRING },
    {"touchscreen/product", (const unsigned int)(unsigned int*)"sophia", E_CONFIG_DATA_TYPE_STRING },
    {"touchscreen/support_chip_name_1", (const unsigned int)(unsigned int*)"synaptics", E_CONFIG_DATA_TYPE_STRING },
    {"touchscreen/support_chip_name_2", (const unsigned int)(unsigned int*)"cypress", E_CONFIG_DATA_TYPE_STRING },
    {"touchscreen/support_chip_name_3", (const unsigned int)(unsigned int*)"atmel", E_CONFIG_DATA_TYPE_STRING },
    {"touchscreen/support_chip_name_4", (const unsigned int)(unsigned int*)"null", E_CONFIG_DATA_TYPE_STRING },
    {"usb/ashell_enable", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"usb/cshell_enable", (unsigned int)0, E_CONFIG_DATA_TYPE_INT },
    {"usb/eye_pattern", (unsigned int)0x709334ff, E_CONFIG_DATA_TYPE_INT },
    {"usb/gpio_intb", (unsigned int)24, E_CONFIG_DATA_TYPE_INT },
    {"usb/use_switch_driver", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"video/ref_frame_count", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"video/vpp_enable", (unsigned int)1, E_CONFIG_DATA_TYPE_INT },
    {"wifi/gpio_wifi_enable", (unsigned int)10, E_CONFIG_DATA_TYPE_INT },
    {"wifi/gpio_wifi_irq", (unsigned int)4, E_CONFIG_DATA_TYPE_INT },
    {"wifi/nvram", (const unsigned int)(unsigned int*)"nvram_SOPHIA_TL", E_CONFIG_DATA_TYPE_STRING },
    {"wifi/wifi_clock", (unsigned int)50000000, E_CONFIG_DATA_TYPE_INT },
    {0, 0, 0}
 };
struct board_id_general_struct config_common_SOPHIA_TL_VB= {
 .name=COMMON_MODULE_NAME,
 .board_id=BOARD_ID_SOPHIA_TL_VB,
 .data_array={.config_pair_ptr=hw_SOPHIA_TL_VB_configs},
 .list={NULL,NULL},
};
