@chcp 437
@netsh interface "%3" set subinterface "%1" mtu="%2" store=persistent > netsh437_result.txt
@exit
