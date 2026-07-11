# How to build

## spring-boot-project
- Eclipse `2026-03 (4.39.0)`, `Spring Tools (aka Spring Tool Suite) 5.1.1` from Eclipse Marketplace
- Oracle jdk `17.0.19`
- Apache Maven `3.9.11`
- Environment variable `NETWORK_NAME` as your tinc service network config folder name
- Add where `tinc.exe` located path to environment variable `PATH`
- In Eclipse import as maven project, right click on project name, Run As, Maven build, Goal `package`
- Get `spring-boot-project-0.0.1-SNAPSHOT.jar` in `target` folder, it contains everything include `application.properties`
- Run and Debug: Find and open `SpringBootProjectApplication.java` in Eclipse,  
  right click on empty space choose Run As, Spring Boot App

## docker
If you behind company proxy to internet, in Docker Desktop
- Settings, Resources, Proxies, Containers Proxy: Same as host proxy
- Don't use ~/.docker/config.json, this value doesn't work on build time and will left sensitive proxy information to "container inspect Env section"
- Set `HTTP_PROXY`, `HTTPS_PROXY` in `buildShell.bat`
  - [CLI plugins and other tools that use the Docker API directly must be configured separately with the `HTTP_PROXY`, `HTTPS_PROXY`, and `NO_PROXY` environment variables.](https://docs.docker.com/desktop/features/networking/)

Build steps
- Docker Desktop (Windows) `v4.74.0`
- Run docker engine in Hyper-V mode instead of WSL2, may not work right now
- Copy [tinc-1.1pre18.tar.gz](https://tinc-vpn.org/download/) to `docker/Downloads/` directory
- Copy `spring-boot-project-0.0.1-SNAPSHOT.jar` to `docker/Downloads/` directory
- Edit `buildShell.bat`, change necessary environment values
- Edit `installDockerImage.bat`, change necessary environment values,  
  mainly `EXTERNAL_ADDRESS` and `LISTEN_PORT`, more see `dockerfile`
- Open `buildShell.bat`
- Run `buildDockerImageBase.bat`, for details see docker README.md "What are these weird dockerfile do"
- Run `bi`, it's same as `buildDockerImage.bat && installDockerImage.bat`
- If lucky enough container up and run, browser open `http://localhost:8080/api/account/invite`
- Confirm your free account is created, use the activation code in your tinc game client :D

Optional step if you want HTTPS
- You can't enable HTTPS on a exist container, must delete (will lose data) and recreate  
  Backup data folder `/usr/local/etc/tinc` and put back to HTTPS container  
  Docker doesn't support change environment variable once container is created
- Edit `pythonEnvironment.bat`, change python install dir
- There is `installCertReceiver.bat` and command `start "cert receiver" startCertReceiver.bat`,  
  this start a HTTP server in `localhost:8001` port allow you use `curl` from where certbot is running  
  to programmatically push HTTPS cert over LAN network **via unencrypted HTTP connection**  
  without have to manually copy file physically or via remote control  
  Once you've mastered that approach, you can continue
- Set environment variable before `installDockerImage.bat` (`docker run`),  
  `set TINC_GAME_SERVER_HTTPS=true`  
  or you can also just set and run `installDockerImage.bat` again
- Spring boot will fail to start but keep retry on each second
- On pc running certbot, push cert files to docker container `ip address:8000`, command like
  - `openssl x509 -outform der -in /path/to/fullchain.pem -out /path/to/fullchain_tinc.pem`
  - `openssl pkey -outform der -in /path/to/privkey.pem -out /path/to/privkey_tinc.pem`
  - `curl -X POST http://<docker container LAN IP address>:8000/upload -F "files=@/path/to/fullchain_tinc.pem"`
  - `curl -X POST http://<docker container LAN IP address>:8000/upload -F "files=@/path/to/privkey_tinc.pem"`
- Upload `fullchain_tinc.pem` by first, then `privkey_tinc.pem`.  
  There is a file watcher to reboot spring boot after `privkey.pem` uploaded
- Once cert exist in container, spring boot service will start listen on `8443` for HTTPS, `8080` for HTTP  
  **don't expose port `8000` outside LAN, this is very dangerous**  
  in fact you can download private key and cert back using 8000 port  
  use browser open `http://<docker container LAN IP address>:8000` and you will see
