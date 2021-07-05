#define addr_term 0x90
#define addr_dac 0xE4
#define all_modul 7
#define T_sensor 1
#define Dac_chip 2
#define mode_set 1<<2
#define mode_clear 0
#define xlat_set 1<<3
#define xlat_mode_set 1<<4
#define clear 0
#define max_dot_curr_ir 0x7f
#define max_dot_curr_white 0x7f
#define max_dot_curr_r 0x7f
#define max_dot_curr_g 0x7f
#define max_dot_curr_b 0x7f
#define max_dot_curr_irf 0x5f
#define speed_grade 25
#define max_power_dac 0x8000
#define power_lamp_100 max_power_dac 
#define power_lamp_80 max_power_dac -max_power_dac/10
#define power_lamp_50 max_power_dac/2
#define power_lamp_33 max_power_dac/3
#define power_lamp_10 max_power_dac/10
#define power_lamp_5 max_power_dac/20
#define power_lamp_1 max_power_dac/100
#define power_lamp_0 0
#define dy 250
#define dp 50
#define lamp_sys 0

#define STATE_ON 1
#define STATE_OFF 0
extern uint8_t SPI0_masterRxData[all_modul+1+32];
extern uint8_t SPI0_masterTxData[all_modul+1+32];
extern uint8_t SPI1_masterRxData[64*all_modul+32];
extern uint8_t SPI1_masterTxData[64*all_modul+32];
//////extern uint8_t led_data[all_modul*4*16];
//////extern uint8_t led_data_ir[all_modul*4*2];
//////extern uint8_t led_data_r[all_modul*4*4];
//////extern uint8_t led_data_g[all_modul*4*4];
//////extern uint8_t led_data_b[all_modul*4*4];
extern uint8_t cos_4[16];
extern uint8_t cos_2[16];
extern uint8_t tringl[31];
extern uint8_t flag_global_save_data;
extern uint8_t flag_global_load_def;

void  set_adress_drv( uint8_t modul,uint8_t device_sel,uint8_t addr);
void  set_data_drv( uint8_t modul,uint8_t mode,uint8_t addr,uint8_t* data);
void  set_xlat_mode_drv( void);
void  set_xlat_drv( void );
void load_dot_correct(uint8_t* data_mass_white,uint8_t* data_mass_green,uint8_t* data_mass_blue,uint8_t* data_mass_ir);
void remap_canal_lamp_r (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData );
void remap_canal_lamp_g (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData);  
void remap_canal_lamp_b (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData);
void remap_canal_lamp_irf (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData);
 void remap_canal_lamp_white (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData);
void remap_canal_lamp_ir (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData);
void DAC_slow_swich (uint16_t power);     
void Set_lamp_power (uint16_t power);
void RSet_lamp_power(void);
void lamp_init (uint16_t power);
void stoboskop (void);
void move_dot(uint8_t sost);