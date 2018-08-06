#define BOOST_SPIRIT_THREADSAFE
#include "server_https.hpp"
#include "AlexaSkill.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using HttpsServer = SimpleWeb::Server<SimpleWeb::HTTPS>;
namespace pt = boost::property_tree;

void Alexa::AlexaSkill::setupServer() {
	server = std::make_unique<HttpsServer>(certificate, privateKey);
	//std::cout << "Server has cert and key" << std::endl;
	server->config.port = port;
	//std::cout << "Port has been set" << std::endl;

	server->default_resource["GET"] = [](std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request) {
		response->write("Why are you here?");
	};
	//std::cout << "Default GET set" << std::endl;

	server->resource["^/" + path + "$"]["POST"] = [this](std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request) {
		//std::cout << "ALEXA REQUEST RECIEVED" << std::endl;
		alexaRequest->readJson(request->content);
		if(alexaRequest->getAppId() == applicationId) {
			if(alexaRequest->getType() == "IntentRequest") {
				std::cout << "ALEXA REQUEST: IntentRequest" << std::endl;
				if(intents.find(alexaRequest->getIntent()) != intents.end()) {
					intents[alexaRequest->getIntent()](alexaRequest, alexaResponse);
					*response << alexaResponse->getResponse();
				} else {
					std::cout << "INVALID INTENT NAME: " << alexaRequest->getIntent() << std::endl;
					*response << alexaResponse->say("Invalid intent name");
				}
			} else {
				std::cout << "INVALID REQUEST TYPE FROM ALEXA: " << alexaRequest->getType() << std::endl;
				*response << alexaResponse->say("Invalid request type");
			}
		} else {
			std::cout << "INVALID APPLICATION ID: " << alexaRequest->getAppId() << std::endl;
		}
	};
}

void Alexa::AlexaSkill::start() {
	setupServer();
	std::thread server_thread([this]() {
			// Start server
			server->start();
			});
	server_thread.join();
}
