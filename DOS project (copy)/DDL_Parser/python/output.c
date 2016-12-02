bool8 deviceaccess(char * topic)
{
switch(topic)
{case "RGB_r_on": gpio_set_mode(0,11,1);
gpio_write(0,11,1);
case "RGB_r_off": gpio_set_mode(0,11),1);gpio_write(0,11,0);
case "RGB_g_on": gpio_set_mode(0,5,1);
gpio_write(0,5,0);
case "RGB_g_off":gpio_set_mode(0,5,1); 
gpio_write(0,5,0);
case "RGB_b_on":gpio_set_mode(0,6,1);
gpio_write(0,6,1);
case "RGB_b_off":gpio_set_mode(0,6,1);
gpio_write(0,6,0)
;case "readtemp":gpio_set_mode(1,39,0);
adcread(1,39)
;case "RGB_b_off":gpio_set_mode(0,6,1);
gpio_write(0,6,0)
;case "LED_r_status " :gpio_read(0,11);
case "LED_g_status ":gpio_read(0,5);
case "LED_b_status ":gpio_read(0,6);
case default: return FALSE;} 
 return TRUE
}
