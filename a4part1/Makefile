runserver: a4Server1
	chmod +x a4Server1
	./a4Server1 30005

a4Server1: cleanServer
	g++ -std=c++0x a4Server1.cpp -o a4Server1

runclient: a4Client1
	chmod +x a4Client1
	./a4Client1 127.0.0.1 30005 commandFile1.txt

a4Client1: cleanClient
	g++ -std=c++0x a4Client1.cpp -o a4Client1

cleanServer:
	rm -f a4Server1

cleanClient:
	rm -f a4Client1