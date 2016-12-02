#include <xinu.h>

bool8 enabled_steps[17];

sid32 read_sem;
sid32 adc_read;

void adc_configure(struct adc_csreg *pReg)
{
	adc_configure_clock(pReg, 3000000);

	config_write_protect(pReg, 1);
	step_configuration(pReg);

	configure_mode(pReg,ADC_GENERAL_PURPOSE_MODE);
	store_step_id(pReg,TRUE);
	clear_interrupt(pReg);

//	set_interrupt(pReg,ADC_END_OF_SEQUENCE_INTR);
//	enable_module(pReg);
}

void adc_configure_clock(struct adc_csreg *pReg, unsigned int freq)
{
	unsigned int clkDiv = 24000000/freq - 1;
	pReg->adcClkDiv &= ~(0xFFFF);
	pReg->adcClkDiv |= clkDiv;
}

void config_write_protect(struct adc_csreg *pReg, unsigned int isWritable)
{
	pReg->ctrl &= ~ (0x01<<2);
	pReg->ctrl |= (isWritable<<2);
}
void configure_mode(struct adc_csreg *pReg,unsigned int workMode)
{
	pReg->ctrl &= ~ ADC_WORK_MODE_CLEAR;
	pReg->ctrl |= (workMode<<ADC_WORK_MODE_SHIFT);
}

void store_step_id(struct adc_csreg *pReg,bool8 store)
{
	pReg->ctrl |= (store<<1);
	
}
void step_enable(struct adc_csreg *pReg, unsigned int selectStep)
{
	pReg->stepEnable |= (0x01<<selectStep);
}

void step_disable(struct adc_csreg *pReg, unsigned int selectStep)
{
	pReg->stepEnable &= ~(0x01<<selectStep);
}

void clear_interrupt(struct adc_csreg *pReg)
{
	pReg->irqStatus |= 0x7FFF;
}

void enable_module(struct adc_csreg *pReg)
{
	pReg->ctrl |= 0x01;
}
void disable_module(struct adc_csreg *pReg)
{
	pReg->ctrl &= ~(0x01);
}

void step_configuration(struct adc_csreg *pReg)
{
	int i =0;
	for(i=0;i<NUM_OF_CHANNELS;i++)
        {
		dprintf("Accessing step configration registers \n");
		//all steps, single ended mode
		ADCStepOperationModeControl(pReg,i,ADC_SINGLE_ENDED_OPER_MODE);	
		//all steps, set channel and reference voltage
		ADCstep_configuration(pReg, i, i, ADC_POSITIVE_REF_VDDA, i, ADC_NEGATIVE_REF_VSSA);
		analog_supply_config(pReg, i, 0, 0, 0);

		analog_ground_config(pReg, i, 0, 0, 0, 0);
		//use FIFO_0
		fifo_sel_config(pReg, i, ADC_FIFO_0);
		//SW continuous mode
		//stepmode_config(pReg, 0, ADC_MODE_SW_CONTINUOUS);
		//average samples
		ADCStepAverge(pReg,i,16);
	}
}
void ADCStepAverge(struct adc_csreg *pReg, int stepSelect, int sampleNum)
{
	if(sampleNum == 2)
		pReg->step[stepSelect].stepConfig |= 0x01<<2;
	else if(sampleNum == 4)
		pReg->step[stepSelect].stepConfig |= 0x02<<2;
	else if(sampleNum == 8)
		pReg->step[stepSelect].stepConfig |= 0x03<<2;	
	else if(sampleNum == 16)
		pReg->step[stepSelect].stepConfig |= 0x04<<2;
	else
		return;
}
void ADCStepOperationModeControl(struct adc_csreg *pReg, unsigned int stepSelect, unsigned int mode)
{
	if(mode)
	{
		pReg->step[stepSelect].stepConfig |= ADC_STEPCONFIG_DIFF_CNTRL;
	}
	else
	{
		pReg->step[stepSelect].stepConfig &= ~ADC_STEPCONFIG_DIFF_CNTRL;
	}
}

