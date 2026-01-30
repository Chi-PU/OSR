#include "game.pb.h"
#include <string>

namespace proto_helpers {

std::string buildLoginRequest(const std::string& username, const std::string& password) {
    osr::ClientMessage msg;
    msg.set_type(osr::LOGIN);
    auto* req = msg.mutable_login();
    req->set_username(username);
    req->set_password(password);
    return msg.SerializeAsString();
}

std::string buildGachaPullRequest(int user_id, int pull_count) {
    osr::ClientMessage msg;
    msg.set_type(osr::GACHA_PULL);
    auto* req = msg.mutable_gacha_pull();
    req->set_user_id(user_id);
    req->set_pull_count(pull_count);
    return msg.SerializeAsString();
}

std::string buildChoiceRequest(int user_id, int choice, int scenario_id) {
    osr::ClientMessage msg;
    msg.set_type(osr::GAME_ACTION);
    auto* req = msg.mutable_game_action();
    req->set_user_id(user_id);
    req->set_choice(choice);
    req->set_scenario_id(scenario_id);
    return msg.SerializeAsString();
}

osr::ServerResponse parseResponse(const std::string& data) {
    osr::ServerResponse resp;
    resp.ParseFromString(data);
    return resp;
}

} // namespace proto_helpers
