CCFLAGS=-O3 -Wall
#CCFLAGS=-g -Wall

kc705_sim: kc705_sim.c
	gcc $(CCFLAGS) -o kc705_sim kc705_sim.c

clean:
	rm -f kc705_sim
