@chcp 437
@netsh interface %1 set subinterface %2 mtu=%3 store=persistent > netsh437_result.txt
@exit
