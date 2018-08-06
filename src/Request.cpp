#define BOOST_SPIRIT_THREADSAFE
#include "AlexaSkill.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

void Alexa::Request::readJson(std::istream& raw) {
	pt::read_json(raw, mJson);
	std::stringstream ss;
	pt::json_parser::write_json(ss, mJson);
	//print raw data for testing/debugging
	//std::cout << std::endl << std::endl << ss.str() << std::endl << std::endl;
}

std::string Alexa::Request::getAppId() {
	std::string s = mJson.get<std::string>("session.application.applicationId");
	//std::cout << "APPLICATION ID: " << s << std::endl;
	return s;
}

std::string Alexa::Request::getType() {
	std::string s = mJson.get<std::string>("request.type");
	//std::cout << "REQUEST TYPE: " << s << std::endl;
	return s;
}

std::string Alexa::Request::getIntent() {
	std::string s = mJson.get<std::string>("request.intent.name");
	//std::cout << "INTENT NAME: " << s << std::endl;
	return s;
}
