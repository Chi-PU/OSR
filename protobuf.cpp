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

std::string buildWishPullRequest(int user_id, int pull_count) {
    osr::ClientMessage msg;
    msg.set_type(osr::WISH_PULL);
    auto* req = msg.mutable_wish_pull();
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

std::string buildTokenAuthRequest(const std::string& jwt_token) {
    osr::ClientMessage msg;
    msg.set_type(osr::TOKEN_AUTH);
    auto* req = msg.mutable_token_auth();
    req->set_jwt_token(jwt_token);
    return msg.SerializeAsString();
}

std::string buildSignupRequest(const std::string& username, const std::string& password) {
    osr::ClientMessage msg;
    msg.set_type(osr::SIGNUP);
    auto* req = msg.mutable_signup();
    req->set_username(username);
    req->set_password(password);
    return msg.SerializeAsString();
}

std::string buildShopRequest(int user_id, int item_id, int quantity) {
    osr::ClientMessage msg;
    msg.set_type(osr::SHOP);
    auto* req = msg.mutable_shop();
    req->set_user_id(user_id);
    req->set_item_id(item_id);
    req->set_quantity(quantity);
    return msg.SerializeAsString();
}

osr::ServerResponse parseResponse(const std::string& data) {
    osr::ServerResponse resp;
    resp.ParseFromString(data);
    return resp;
}

} // namespace proto_helpers
