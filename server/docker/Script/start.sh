#!/bin/bash

# tinc interface script
echo "Write tincUpScript:"
echo "#!/bin/sh" > $tincUpScript
echo "" >> $tincUpScript
echo "ip addr add $IPV4_SERVER_ADDRESS/$IPV4_SUBNET_PREFIX dev \$INTERFACE" >> $tincUpScript
echo "# Necessary to do interface up here otherwise cause Can't write to Linux tun/tap device (tun mode) /dev/net/tun: Input/output error" >> $tincUpScript
echo "ip link set \$INTERFACE up" >> $tincUpScript
echo "service dnsmasq restart" >> $tincUpScript
echo "" >> $tincUpScript
echo "#echo 'Unconfigured tinc-up script, please edit '$0'!'" >> $tincUpScript
echo "" >> $tincUpScript
echo "#ifconfig $INTERFACE <your vpn IP address> netmask <netmask of whole VPN>" >> $tincUpScript
echo "" >> $tincUpScript
cat $tincUpScript

# dnsmasq config
echo "Write DHCP settings:"
echo "port=0" > $dnsmasqConfig
echo "interface=tinc_docker" >> $dnsmasqConfig
echo "dhcp-range=$IPV4_SUBNET_RANGE_BEGIN,$IPV4_SUBNET_RANGE_END,$IPV4_SUBNET_MASK,$DHCP_EXPIRE" >> $dnsmasqConfig
echo "dhcp-option=option:router" >> $dnsmasqConfig
echo "dhcp-option=option:dns-server" >> $dnsmasqConfig
cat "$dnsmasqConfig"
echo ""

tinc -n $NETWORK_NAME set ListenAddress 0.0.0.0 $LISTEN_PORT
tinc -n $NETWORK_NAME add ListenAddress :: $LISTEN_PORT
tinc -n $NETWORK_NAME set Address $EXTERNAL_ADDRESS $LISTEN_PORT
tinc -n $NETWORK_NAME set PMTUDiscovery $PMTU_DISCOVERY

cleanup() {
    tincPid=$(lsof -t -i:$LISTEN_PORT)
    springBootPid=$(lsof -t -i:8080)
    kill -15 $springBootPid
    tail --pid=$springBootPid -f /dev/null
    tinc -n tinc_docker stop
    tail --pid=$tincPid -f /dev/null
    echo "Safe shutdown success"
}

trap 'cleanup' SIGTERM

tinc -n tinc_docker start
python -m uploadserver 8000 --allow-replace &
# If https false exit
# On https true, wait for privkey.pem to be exist
# Watch this file once it updated, reboot spring boot
bash -c "if [ $TINC_GAME_SERVER_HTTPS = "false" ]; then exit; fi; until [ -f "/home/root/privkey.pem" ]; do sleep 1; done; until inotifywait "/home/root/privkey.pem" -e close_write; do kill -15 \$(lsof -t -i:8080); done" &
bash -c "until java -jar spring-boot-project-0.0.1-SNAPSHOT.jar; do sleep 1; done" &

wait $!
