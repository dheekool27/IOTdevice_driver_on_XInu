#include <xinu.h>


extern sid32 read_sem;

devcall	adcread(uint8 pin,char *buff, int32 count)
{
	intmask mask = disable();
	// Analog Pins are between 0 to 7
	if(!isvalidanalogpin(pin)) {
		dprintf("The pin is not a valid analog pin \n");
		restore(mask);
		return SYSERR;
	}

	/*if((pin_map[1][pin-1] & (1<<6)) == 0){
		dprintf("Not a valid analog pin \n");
		return SYSERR;
	}*/

	uint8 step = pin + 1;
	struct adc_csreg* pReg = (struct adc_csreg*)0x44e0d000;

	//Enable the ADC module, set the interrupt and enable the step

	if(!module_enabled(pReg)){
		
		dprintf("Enabling the ADC module for read");
		set_interrupt(pReg,ADC_END_OF_SEQUENCE_INTR);
		stepmode_config(pReg, 0, ADC_MODE_SW_ONE_SHOT);

		enable_module(pReg);
	}

	if(adc_busy(pReg)){
		//Wait till adc is free
		restore(mask);
		return SYSERR;
		}
	if(!step_enabled(pReg,step)){
		dprintf("Enabling step %d\n",step);
		step_enable(pReg,step);
		enabled_steps[step] = TRUE;
	}

	//wait for semaphore signaled by interrupt handler.
	wait(read_sem);

	unsigned int data = 0;
	int sampleNum = pReg->fifoInfo[0].fifoCount;
	int i;
	//dprintf("Total no of samples %d\n",sampleNum);
	for(i = 0; i < sampleNum; i++)
	{
		data = pReg->fifoData0 &(0xFFF) ;
	}
	buff[3] = (data&(0xff000000))>>24;
	buff[2] = (data&(0x00ff0000))>>16;	
	buff[1] = (data&(0x0000ff00))>>8;
	buff[0] = (data&(0x000000ff));
	restore(mask);
	return 1;
}
