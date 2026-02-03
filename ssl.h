#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <stdexcept>

// RAII wrapper for OpenSSL library init/cleanup
class SSLLibrary {
public:
    static void initialize();
    static void cleanup();
private:
    static bool initialized_;
};

// RAII wrapper for SSL_CTX (client-side TLS 1.2 context)
class SSLContext {
public:
    // ca_cert_path: optional path to CA certificate for server verification.
    // If empty, server certificate verification is disabled (self-signed).
    explicit SSLContext(const std::string& ca_cert_path = "");
    ~SSLContext();

    SSLContext(const SSLContext&) = delete;
    SSLContext& operator=(const SSLContext&) = delete;

    SSL_CTX* get() const { return ctx_; }

private:
    SSL_CTX* ctx_;
};
