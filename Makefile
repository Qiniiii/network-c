objects = serverA.o serverB.o serverC.o aws.o client.o
exes = serverA serverB serverC aws client

%.o: %.cpp
	g++ -g -c -o $@ $^
	
all: $(objects)
	g++ -g -o serverA serverA.o
	g++ -g -o serverB serverB.o
	g++ -g -o serverC serverC.o
	g++ -g -o aws aws.o
	g++ -g -o client client.o

serverA: 
	./serverA
serverB: 
	./serverB
serverC: 
	./serverC
aws:
	./aws

.PHONY: clean aws serverA serverB serverC

clean:
	rm $(objects) $(exes)
