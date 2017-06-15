#
# Makefile for the accelerometer drivers.
#
#sensor unification for differ platform
# Each configuration option enables a list of files.

obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACCELEROMETER_ADI_ADXL346)	+= adxl34x.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACCELEROMETER_MMA8452)	+= gs_mma8452.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACCELEROMETER_ST_LIS3XH)	+= lis3dh_acc.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACCELEROMETER_ROHM_KX023)	+= gs_rohm_kx023.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACC_COMPASS_LSM303D)	+= lsm303d.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACC_GYRO_LSM330)   += lsm330_acc.o
obj-$(CONFIG_HUAWEI_FEATURE_SENSORS_ACC_GYRO_LSM330)   += lsm330_gyr.o
obj-$(CONFIG_INPUT_LSM330_I2C) += lsm330_acc_i2c.o lsm330_gyr_i2c.o
ifeq ($(CONFIG_GSENSOR_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG
endif