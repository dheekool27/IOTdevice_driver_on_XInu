/*  main.c  - main */

#include<xinu.h>
#include<stdlib.h>
void buzzer_on(void)
{	
	static int count = 0;
	gpio_set_mode(0,14,0);
	if(count++ < 100)
 	  gpio_write(0,3,1);
	  
	else if(count++ < 200)
	   gpio_write(0,3,0);
	else
	 count = 0;
	return;	
}
void LED_g(void)
{	gpio_set_mode(0,3,0);
	static bool8 data = TRUE;		
	

	
	gpio_write(0,3,data);
}
void LED_r(void)
{	gpio_set_mode(0,5,0);
	static bool8 data = FALSE;

	gpio_write(0,5,data);
		
}

void buzzer(void)
{
	static bool8 data = FALSE;
	
	
	
	gpio_write(0,14,data);
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
process main(void)
{
	uid32	slot;			/* Slot in UDP table		*/
	uint32 remote_ip=0;
	uint16 remote_port=0,local_port = 1234;
	int32 retval=0;
   	int32 len;
   	int32 spcCount = 0;
   	int32 ptrIndex = 0;
   	int32 i,j,k;
	gpio_set_mode(0,11,1);
	gpio_subscribe(0,11,LED_g,TRUE);
	gpio_subscribe(0,11,LED_r,TRUE);
	gpio_subscribe(0,11,buzzer,TRUE);
   
 	slot = udp_register(remote_ip, remote_port, local_port);
	
	if (slot == SYSERR) {
		fprintf(stderr,"getutime: cannot register a udp port %d\n",
					local_port);
	}
	kprintf("UDP slot obtained");
    while(1) {
        char buffer[100];
	retval = udp_recvaddr(slot,&remote_ip, &remote_port,buffer, sizeof(buffer),5000);
		
		printf("received message:%s\n",buffer);			 
// ==========================================================================================================================
	if ( (retval == SYSERR) ) {
	printf("retval syserr");
		
		break;
	}

	if(retval == TIMEOUT){
	printf("retval timeout");
		continue;
	}



// added the c -tokenizer
  char num[10];		
  int32 q=0;

  while(buffer[q]!=' '){
num[q]=buffer[q]; 
q++;
}
 num[q]='\0';
 len=atoi(num);
  //printf("length is:%d\n",len);

int32 p=0;
char action[20];
for(i=0;i<len;i++)
{action[p]=buffer[q+1+i];
p++;
}
action[p]='\0';


char buffer_out[65535];
bool8 status;


	if(strncmp("PING",action,strlen(action)) == 0)
	{

		
		char ip_addr[256] ="192.168.0.50";
		int port = 1234;
		udp_sendto (
	 slot,	 remote_ip, remote_port, ip_addr, strlen(ip_addr));
	}
	
	if(strncmp("LEDR|ON",action,strlen(action))==0)
	{	
	//if((strncmp("ON",ptr[1],strlen(ptr[1])))==0)
        printf("message received from edge: %s",buffer);
		gpio_set_mode(0,5,0);
		gpio_write(0,5,1);
		sprintf(buffer_out,"%s","LEDRON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 6);
	 
	}
			
	else if(strncmp("LEDR|OFF",action,strlen(action))==0)
	{
	gpio_set_mode(0,5,0);
	gpio_write(0,5,0);
	sprintf(buffer_out,"%s","LEDROFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	else if(strncmp("LEDR|STATUS",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
	
		status=gpio_read(0,5);
	if(status){
		sprintf(buffer_out,"%s","LEDR|ON");
				udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	else
	{
		sprintf(buffer_out,"%s","LEDR|OFF");			
	udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 8);
	 
	}		
	}
	
	else if(strncmp("LEDG|STATUS",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
	
		status=gpio_read(0,3);
	if(status){
		sprintf(buffer_out,"%s","LEDG|ON");
				udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	else
	{
		sprintf(buffer_out,"%s","LEDG|OFF");			
	udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 8);
	 
	}		
	}	
	
	else if(strncmp("LEDB|STATUS",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
	
		status=gpio_read(0,12);
	if(status){
		sprintf(buffer_out,"%s","LEDB|ON");
				udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	else
	{
		sprintf(buffer_out,"%s","LEDB|OFF");			
	udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 8);
	 
	}		
	}			
	else if(strncmp("LEDG|OFF",action,strlen(action))==0)
	{
	gpio_set_mode(0,3,0);
	gpio_write(0,3,0);
	sprintf(buffer_out,"%s","LEDGOFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	
	
	else if(strncmp("LEDG|ON",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
		gpio_set_mode(0,3,0);
		gpio_write(0,3,1);
		sprintf(buffer_out,"%s","LEDGON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 6);
	 
	}
	else if(strncmp("LEDB|OFF",action,strlen(action))==0)
	{
	gpio_set_mode(0,12,0);
	gpio_write(0,12,0);
	sprintf(buffer_out,"%s","LEDBOFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	
	
	else if(strncmp("LEDB|ON",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
		gpio_set_mode(0,12,0);
		gpio_write(0,12,1);
		sprintf(buffer_out,"%s","LEDBON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 6);
	 
	}
	else if(strncmp("BUZZER|OFF",action,strlen(action))==0)
	{
	gpio_set_mode(0,14,0);
	gpio_write(0,14,0);
	sprintf(buffer_out,"%s","BUZZEROFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 9);
	 
	}
	
	
	else if(strncmp("BUZZER|ON",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
		gpio_set_mode(0,14,0);
		gpio_write(0,14,1);
		sprintf(buffer_out,"%s","BUZZERON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 8);
	 
	}
	else if(strncmp("BUZZER|STATUS",action,strlen(action))==0)
	{	
        printf("message received from edge: %s",buffer);
	
		status=gpio_read(0,14);
	if(status){
		sprintf(buffer_out,"%s","BUZZER|ON");
				udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 9);
	 
	}
	else
	{
		sprintf(buffer_out,"%s","BUZZER|OFF");			
	udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 10);
	 
	}		
	}
	else if(strncmp("TEMP|STATUS",action,strlen(action))==0)
		{ 
			
  			

				char data[4];
				uint32 integer = 0;
				uint32 decimal = 0;
				float voltage,temp_c;
				adcread(0,(char*)data,4);
				uint32 temp = 0;
				memcpy(&temp,data,4);
				printf("value from data is:%d\n",temp);
				voltage = (temp*1.8/4096)*1000;
				temp_c = (voltage - 500) / 10;
				integer = (unsigned int)temp_c;
				decimal = (unsigned int)((temp_c - integer)*10);
				kprintf("Voltage = %d mv\r\n",voltage);
				sprintf(buffer_out,"%d.%d",integer,decimal);
				kprintf("integer value is: %d\n",integer);
				kprintf("decimal value is: %d\n",decimal);	
				kprintf("temperature is %s:",buffer_out);
				udp_sendto ( slot, remote_ip,remote_port,	
	 buffer_out, strlen(buffer_out)	);
				//analog_read(257,1,);
		}
	
	else if(strncmp("SUBSCRIBE",action,strlen(action))==0){
		 //       analog_subscribe(257,12,buzzeron);
		}
        else
	    printf("unknown function name");
 
//=======================================================================================================================
	
	printf("sending back one reply ");
	


}
}
