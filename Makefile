
%.o:%.c
	g++ -c -o $@ $<
    
tunermon : tunermon.o serial.o
	g++ -o $@ $^

tunerctl: tunerctl.o serial.o
	g++ -o $@ $^

ampctl: ampctl.o serial.o
	g++ -o $@ $^

	

clean:
	rm -f *.o
	rm -f tunerctl
	rm -f tunermon
	rm -f ampctl

all: tunerctl ampctl tunermon

