
%.o:%.c
	g++ -c -o $@ $<
    
tunerctl : tuner.o serial.o
	g++ -o $@ $^
	
clean:
	rm -f *.o
	rm -f tunerctl
    
