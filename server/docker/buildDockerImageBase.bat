@REM Build base, comments in each docker file

docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
-t tinc-install-compiler:latest ^
-f dockerfile1-tinc-install-compiler.base ^
.

docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
-t tinc-install-jre:latest ^
-f dockerfile2-tinc-install-jre.base ^
.

docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
-t tinc-install-cert-receiver:latest ^
-f dockerfile3-tinc-install-cert-receiver.base ^
.

docker build ^
--build-arg HTTP_PROXY=%HTTP_PROXY% ^
--build-arg HTTPS_PROXY=%HTTPS_PROXY% ^
-t tinc-install-other:latest ^
-f dockerfile4-tinc-install-other.base ^
.
