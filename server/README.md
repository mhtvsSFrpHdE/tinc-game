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
- Docker Desktop (Windows) `v4.74.0`
- Run docker engine in Hyper-V mode instead of WSL2, may not work right now
- Copy [tinc-1.1pre18.tar.gz](https://tinc-vpn.org/download/) to `docker/Downloads/` directory
- Copy `spring-boot-project-0.0.1-SNAPSHOT.jar` to `docker/Downloads/` directory
- Edit `buildShell.bat`, change necessary environment values
- Edit `installDockerImage.bat`, change necessary environment values,  
  mainly `EXTERNAL_ADDRESS` and `LISTEN_PORT`, more see `dockerfile`
- Open `buildShell.bat`
- Run `bi`, it's same as `buildDockerImage.bat && installDockerImage.bat`
- If lucky enough container up and run, browser open `http://localhost:8080/api/account/invite`
- Confirm your free account is created, use the activation code in your tinc game client :D

If you behind company proxy to internet, in Docker Desktop
- Settings, Resources, Proxies, Containers Proxy: Same as host proxy
- Don't use ~/.docker/config.json, this value doesn't work on build time and will left sensitive proxy information to "container inspect Env section"
- [CLI plugins and other tools that use the Docker API directly must be configured separately with the `HTTP_PROXY`, `HTTPS_PROXY`, and `NO_PROXY` environment variables.](https://docs.docker.com/desktop/features/networking/)
