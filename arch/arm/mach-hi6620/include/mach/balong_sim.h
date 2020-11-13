
#define SIM1 "SIM1"
#define SIM2 "SIM2"

//#define SIM_HPD

/*sim����λ��ƽֵ*/
#define HIGH_LEVEL 1
#define LOW_LEVEL 0

/*GPIO����ʱ��*/
#define SIM_DEBOUNCE_TIME   (10) 

/*if there are no sim gpio detection on hw platform, simX_detection_gpio should be 0xffffffff*/
#define NO_SIM_GPIO 	(0xffffffff)

struct balong_sim_plat_data{
    char * name;
    unsigned int sim_detection_gpio;
    unsigned int sim_status;
#ifdef SIM_HPD
    unsigned int sim_hpd_out_f;
    unsigned int sim_hpd_in_f;
#endif
};
