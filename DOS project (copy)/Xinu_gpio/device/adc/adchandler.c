#include <xinu.h>

extern sid32 read_sem;

void adchandler(uint32 xnum)
{
	//int i=0;
	struct	dentry	*devptr;            // Address of device control blk	
	struct	adc_csreg *csrptr;          // Address of UART's CSR	
	volatile uint32	irqStatus = 0;      // Interrupt identification
	//pub_data *temp;
	
	// Get register base address of the device
	devptr = (struct dentry *) &devtab[ADC];
	csrptr = (struct adc_csreg *) devptr->dvcsr;
	irqStatus = csrptr->irqStatus;      // read interrupt status
	csrptr->irqStatus = irqStatus;      // clear interrupt by write back	
	dprintf("ADC Handler called \n");

	/*if(irqStatus & OUT_OF_RANGE){
		dprintf("called out of range \n");
	
		// Store the result in the publish queue
		temp = (pub_data*)getmem(sizeof(pub_data)*1);
		
		temp->signal = (257<<8) + enabled_step(csrptr) - 1;
		temp->next = NULL;
		temp->data = csrptr->fifoData0 &(0xFFF); 


		if(pub_head == NULL) {
			/* List is empty, create one */
		/*	pub_head = temp;
			pub_tail = pub_head;
		}
    		else {
    			pub_tail->next = temp;
    			pub_tail = temp;
    		} */
	
	/*else if(irqStatus & ADC_END_OF_SEQUENCE_INTR){
		//Need to push FIFO data into the process buffer
		
		
		 	
	}*/
	signal(read_sem);                    //signal all readers	
	

	return;
}
