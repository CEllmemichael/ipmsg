
STD=-std=gnu99
CC=gcc
all:
	$(CC) $(STD) tcp.c tcp_re.c udp_11.c ipmsg.c tools.c -o oipmag -lpthread

clean:
	rm oipmag
	
	
	
	
	
	
	
	
	
		
	
