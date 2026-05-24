package com.example.demo.Controller.Response;

import java.util.HashMap;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

@Component
@Scope("prototype")
public class BasicJsonResponse {
	public BasicJsonResponse(){
		data.put(reasonKeyName, "");
		data.put(reasonCodeKeyName, 0);
	}
	
	private String reasonKeyName = "reason";
	private String reasonCodeKeyName = "reasonCode";
	
	Boolean success = false;
	public Boolean getSuccess() {
		return success;
	}
	public void setSuccess(Boolean success) {
		this.success = success;
	}

	// No set data to avoid confuse (set data usually is a setter that replace entire variable)
	// use getData.put('<key name>', <value>) to reuse exist data 
	HashMap<String, Object> data = new HashMap<>();
	public HashMap<String, Object> getData() {
		return data;
	}
	public void putData(String key, Object value) {
		data.put(key, value);
	}
	public void setReason(Object reason) {
		this.data.replace(reasonKeyName, reason);
	}
	public void setReasonCode(int reasonCode) {
		this.data.replace(reasonCodeKeyName, reasonCode);
	}
}
