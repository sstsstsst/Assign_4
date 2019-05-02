runserver: a4Server1
	chmod +x a4Server2
	./a4Server2 30005

a4Server1: cleanServer
	g++ -std=c++0x a4Server2.cpp -o a4Server2



runclient_1: a4Client2
	chmod +x a4Client2
	./a4Client2 127.0.0.1 30005 commandFile1.txt client_1_log.txt


runclient_2: a4Client2
	chmod +x a4Client2
	./a4Client2 127.0.0.1 30005 commandFile2.txt client_2_log.txt

a4Client2: cleanClient
	g++ -std=c++0x a4Client2.cpp -o a4Client2

cleanServer:
	rm -f a4Server2

cleanClient:
	rm -f a4Client2