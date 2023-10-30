PORT=6969

./obj/server.o : ./src/server.c
	gcc -c $< -o $@
./obj/client.o : ./src/client.c
	gcc -c $< -o $@

./bin/server: ./obj/server.o
	gcc $< -o $@
./bin/client: ./obj/client.o
	gcc $< -o $@

clean:
	rm -f ./obj/*.o ./bin/*

server: ./bin/server
client: ./bin/client

run-server: ./bin/server
	@ ./bin/server $(PORT)

run-client: ./bin/client
	@ ./bin/client 127.0.0.1 $(PORT)
