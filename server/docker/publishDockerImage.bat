@REM -t :latest to build to same image instead of create different docker image each time
@REM -f specify non default dockerfile name to build
@REM --build-arg pass exist environment variable to builder

docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
--build-arg BUILD_CPU_CORE=%BUILD_CPU_CORE% ^
--build-arg TINC_GAME_SERVER_HTTPS=false ^
-t jtfeejflcdxj/tinc-game-server:latest .

docker push <account name>/tinc-game-server:latest
