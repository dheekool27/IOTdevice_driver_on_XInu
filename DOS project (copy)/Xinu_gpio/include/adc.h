#ifndef ADC_H
#define ADC_H

struct adc_csreg
{
	volatile uint32 revison;
	volatile uint32 reserve1[3];
	volatile uint32 sysconfig;
	volatile uint32 reserve2[4];
	volatile uint32 irqStatusRaw;
	volatile uint32 irqStatus;
	volatile uint32 irqEnableSet;	
	volatile uint32 irqEnableClr;
	volatile uint32 irqWakeup;
	volatile uint32 dmaEnableSet;
	volatile uint32 dmaEnableClr;
	volatile uint32 ctrl;
	volatile uint32 adcStat;
	volatile uint32 adcRange;
	volatile uint32 adcClkDiv;
	volatile uint32 adcMisc;
	volatile uint32 stepEnable;
	volatile uint32 idleConfig;
	volatile uint32 tsChargeStepconfig;
	volatile uint32 tsChargeDelay;
	struct 
	{
		volatile uint32 stepConfig;
		volatile uint32 stepDelay;
	}step[16];
	struct 
	{
		volatile uint32 fifoCount;
		volatile uint32 fifoThreshold;
		volatile uint32 dmaReq;
	}fifoInfo[2];
	volatile uint32 reserve3;
	volatile uint32 fifoData0;
	volatile uint32 reserve4[63];
	volatile uint32 fifoData1;
};

devcall	adcread(uint8 pin, char* buff, int32 count);
devcall	adcinit (struct	dentry *devptr);
void adchandler(uint32 xnum);

extern bool8 enabled_steps[];

#define ADC_ADDR					(0x44E0D000)


#define ADC_STEPCONFIG_DIFF_CNTRL   (0x01<<25)

//differential mode or single ended mode
#define ADC_SINGLE_ENDED_OPER_MODE  (0)
#define ADC_DIFFERENTIAL_OPER_MODE	(1)

//ADC channel
#define AN0            	(0)		//PIN AN0
#define AN1            	(1) 		//PIN AN1
#define AN2            	(2)   		//PIN AN2
#define AN3            	(3)   		//PIN AN3
#define AN4            	(4)   		//PIN AN4
#define AN5            	(5) 		//PIN AN5
#define AN6            	(6) 		//PIN AN6
#define AN7            	(7)		//PIN AN7


//SEL_RFM_SWC field in step config register
#define ADC_NEGATIVE_REF_VSSA		(0)			//SEL_RFM pins SW configuration (23:24)
#define ADC_NEGATIVE_REF_XNUR		(1)
#define ADC_NEGATIVE_REF_YNLR		(2)
#define ADC_NEGATIVE_REF_VREFN		(3)

#define ADC_POSITIVE_REF_VDDA		(0)			//SEL_RFP pins SW configuration (12:14)
#define ADC_POSITIVE_REF_XPUL		(1)
#define ADC_POSITIVE_REF_YPLL		(2)
#define ADC_POSITIVE_REF_VREFP		(3)

#define SEL_RFM_SWC_CLEAR			(0X03<<23)	//& ~SEL_RFM_SWC_CLEAR will clear the SEL_RFM_SWC field
#define SEL_RFM_SWC_SHIFT			(23)		//SEL_RFM_SWC is at 23:24 bit

//SEL_INP_SWC field in step config register
#define SEL_INP_SWC_CLEAR			(0XF<<19)	//& ~SEL_INP_SWC_CLEAR will clear the SEL_INP_SWC field
#define SEL_INP_SWC_SHIFT			(19)		//SEL_INP_SWC is at 19:22 bit

//SEL_INM_SWC field in step config register
#define SEL_INM_SWC_CLEAR			(0X0F<<15)	//& ~SEL_INM_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_INM_SWC_SHIFT			(15)		//SEL_INM_SWC is at 15:18 bit

//SEL_RFP_SWC field in step config register
#define SEL_RFP_SWC_CLEAR			(0x07<<12)	//& ~SEL_RFP_SWC_CLEAR will clear the SEL_INM_SWC field
#define SEL_RFP_SWC_SHIFT			(12)		//SEL_RFP_SWC is at 15:18 bit


#define ADC_STEPCONFIG_WPNSW_SWC_CLEAR   		(0x01<<11)
#define ADC_STEPCONFIG_WPNSW_SWC_SHIFT  		(0x0000000Bu)

