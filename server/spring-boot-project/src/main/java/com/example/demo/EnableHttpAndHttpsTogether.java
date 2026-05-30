package com.example.demo;


import org.apache.catalina.connector.Connector;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.tomcat.servlet.TomcatServletWebServerFactory;
import org.springframework.boot.web.server.WebServerFactoryCustomizer;
import org.springframework.stereotype.Component;

@Component
public class EnableHttpAndHttpsTogether implements WebServerFactoryCustomizer<TomcatServletWebServerFactory> {
    @Value("${server.http.port}")
    private int httpPort;
    
	@Override
	public void customize(TomcatServletWebServerFactory factory) {
	    Connector connector = new Connector(TomcatServletWebServerFactory.DEFAULT_PROTOCOL);
	    connector.setPort(httpPort);
		factory.addAdditionalConnectors(connector);
	}
}
