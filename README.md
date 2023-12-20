# Aplicacio sobre sockets UDP

## Descripció
L’aplicació es una mena de registre de sessió. El funcionament és el següent; el client indica que vol fer un registre proporcionant nom d’usuari, nom real i contrasenya. 
Desprès, el client pot tornar a fer un registre per a tants usuaris com vulgui o iniciar sessió especificant nom d’usuari i contrasenya. 
Si s’inicia sessió el servidor retorna un salut amb el nom real del usuari.

## Compilació
Per a compilar el servidor i client alhora fem:
```
make both
```
Per a compilar individualment el servidor o el client:
```
make server
// o
make client
```

## Execució
Els binaris es desen al directori `./bin/`. Per a executar el servidor:
```
./bin/server <un port>
```
Per a executar el client:
```
./bin/client <ip del servidor> <port d'escolta del servidor>
```
