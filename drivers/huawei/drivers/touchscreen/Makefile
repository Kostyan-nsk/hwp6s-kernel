#ifdef CONFIG_HUAWEI_TOUCH_CODE


ifdef CONFIG_HUAWEI_H30L_TOUCH_CODE
  obj-y += ts_h30l/
else
  obj-y += ts/
endif


#else
#obj-y += rmi/
#obj-y += cypress/
#endif
obj-$(CONFIG_HUAWEI_TOUCH_INPUT_INFO) += touch_info.o
