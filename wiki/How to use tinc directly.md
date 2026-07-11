# How to use tinc directly
Although having GUI is good, you may want to use tinc to game without GUI.

## Install from source on Linux (optional)
tinc `1.1pre18` isn't available yet in stable apt source, need to compile from source  
There are few places hosts build guide and some use `meson` while others don't  
tinc game internally not use meson style for simplicty,  
if one day the build method obsolute, some link may help

https://www.tinc-vpn.org/documentation-1.1/Libraries.html  
https://github.com/gsliepen/tinc/blob/1.1/INSTALL.md  
https://gist.github.com/zingmars/a19c509b58ae5e463a204f9a692a0b65  
https://nwgat.ninja/quick-easy-tinc-1-1-2/

Build from source
- [tinc-1.1pre18.tar.gz](https://www.tinc-vpn.org/download/)
- `sudo apt-get install build-essential libncurses5-dev libreadline6-dev libzlcore-dev zlib1g-dev liblzo2-dev libssl-dev`
- `./configure`
- `make`
- `sudo make install`

## Configure first node (server)
- Create pid folder for tincd, without this seems failed to start daemon  
  `sudo mkdir -p /usr/local/var/run`
- `sudo tinc -n NETWORK_NAME init server`
- How other node connect to this node, external address / domain and port  
  `sudo tinc -n NETWORK_NAME add Address <DOMAIN> <PORT>`
- Layer 2 IPv4 static address for server itself  
  necessary for DHCP to work, but optional if use static address  
  If you don't want other node see and communicate with server  
  like ping or ssh to your server, use a firewall to block incoming package  
 `sudo nano /usr/local/etc/tinc/NETWORK_NAME/tinc-up`
  ```
  ip addr add LAN_IP/24 dev $INTERFACE
  # Necessary to do interface up here otherwise cause Can't write to Linux tun/tap device (tun mode) /dev/net/tun: Input/output error
  ip link set $INTERFACE up
  systemctl restart dnsmasq_NETWORK_NAME
  ```
- Run a DHCP server on address set in `tinc-up`  
  dnsmasq is OK for this purpose, here is a DHCP only instance example  
  you need to start dnsmasq after `tinc-up`
  ```
  port=0
  interface=tinc_docker
  dhcp-range=10.77.1.2,10.77.255.254,255.255.0.0,12h
  dhcp-option=option:router
  dhcp-option=option:dns-server
  ```
- Enable L2 network  
  `sudo tinc -n NETWORK_NAME set Mode switch`
- Start tinc node  
  `sudo tinc -n NETWORK_NAME start`  
  Start and show debug output: `sudo tinc -n NETWORK_NAME start -D -d3`  
  Confirm firewall and port forward for TCP and UDP 655
- Start dnsmasq
- Invite other node to network  
  `sudo tinc -n NETWORK_NAME invite CLIENT_NAME`

## Connect to network, here is a Windows client example
- Join with custom config dir or network name  
  `-n` let tinc decide where to store network config  
  `--config=` specify network config path manually
  - `tinc -n NETWORK_NAME join <invite code>`  
  `tinc -n NETWORK_NAME start`
  - `tinc --config=<Config dir> join <invite code>`  
  `tinc --config=<Config dir> start`
  - Start and show debug output  
    `tinc -n NETWORK_NAME start -D -d3`
- Wait for DHCP address or set address manually in control panel
- Try access server or other nodes also in network
