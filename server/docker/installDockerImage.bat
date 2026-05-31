set LISTEN_PORT=655
set EXTERNAL_ADDRESS="localhost"
set CONTAINER_NAME=tinc-game-server
docker stop %CONTAINER_NAME%
docker rm %CONTAINER_NAME%

@REM --cap-add NET_ADMIN and --device allow tinc to access tun device inside docker container
@REM /udp open udp port
@REM --name specify fixed container name handy for maintenance
@REM -d after run (will also start automatically), let container run in background
docker run ^
-e EXTERNAL_ADDRESS=%EXTERNAL_ADDRESS% ^
-e LISTEN_PORT="%LISTEN_PORT%" ^
-e TINC_GAME_SERVER_HTTPS=%TINC_GAME_SERVER_HTTPS% ^
-p %LISTEN_PORT%:%LISTEN_PORT% -p %LISTEN_PORT%:%LISTEN_PORT%/udp ^
-p 8080:8080 ^
-p 8443:8443 ^
-p 8000:8000 ^
--cap-add=NET_ADMIN --device /dev/net/tun:/dev/net/tun ^
-d ^
--name %CONTAINER_NAME% %CONTAINER_NAME%
