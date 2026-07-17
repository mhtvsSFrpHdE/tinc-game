## What is this?
https://www.tinc-vpn.org/  
tinc is the sole surviving project being maintained that support from Linux to Windows XP to Windows 10 (11).  
To utilize it you need to be experienced in operating system and network.

tinc-game some extent restricts your choices among all config possibilities.  
The result is in one-click to join public network, get LAN IP address, start gaming.

What have been done:  
- A public server (docker) that accept HTTP/HTTPS requests
- Client join network by register, send requests to server
- On server receive request, server type tinc command, get invite code, send back to client
- Client use invite code join network
- A DNS server give client DHCP LAN IP address
- Another client join network, also get LAN IP address
- Game!

## Watchout layer 2 network!!!
Necessary for DHCP IP auto configuration to work.  
In original tinc network layer 2 is optional and should use Layer 3 mode for better security,  
which is manually IP configuration

It's impossible to manage IP manually in large public network

### ARP attack
Network device won't do verification when they do IP auto configuration and LAN IP discover  
and usually use first result they received from broadcast

This lead to man in the middle attack,  
hackers can read and modify unencrypted message goes through network without being notice

### Port scanning and remote code execute
For commercial operating system like Windows, many unknown service listen on ports by default  
Some of them may contains security vulnerability that allow hackers execute unauthorized command on computer  
Windows firewall is no use because Microsoft let these service get pass firewall

NAT somehow protected these device from being seen, but in tinc game public network you're exposed

## How to use client
- Download client installer https://github.com/mhtvsSFrpHdE/tinc-game/releases
- Install, search `tinc game` in start menu and open it
- Install a virtual network adapter  
  Menu bar, Tools, Manage virtual network adapter
- Join network, you can register on public server or use invite code  
  Menu bar, Network, Join network
- Select your network and click Connect
- Wait for Assigned IP change to something start with "10..."
- Click on Copy Refresh button, paste your IP to your friend also use tinc
- If your network is weak and haven't got IP for a while, click on Copy Refresh to refresh
- Use Menu bar, Network, Edit to set auto connect network on program start

## How to use server
- Download server with docker  
  `sudo docker pull jtfeejflcdxj/tinc-game-server:latest`
- Create a script to start server  
  <details>
      <summary>Linux</summary>
    
    ```
    export LISTEN_PORT=655
    export EXTERNAL_ADDRESS="<your ip or domain>"
    export CONTAINER_NAME=tinc-game-server
    sudo docker stop $CONTAINER_NAME
    sudo docker rm $CONTAINER_NAME
  
    sudo docker run \
    -e EXTERNAL_ADDRESS=$EXTERNAL_ADDRESS \
    -e LISTEN_PORT="$LISTEN_PORT" \
    -e TINC_GAME_SERVER_HTTPS=$TINC_GAME_SERVER_HTTPS \
    -e IPV4_SUBNET_RANGE_BEGIN="10.77.1.65" \
    -p $LISTEN_PORT:$LISTEN_PORT -p $LISTEN_PORT:$LISTEN_PORT/udp \
    -p 8080:8080 \
    -p 8443:8443 \
    -p 8000:8000 \
    --cap-add=NET_ADMIN --device /dev/net/tun:/dev/net/tun \
    --restart unless-stopped \
    --stop-timeout=45 \
    -d \
    --name $CONTAINER_NAME jtfeejflcdxj/$CONTAINER_NAME
    ```
  
  </details>
  <details>
      <summary>Windows</summary>
    https://github.com/mhtvsSFrpHdE/tinc-game/blob/main/server/docker/installDockerImage.bat
  </details>

- Forward port 655, 8080 for HTTP, 8443 for HTTPS, don't forward 8000
- To enable HTTPS, see wiki "How to build", "Server", "HTTPS"

## Lean more on wiki
https://github.com/mhtvsSFrpHdE/tinc-game/tree/main/wiki

## Drawbacks
Currently there is no contributors host "public server" for everybody to use.  
And we don't know how to prevent contributors from DDOS attack.  
A public network also for sure is full of hackers that do MAC Address Spoofing to steal your money and password.

Client (GUI) still need time to support old system like Windows XP.
