import sys
import json

orig_stdout = sys.stdout
f = file('output.c', 'w')
sys.stdout = f

with open('ddl.json') as json_data:
	d=json.load(json_data)
		
	k=d['DDL']['Platform']
	filename="bool8 deviceaccess(char * topic)\n{\n"
	switch="switch(topic)\n{"

	a="case \"RGB_r_on\": gpio_set_mode(0,11,1);\ngpio_write(0,11,1);\n"
	b="case \"RGB_r_off\": gpio_set_mode(0,11),1);gpio_write(0,11,0);\n"
	c="case \"RGB_g_on\": gpio_set_mode(0,5,1);\ngpio_write(0,5,0);\n"
	d="case \"RGB_g_off\":gpio_set_mode(0,5,1); \ngpio_write(0,5,0);\n"
	e="case \"RGB_b_on\":gpio_set_mode(0,6,1);\ngpio_write(0,6,1);\n"
	f="case \"RGB_b_off\":gpio_set_mode(0,6,1);\ngpio_write(0,6,0)\n;"

	g="case \"readtemp\":gpio_set_mode(1,39,0);\nadcread(1,39)\n;"
	h= "case \"LED_r_status \" :gpio_read(0,11);\n"
	i= "case \"LED_g_status \":gpio_read(0,5);\n"
	j= "case \"LED_b_status \":gpio_read(0,6);\n"
	k= "case default: return FALSE;} \n return TRUE\n}"
	
	
	print filename+switch+a+b+c+d+e+f+g+f+h+i+j+k

sys.stdout = orig_stdout
#f.close()
print "complete"