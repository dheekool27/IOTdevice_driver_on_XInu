/*  main.c  - main */

#include <xinu.h>

uint16 my_getnbr(char *str)
{
  uint16           result;
  uint16           puiss;

  result = 0;
  puiss = 1;
  /*while (('-' == (*str)) || ((*str) == '+'))
    {
      if (*str == '-')
        puiss = puiss * -1;
      str++;
    }*/
  while ((*str >= '0') && (*str <= '9') && *str !='\0')

    { printf("extracted number %c",*str);
      result = (result * 10) + ((*str) - '0');
      str++;
    }
printf("length extracted\n");
  return (result * puiss);
}

void buzzer_on(void)
{
	static int count = 0;

	if(count++ < 100)
 	  gpio_write(256,3,1);
	  
	else if(count++ < 200)
	   gpio_write(256,3,0);
	else
	 count = 0;

	return;
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
process main(void)
{
	uid32	slot;			/* Slot in UDP table		*/
	uint32 remote_ip=0;
	uint16 remote_port=0,local_port = 1234;
	int32 retval=0;
   	int len;
   	int spcCount = 0;
   	int ptrIndex = 0;
   	int i,j,k;
   
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
  int j=0;
  while(buffer[j]!=" " && buffer[j]!='\0'){
num[j++]=buffer[j];  
}
num[j]='\0';
 len=my_getnbr(num);
  printf("length is:%d\n",len);

spcCount = 0;
  ptrIndex = 0;

   for(i = j; i < len; i++)
   {
      if(i != 0 && buffer[i] == ' ' && buffer[i-1] != ' ')
      {
         spcCount++;
      }
   }

   char** ptr = (char**) getmem(sizeof(char*)*(spcCount+2));
   printf("memory allocated");
   //Used to differentiate separating spaces from unnecessary ones
   char temp;

   for(j = 0; j < len; j++){
      if(j == 0){
         ptr[ptrIndex] = &buffer[j];
         temp = buffer[j];
          ptrIndex++;
      }
      else{
         if(buffer[j] == '|'){
            temp = buffer[j];
            buffer[j] = '\0';
         }
         else if(buffer[j] != '|' && buffer[j] != '\0' && temp == '|'){
            ptr[ptrIndex] = &buffer[j];
            temp = buffer[j];
            ptrIndex++;

         }
      }
   }
 
   for (k = 0; k < ptrIndex; ++k){
      printf("%s \n", ptr[k]);
   }

	if((strncmp("LEDR",ptr[0],strlen(ptr[0])))==0)
	{	char buffer_out[65535];
	if((strncmp("ON",ptr[1],strlen(ptr[1])))==0)
        {printf("message received from edge: %s",buffer);
		gpio_set_mode(0,5,0);
		gpio_write(0,5,1);*/
		sprintf(buffer_out,"%s","LEDRON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 6);
	 
	}
			
		else if((strncmp("OFF",ptr[1],strlen(ptr[1])))==0)
	{
	gpio_set_mode(0,5,0);
	gpio_write(0,5,0);
	sprintf(buffer_out,"%s","LEDROFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	}
	else if((strncmp("LEDG",ptr[0],strlen(ptr[0])))==0)
	{	char buffer_out[65535];
	if((strncmp("ON",ptr[1],strlen(ptr[1])))==0)
        {      
		gpio_set_mode(0,12,0);
		gpio_write(0,12,1);
		sprintf(buffer_out,"%s","LEDGON");
printf("trying to send GREEN to cloud \n ");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 6);
	 printf("succesfully sent GREEN to cloud  ");
	}
			
		else if((strncmp("OFF",ptr[1],strlen(ptr[1])))==0)
	{
	gpio_set_mode(0,12,0);
	gpio_write(0,12,0);
	sprintf(buffer_out,"%s","LEDGOFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 7);
	 
	}
	}
	else if(strncmp("BUZZER",ptr[0],strlen(ptr[0]))==0)
		{ char buffer_out[65535];
if((strncmp("ON",ptr[1],strlen(ptr[1])))==0)
        {
		gpio_set_mode(0,3,0);
		gpio_write(0,3,1);
		sprintf(buffer_out,"%s","BUZZERON");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 8);
	 
	}
			
		else if((strncmp("OFF",ptr[1],strlen(ptr[1])))==0)
	{
	gpio_set_mode(0,3,0);
	gpio_write(0,3,0);
	sprintf(buffer_out,"%s","BUZZEROFF");
			udp_sendto (
	 slot,	 remote_ip, remote_port, buffer_out, 9);
	 
	}
	}

	else if((strncmp("TEMP",ptr[0],strlen(ptr[0])))==0)
		{ char buffer_out[65535];
if((strncmp("STATUS",ptr[1],strlen(ptr[1])))==0)
  			{

				char data[4];
				unsigned int integer = 0;
				unsigned int decimal = 0;
				float voltage,temp_c;
				adcread(39,(char*)data,4);
				unsigned int vol = *(unsigned int*)data;
				voltage = (vol*1.8/4096)*1000;
				temp_c = (voltage - 500) / 10;
				integer = (unsigned int)temp_c;
				decimal = (unsigned int)((temp_c - integer)*10);
				//kprintf("Voltage = %d mv\r\n",voltage);
				sprintf(buffer_out,"%d.%d",integer,decimal);
				udp_sendto ( slot, remote_ip,remote_port,	
	 buffer_out, strlen(buffer_out)	);
				//analog_read(257,1,);
			}
		else if(strncmp("SUBSCRIBE",ptr[1],strlen(ptr[1]))==0){
		 //       analog_subscribe(257,12,buzzeron);
		}
	}
	else
		printf("unknown function name");
 
  freemem((char*)ptr,sizeof(char*)*(spcCount+2));
//=======================================================================================================================


	
	printf("sending back one reply ");
	


}
}
