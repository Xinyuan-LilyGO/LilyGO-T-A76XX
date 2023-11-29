#include <pgmspace.h>

// Amazon Root CA 
static const char AmazonRootCA[] PROGMEM = R"EOF(
-----BEGIN ROOT CERTIFICATE-----
// Please download the device certificate from AWS IoT Core when you create the thing and paste it here
//-----END ROOT CERTIFICATE-----
)EOF";