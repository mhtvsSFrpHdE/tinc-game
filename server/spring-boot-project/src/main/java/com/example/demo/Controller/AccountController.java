package com.example.demo.Controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.demo.Controller.Response.BasicJsonResponse;
import com.example.demo.Service.AccountService;

@RestController
@RequestMapping("/account")
public class AccountController {
	@Autowired
	private ApplicationContext context;

	private String tincNetworkName = System.getenv("NETWORK_NAME");

	@GetMapping("/invite")
	public BasicJsonResponse invite() {
		var accountService = context.getBean(AccountService.class);
		var result = accountService.Invite(tincNetworkName);
		return result;
	}
}
