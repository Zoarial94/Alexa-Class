#include "AlexaSkill.hpp"

Alexa::AlexaSkill::AlexaSkill(std::string appId, std::string path, std::string cert, std::string key) : applicationId(appId), path(path), certificate(cert), privateKey(key) {
	alexaRequest = std::make_shared<Alexa::Request>();
	alexaResponse = std::make_shared<Alexa::Response>();
}
