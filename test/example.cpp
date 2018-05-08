#include "AlexaSkill.hpp"
#include <iostream>
#include <map>
#include <functional>
#include <memory>

int main() {

	Alexa::AlexaSkill skill("INSERT APPLICATION ID HERE", "alexa", "certificate.pem", "private-key.pem");

	std::cout << "Initialized class" << std::endl;

	skill.setupServer();

	std::cout << "Server is Setup" << std::endl;

	std::cout << "Adding Intents" << std::endl;

	skill.intents["TestTalking"] = [](std::shared_ptr<Alexa::Request> request, std::shared_ptr<Alexa::Response> response) {
		std::cout << "IN TEST TALKING CALLBACK FUNCTION" << std::endl;
		response->speech("Talking has been tested");
		response->simpleCard("Testing", "Talking has been tested");
	};

	std::cout << "Starting Server" << std::endl;

	skill.start();

	return 0;
}
