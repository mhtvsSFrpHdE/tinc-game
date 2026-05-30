@REM -t :latest to build to same image instead of create different docker image each time
docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
--build-arg BUILD_CPU_CORE=%BUILD_CPU_CORE% ^
-t tinc-game-server:latest .
