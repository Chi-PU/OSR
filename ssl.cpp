#include "ssl.h"
#include <iostream>

bool SSLLibrary::initialized_ = false;

void SSLLibrary::initialize() {
    if (!initialized_) {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
        initialized_ = true;
    }
}

void SSLLibrary::cleanup() {
    if (initialized_) {
        EVP_cleanup();
        ERR_free_strings();
        initialized_ = false;
    }
}

SSLContext::SSLContext(const std::string& ca_cert_path) : ctx_(nullptr) {
    SSLLibrary::initialize();

    const SSL_METHOD* method = TLS_client_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        throw std::runtime_error("Failed to create SSL context");
    }

    // Require TLS 1.2 minimum to match server
    SSL_CTX_set_min_proto_version(ctx_, TLS1_2_VERSION);

    if (!ca_cert_path.empty()) {
        // Verify the server's certificate against the provided CA cert
        if (!SSL_CTX_load_verify_locations(ctx_, ca_cert_path.c_str(), nullptr)) {
            SSL_CTX_free(ctx_);
            ctx_ = nullptr;
            throw std::runtime_error("Failed to load CA certificate: " + ca_cert_path);
        }
        SSL_CTX_set_verify(ctx_, SSL_VERIFY_PEER, nullptr);
    } else {
        // Self-signed server cert: skip verification
        SSL_CTX_set_verify(ctx_, SSL_VERIFY_NONE, nullptr);
    }
}

SSLContext::~SSLContext() {
    if (ctx_) {
        SSL_CTX_free(ctx_);
    }
}