void ADCstep_configuration(struct adc_csreg *pReg, unsigned int stepSelect, 
					unsigned int positiveChannel,unsigned int positiveRef,
					unsigned int negativeChannel,unsigned int negativeRef)
{
	//configure the nagtive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeRef<<SEL_RFM_SWC_SHIFT;     //write

	//configure the positive input channel
	pReg->step[stepSelect].stepConfig &= ~ SEL_INP_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= positiveChannel<<SEL_INP_SWC_SHIFT; //write

	//configure the negative input channel,
	pReg->step[stepSelect].stepConfig &= ~ SEL_INM_SWC_CLEAR;                //clear
	pReg->step[stepSelect].stepConfig |= negativeChannel<<SEL_INM_SWC_SHIFT; //write

	//configure the positive reference
	pReg->step[stepSelect].stepConfig &= ~ SEL_RFP_SWC_CLEAR;                 //clear
	pReg->step[stepSelect].stepConfig |= positiveRef<<SEL_RFP_SWC_SHIFT;      //write
}

void analog_supply_config(struct adc_csreg *pReg, unsigned int stepSelect,
									unsigned int xppsw, unsigned int xnpsw, unsigned int yppsw)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XPPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= xppsw<<ADC_STEPCONFIG_XPPSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XNPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= xnpsw<<ADC_STEPCONFIG_XNPSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YPPSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |= yppsw<<ADC_STEPCONFIG_YPPSW_SWC_SHIFT;
}

void analog_ground_config(struct adc_csreg *pReg, unsigned int stepSelect,
								 unsigned int xnnsw,unsigned int ypnsw, unsigned int ynnsw,
                                 unsigned int wpnsw)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_XNNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  xnnsw<<ADC_STEPCONFIG_XNNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YPNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  ypnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_YNNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  ynnsw<<ADC_STEPCONFIG_YPNSW_SWC_SHIFT;

	pReg->step[stepSelect].stepConfig &= ~ ADC_STEPCONFIG_WPNSW_SWC_CLEAR;
	pReg->step[stepSelect].stepConfig |=  wpnsw<<ADC_STEPCONFIG_WPNSW_SWC_SHIFT;	
}

void fifo_sel_config(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int fifoNO)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_FIFO_SELECTION_CLEAR;
	pReg->step[stepSelect].stepConfig |= fifoNO<<ADC_FIFO_SELECTION_SHIFT;
}

void stepmode_config(struct adc_csreg *pReg, unsigned int stepSelect,unsigned int mode)
{
	pReg->step[stepSelect].stepConfig &= ~ ADC_MODE_CLEAR;
	pReg->step[stepSelect].stepConfig |= mode<<ADC_MODE_SHIFT;
}

void set_interrupt(struct adc_csreg *pReg, unsigned int irqEnable)
{
	pReg->irqEnableSet |=irqEnable;
}


bool8 module_enabled(struct adc_csreg *pReg){
	return (pReg->ctrl & (0x01));
}

bool8 step_enabled(struct adc_csreg *pReg,uint8 step){
	return	((pReg->stepEnable >> step) & 0x01);
}


void set_adc_high_range(struct adc_csreg *pReg,uint32 range) {
	pReg->adcRange |= ((range & MAX_HIGH_RANGE) << HIGH_RANGE_SHIFT);
	
}

void set_adc_low_range(struct adc_csreg *pReg,uint32 range) {
	pReg->adcRange |= ((range & MAX_LOW_RANGE) << LOW_RANGE_SHIFT);
	
}

bool8 adc_busy(struct adc_csreg *pReg){
	//A step is enabled
	return ((pReg->stepEnable & 0xFE) != 0);
}

uint8 enabled_step(struct adc_csreg *pReg) {
	return (pReg->stepEnable & 0xFE);
}	



bool8 continuous_mode(struct adc_csreg *pReg,uint8 step) {
	return (((pReg->step[step].stepConfig >> ADC_MODE_SHIFT) & 7) == ADC_MODE_SW_CONTINUOUS);
}

devcall	adcinit (struct	dentry *devptr)
{
	dprintf("Started ADC \n");
	struct adc_csreg* pReg = (struct adc_csreg*)devptr->dvcsr;

	read_sem = semcreate(0);
	adc_read = semcreate(0);									
	adc_clock_init();
	adc_interrupt(devptr);

	adc_configure(pReg);

	return 1;
}
