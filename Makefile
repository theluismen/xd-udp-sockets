PORT=6969

./obj/server.o : ./src/server.c
	gcc -Iinc -c $< -o $@
./obj/client.o : ./src/client.c
	gcc -Iinc -c $< -o $@
./obj/sopor.o : ./src/sopor.c
	gcc -Iinc -c $< -o $@

./bin/server: ./obj/server.o ./obj/sopor.o
	gcc $< ./obj/sopor.o -o $@
./bin/client: ./obj/client.o ./obj/sopor.o
	gcc $< ./obj/sopor.o -o $@

clean:
	rm -f ./obj/*.o ./bin/*

server: ./bin/server
client: ./bin/client
both:	./bin/server ./bin/client
	
run-server: ./bin/server
	@ ./bin/server $(PORT)

run-client: ./bin/client
	@ ./bin/client 127.0.0.1 $(PORT)
