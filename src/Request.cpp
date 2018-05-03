#define BOOST_SPIRIT_THREADSAFE
#include "AlexaSkill.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

void Alexa::Request::readJson(std::istream& raw) {
	pt::read_json(raw, json);
	std::stringstream ss;
	pt::json_parser::write_json(ss, json);
	std::cout << "JSON HAS BEEN READ: " << ss.str() << std::endl;
}

std::string Alexa::Request::getAppId() {
	std::string s = json.get<std::string>("session.application.applicationId");
	std::cout << "APPLICATION ID: " << s << std::endl;
	return s;
}

std::string Alexa::Request::getType() {
	std::string s = json.get<std::string>("request.type");
	std::cout << "REQUEST TYPE: " << s << std::endl;
	return s;
}

std::string Alexa::Request::getIntent() {
	std::string s = json.get<std::string>("request.intent.name");
	std::cout << "INTENT NAME: " << s << std::endl;
	return s;
}
