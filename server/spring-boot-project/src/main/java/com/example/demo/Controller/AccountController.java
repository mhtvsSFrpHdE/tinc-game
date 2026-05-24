package com.example.demo.Controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.demo.Controller.Response.BasicJsonResponse;

@RestController
public class AccountController {
	@Autowired
	private ApplicationContext context;
	
	 @GetMapping("/")
	 public BasicJsonResponse index() {
		 BasicJsonResponse result = new BasicJsonResponse();
		 return result;
	 }
}
