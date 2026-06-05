## tinc game
Software solution that help player under different router and NAT to connect together,  
just like how they connect physically with wired RJ45 or in same Wi-Fi

## tinc
In original tinc network peers are equal, no strict distinguish between server and client

### tinc as client
In tinc game, for client virtual network adapter and join network

### tinc as server
In tinc game, for server virtual network adapter and STUN / UDP hole punching service

It's best for it to have open ports in IPv4  
Although is possible to run and connect to it with IPv6,  
but why do you care UDP hole punching if you have IPv6 in both side

## tinc game client
GUI for tinc as client, you can register and connect to public tinc game server,  
or use your own tinc server by invite code from tinc command

By default tinc game client operates tinc to run in Layer 2 to enable IP auto configuraton  
This also allows network to be attack by APR spoofing which is dangerous for any hacker have Layer 2 access

## tinc game server
Spring boot HTTP/HTTPS web services

On client request, run `tinc -n <network name> invite <random user name>`,  
send result back to tinc game client

This server eliminated needs that each time new player come in,  
network owner need to manually type tinc invite command

Sub components, they must deploy in same docker image to function properly:
- tinc as server
- dnsmasq  
  For DHCP LAN IP automatically configuration
- cert receiver  
  Component of tinc game server for HTTPS cert update, a small bash script