#define ADC_STEPCONFIG_XNNSW_SWC_CLEAR   		(0x01<<6)
#define ADC_STEPCONFIG_XNNSW_SWC_SHIFT  		(0x00000006u)

#define ADC_STEPCONFIG_XNPSW_SWC_CLEAR   		(0x01<<9)
#define ADC_STEPCONFIG_XNPSW_SWC_SHIFT  		(0x00000009u)

#define ADC_STEPCONFIG_XPPSW_SWC_CLEAR   		(0x01<<5)
#define ADC_STEPCONFIG_XPPSW_SWC_SHIFT  		(0x00000005u)

#define ADC_STEPCONFIG_YNNSW_SWC_CLEAR   		(0x01<<8)
#define ADC_STEPCONFIG_YNNSW_SWC_SHIFT  		(0x00000008u)

#define ADC_STEPCONFIG_YPNSW_SWC_CLEAR   		(0x01<<10)
#define ADC_STEPCONFIG_YPNSW_SWC_SHIFT  		(0x0000000Au)

#define ADC_STEPCONFIG_YPPSW_SWC_CLEAR   		(0x01<<7)
#define ADC_STEPCONFIG_YPPSW_SWC_SHIFT  		(0x00000007u)



// FIFO field
#define ADC_FIFO_0                           	(0)
#define ADC_FIFO_1                           	(1)
#define ADC_FIFO_SELECTION_CLEAR			 	(0X01<<26)
#define ADC_FIFO_SELECTION_SHIFT			 	(26)

//MODE field
#define ADC_MODE_CLEAR						 	(0X03)
#define ADC_MODE_SW_ONE_SHOT					(0X00)
#define ADC_MODE_SW_CONTINUOUS					(0x01)
#define ADC_MODE_HW_ONE_SHOT					(0X10)
#define ADC_MODE_HW_CONTINUOUS					(0x11)
#define ADC_MODE_SHIFT							(0x00)

//ADC work mode
#define ADC_FOUR_WIRE_MODE                   	(1)		//for touch screen
#define ADC_FIVE_WIRE_MODE                   	(2)		//for touch screen
#define ADC_GENERAL_PURPOSE_MODE             	(0)		//for general purpose
#define ADC_WORK_MODE_CLEAR						(0x03<<5)
#define ADC_WORK_MODE_SHIFT						(5)		

//ADC interrupt bit set
#define ADC_END_OF_SEQUENCE_INTR				(0x01<<1)	//end of sequence interrupt
#define OUT_OF_RANGE 						(0x01<<8)       //sampled data out of range

#define HIGH_RANGE_SHIFT 16
#define LOW_RANGE_SHIFT  0
#define MAX_HIGH_RANGE  0xFFF
#define MAX_LOW_RANGE   0xFFF

#define ADC_CHANNEL_ID_SHIFT 16
#define NUM_OF_CHANNELS      8


#define isvalidanalogpin(pin) (pin < 8)


void step_configuration(struct adc_csreg *pReg);

void step_disable(struct adc_csreg *pReg, unsigned int selectStep);


void stepmode_config(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode);

void fifo_sel_config(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO);

void analog_ground_config(struct adc_csreg *pReg, unsigned int stepSelect,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw);

void analog_supply_config(struct adc_csreg *pReg, unsigned int stepSelect,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw);

void ADCstep_configuration(struct adc_csreg *pReg, unsigned int stepSelect, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef);
void ADCStepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode);

void step_configuration(struct adc_csreg *pReg);

void adc_configure(struct adc_csreg *pReg);

void enable_module(struct adc_csreg *pReg);

void disable_module(struct adc_csreg *pReg);


void clear_interrupt(struct adc_csreg *pReg);

void step_enable(struct adc_csreg *pReg, unsigned int selectStep);

void configure_mode(struct adc_csreg *pReg,unsigned int workMode);

void config_write_protect(struct adc_csreg *pReg, unsigned int isWritable);

void adc_configure_clock(struct adc_csreg *pReg, unsigned int freq);

void set_interrupt(struct adc_csreg *pReg, unsigned int irqEnable);

void adc_interrupt(struct	dentry *devptr);

void ADCStepAverge(struct adc_csreg *pReg, int stepSelect, int sampleNum);

void adc_clock_init();

extern sid32 adc_read;

void adc_subscribe(uint32 device_id, uint32 topic_id);

void adc_unsubscribe(uint32,uint32);
void store_step_id(struct adc_csreg *,bool8 );

uint8 enabled_step(struct adc_csreg *pReg);

#endif
