package com.example.demo.Service;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Random;

import org.springframework.stereotype.Repository;

import com.example.demo.Controller.Response.BasicJsonResponse;

@Repository
public class AccountService {
	/**
	 * Invite random length 13 unique account name to network
	 * 
	 * @param networkName
	 * @return data\inviteCode: Invite code output from tinc process
	 */
	public BasicJsonResponse Invite(String networkName) {
		BasicJsonResponse inviteResult;
		while (true) {
			var accountName = GenerateRandomAccountName();
			inviteResult = Invite_TincServer(networkName, accountName);
			if (inviteResult.getSuccess()) {
				break;
			}
		}
		return inviteResult;
	}

	/**
	 * Reason 1: User already exists (joined network)
	 * 
	 * @param networkName
	 * @param accountName
	 * @return
	 */
	private BasicJsonResponse Invite_TincServer(String networkName, String accountName) {
		var result = new BasicJsonResponse();
		try {
			var processBuilder = new ProcessBuilder("tinc", "-n", networkName, "invite", accountName);
			processBuilder.redirectErrorStream(true);
			var process = processBuilder.start();
			var reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
			var lines = new ArrayList<String>();
			String line;
			while ((line = reader.readLine()) != null) {
				// "A host config file for <username> already exists!"
				var findUserExists = accountName + " already exists";
				if (line.contains(findUserExists)) {
					result.setReasonCode(1);
				}
				lines.add(line);
			}
			if (result.getReasonCode() < 1) {
				result.putData("inviteCode", lines.get(0));
				result.setSuccess(true);
				return result;
			} else {
				return result;
			}
		} catch (Exception e) {
			result.setReasonCode(0);
			result.setReason(e.toString());
			return result;
		}
	}

	private String GenerateRandomAccountName() {
		var randomFrom = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
		var stringBuilder = new StringBuilder();
		var random = new Random();
		for (int i = 0; i < 13; i++) {
			stringBuilder.append(randomFrom.charAt(random.nextInt(randomFrom.length())));
		}
		var result = stringBuilder.toString();
		return result;
	}
}
