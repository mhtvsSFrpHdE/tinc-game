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

## How to use
- Download client installer https://github.com/mhtvsSFrpHdE/tinc-game/releases
- Install, search `tinc game` in start menu and open it
- Install a virtual network adapter  
  Menu bar, Tools, Manage virtual network adapter, Menu bar
- Join network, you can register on public server or use invite code  
  Menu bar, Network, Join network
- Select your network and click Connect
- Wait for Assigned IP change to something start with "10..."
- Click on Copy Refresh button, paste your IP to your friend also use tinc
- If your network is weak and haven't got IP for a while, click on Copy Refresh to refresh
- Use Menu bar, Network, Edit to set auto connect network on program start

## Drawbacks
Currently there is no contributors host "public server" for everybody to use.  
And we don't know how to prevent contributors from DDOS attack.  
A public network also for sure is full of hackers that do MAC Address Spoofing to steal your money and password.
