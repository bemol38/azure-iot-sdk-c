// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// CAVEAT: This sample is to demonstrate azure IoT client concepts only and is not a guide design principles or style
// Checking of return codes and error values shall be omitted for brevity.  Please practice sound engineering practices
// when writing production code.

#include <stdio.h>
#include <stdlib.h>

#include "iothub.h"
#include "iothub_device_client_ll.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/shared_util_options.h"

/* This sample uses the _LL APIs of iothub_client for example purposes.
Simply changing the using the convenience layer (functions not having _LL)
and removing calls to _DoWork will yield the same results. */

// The protocol you wish to use should be uncommented
//
#define SAMPLE_MQTT
//#define SAMPLE_MQTT_OVER_WEBSOCKETS
//#define SAMPLE_AMQP
//#define SAMPLE_AMQP_OVER_WEBSOCKETS
//#define SAMPLE_HTTP

// If using an OpenSSL ENGINE uncomment and modify the line below
//#define SAMPLE_OPENSSL_ENGINE "pkcs11"
#define SAMPLE_OPENSSL_ENGINE "aziot_keys"

#ifdef SAMPLE_MQTT
    #include "iothubtransportmqtt.h"
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    #include "iothubtransportmqtt_websockets.h"
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    #include "iothubtransportamqp.h"
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    #include "iothubtransportamqp_websockets.h"
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    #include "iothubtransporthttp.h"
#endif // SAMPLE_HTTP

#ifdef MBED_BUILD_TIMESTAMP
    #define SET_TRUSTED_CERT_IN_SAMPLES
#endif // MBED_BUILD_TIMESTAMP

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
    #include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES

/* Paste in the your x509 iothub connection string  */
/*  "HostName=<host_name>;DeviceId=<device_id>;x509=true"                      */
static const char* connectionString = "HostName=hbb.azure-devices.net;DeviceId=7780f24f-8109-4ccb-ab53-11bcc030afb1;x509=true";

static const char* x509certificate =
"-----BEGIN CERTIFICATE-----\nMIIEZjCCA06gAwIBAgIUIhvyYVNOzZulqaNjDKvd2ueMBrswDQYJKoZIhvcNAQEL\nBQAwSDELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSMwIQYDVQQD\nExpCLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMjAeFw0yMjAzMTcxODE5MzRaFw0y\nNDAzMTcxODE5MzRaMC8xLTArBgNVBAMTJDc3ODBmMjRmLTgxMDktNGNjYi1hYjUz\nLTExYmNjMDMwYWZiMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALVN\nHhrElVHxHJCBYxahi2+0pp2pdJmLgU9ogoa0nOx76NQd1lzFK20x+uD93qVSHQv6\nfN24wXduEqlcWvFcsB320ZbMUwwZ40hNVKcRYSzsyxAbr/Ov5O5vi89a7HGKffR7\nW4/4IhfFQKiTqhDKHiR255tkUCmKeRNqhcflFiVY2dUg7fIKBDZPX4UrdflXDLTL\nABB6w715A2wKcRpSl7xfFa1FVVyeH1d3AVeT5FvjHw7y9shOkXsLtty3piRma0ez\n5Ot2ZYfcWrClDZyRCuMtO6JrFJ7YwSlJ9icwqzbwyg+R59d2etzP6PXKP2Gc/YrE\nfak5NP8NO7vmHkl/O7cCAwEAAaOCAV8wggFbMAwGA1UdEwEB/wQCMAAwHQYDVR0O\nBBYEFExZLQM5Mn6WAzPkK7HYOfdf/mYFMB8GA1UdIwQYMBaAFDb0QaEeb2kGoOTb\nGYEqtDf6fxixMA4GA1UdDwEB/wQEAwIFoDAWBgNVHSUBAf8EDDAKBggrBgEFBQcD\nAjCBkAYIKwYBBQUHAQEEgYMwgYAwLQYIKwYBBQUHMAGGIWh0dHA6Ly9vY3NwLmRl\nbW8ub25lLmRpZ2ljZXJ0LmNvbTBPBggrBgEFBQcwAoZDaHR0cDovL2NhY2VydHMu\nZGVtby5vbmUuZGlnaWNlcnQuY29tL0IuQnJhdW4tUE9DLVJTQS1Jb1QtSUNBLUcy\nLmNydDBQBgNVHR8ESTBHMEWgQ6BBhj9odHRwOi8vY3JsLmRlbW8ub25lLmRpZ2lj\nZXJ0LmNvbS9CLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMi5jcmwwDQYJKoZIhvcN\nAQELBQADggEBAJlVxyCfvVYmFAdJQZqHHjbdvzXYhlZsX7C/Ie6lnsQt/4TCVl2d\ndPoGm9IDYCryMEc7WD5o9ui/YpNMmiI+fbyfhTY8dAky+2esX4MXlnO1H04MENqC\nN1+DUooSIUuyB9bv0pDday64q1KdwWC8pJg+21Rm/8S6BabtRmSet59CR9TTqJMt\nZVqgQE57xayiktpSEQ8A7wrSP2sGhqwuXIhlkwoAQkdEhQPAGFrM/PBpz2uS9NcC\n8dY+p7y6iPzzknK5BpLzE73doRUGuujqiGeFRs14ej9yC+2V7kAv34jG5s0edjHp\ng+/Y3rnl0U1GYMu/BSne09MpWxNnIvOVzSY=\n-----END CERTIFICATE-----\n-----BEGIN CERTIFICATE-----\nMIIFYDCCA0igAwIBAgIUMnX35u2iceM49L9cU0LHS290RkcwDQYJKoZIhvcNAQEL\nBQAwSjELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSUwIwYDVQQD\nExxCIEJyYXVuIFBPQyAtIFJTQSBSb290IENBIEcyMB4XDTIyMDMwNDEzMjYwOFoX\nDTQxMDMzMDIzMDAwMFowSDELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVu\nIFNFMSMwIQYDVQQDExpCLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMjCCASIwDQYJ\nKoZIhvcNAQEBBQADggEPADCCAQoCggEBALdEOcXP0KTOcgFN742D9y3k1JQOYJDX\nEXdAXc4bKBPwDVHlSrbURB1dw/RXBrLzKb8o6/ZReUGGQEQpLcBJoa+4eP6hwmgH\nfapbY/aGvMC2zRLOliniwsxisTp6KBkhr9kalaFKRg2NManupfHqsIXsj89pEkiL\nqg8t07aiN8CQXW8j4oWxyPkWHzqJBpv5PzEy/ovZiKlrWme7J/04UxaRahucKFEe\nQM0F5WB7B9xFXskqBwEH6R0o39JYKC9iACkLdhTZW7KMKn9ZNU1nV+ick6syxPmR\ndPDI4z0B8e3HcVEm8Qq3PbCuosZnURjdJv+erWdHW2HqVFZ9ZQVN0X0CAwEAAaOC\nAT4wggE6MA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFDb0QaEeb2kGoOTbGYEq\ntDf6fxixMB8GA1UdIwQYMBaAFB0rS6ZK9yah92zluFaTVIsf35DnMA4GA1UdDwEB\n/wQEAwIBhjCBiQYIKwYBBQUHAQEEfTB7MC0GCCsGAQUFBzABhiFodHRwOi8vb2Nz\ncC5kZW1vLm9uZS5kaWdpY2VydC5jb20wSgYIKwYBBQUHMAKGPmh0dHA6Ly9jYWNl\ncnRzLmRlbW8ub25lLmRpZ2ljZXJ0LmNvbS9CQnJhdW5QT0MtUlNBUm9vdENBRzIu\nY3J0MEsGA1UdHwREMEIwQKA+oDyGOmh0dHA6Ly9jcmwuZGVtby5vbmUuZGlnaWNl\ncnQuY29tL0JCcmF1blBPQy1SU0FSb290Q0FHMi5jcmwwDQYJKoZIhvcNAQELBQAD\nggIBAFZgWRr0mWfz9eClCjnyE1w8zLUhkbsOPhF7+IoEWu5iYh5Z11l44GUx1k3W\n/eoq/kjM8hgJaoGBsVuYcGs/rMBLh9m9VRDx2TUAKfUfSo2vUsNHUeWz+lONYaNw\nck/YNyobuFcR5SXctHMVyV+dlAiD5CntL/uUf69Yrm5bh3YRqj4Se4qWF+V46vjP\n8qymtma5gnPR2KweiqmS0oLGNbjomEIWX0ObFgjjdOLPsrx/OH0EMxp1vq0LKvlM\n2aJK+k8VN4JhVJczBYElIanO7GAPTLPHnev+GU6pYl7r++eXUcX8qDUeiVlsOlYg\nXKh7fD//sH2n/vDQiPaVbakZscmoXVqxgKeF/eyhoLH4rT7+6Xd2LELV0+YjFszj\nudh+FEGOUEg7Z6hOa2xFj7AC2zuHjwemu7mTcV7Y4lxP3e1M8KIpWszkEtMjXdJ0\nqlXO5/gxkjXcCzh+kpCilknigxV03ASe9J3w8m8GsAO30ggF9fe0q7FTVZTvub3Z\nOdbufMOd7XsJabfT/pDJSj5Pfgo4NsBCVaqaqnGs+HdMwwX5h0GBWTdAGO9eZzTb\nVzx2pl6IHw7hIj+LK7BSyqP1VTV2SiDYe1Xm9LHZRyd3fu9lpUosJ3cOTC9S8y9O\nNgfSkdaXEupq5YuZmPvt8az0KmWjcex/3pCJOCvK+2mg3joI\n-----END CERTIFICATE-----\n-----BEGIN CERTIFICATE-----\nMIIFZjCCA06gAwIBAgIUcGx1PLOwiEVnvF+cs9RIe98YiWgwDQYJKoZIhvcNAQEL\nBQAwSjELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSUwIwYDVQQD\nExxCIEJyYXVuIFBPQyAtIFJTQSBSb290IENBIEcyMCAXDTIyMDMwNDEzMjE1NVoY\nDzIwNTIwMzA0MTMxOTAwWjBKMQswCQYDVQQGEwJERTEUMBIGA1UEChMLQi4gQnJh\ndW4gU0UxJTAjBgNVBAMTHEIgQnJhdW4gUE9DIC0gUlNBIFJvb3QgQ0EgRzIwggIi\nMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDVkyTSWxg9ii9Z/41rOelcWDaS\nzvG7zHaHqiG1FJM0BUhjhz8nRVEfCM5XqbxoKsky+az6TcKsUwFw4Zh4bbeT5cLR\nqw7R/9SOWg7qDByMVr4NfbeZvIhQ/PDRsOvQ2rJYqsq1qsIIHapNGYbt66nwH6vz\nW0V3v93CtT6pUGXli3l4cc+52H6zjiFd9KyKQEiLR3s+2M+kgN1DrFiilskEb9vq\n9oy/H7dnixTBryTEkFb6ITv0jXq56ty/LZiK9CNw+QOBg9qUqIbxKs1q96NPfjqq\niJwKVR7mQQdhZTaX5+n0vmxZ3b/nWWv7/DdQ+TKpLXVtz4qRyk5ugAqZL/Utl76c\n3MVkzm4qmnEAt3LJCkln6NKu1yDs9otYhZSDRJDkELQWSRDR0t115X0ImUIz9anL\nvga515yUGr57/H3AWmQiJX4QhHsfnL5CeC0hhPQCx8Dn9XMWM6Oft1Tzw4JgX+93\n204Hgu4ty+dZVPmCQPdWlNCCLPnQnvGOFeiJ8zSKSZwp7PyIzq4iMeXlGteqdQfX\nV1k48DQA0ddU0ebvxxhBsYiJwC3f9UHx1TVWYAxxJ04bWgySKK1c/ubVxGniiges\n/rkHKXQN2EA1uS//3VA1iRij0DxSm+SJFL+tSJX0nizlU/LORzF5z+xGucgoO108\n6VYxAkC8wR7bvZ67DwIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW\nBBQdK0umSvcmofds5bhWk1SLH9+Q5zAOBgNVHQ8BAf8EBAMCAYYwDQYJKoZIhvcN\nAQELBQADggIBAMdZN7ZHvoH9H+og70zvkH3t0ReoSXaIMCIVfeIe1N5w34z43mi8\n7NGDZX/CYH6WxqwODfRPDVe/Uwpol1hB/MhFfU5LOAMR/uw9aOMgUBYRjPXcytjt\nG4m/dQ8Rj0yQB0z5S4gnLNuqaAbamOHBeHC1FxqhEfMpECvDig6LxOIldJ7Uzv6a\n4nbzOJb/SuLRqNFvtoJiedn1KE1PnLt+p1pZDIRZvjgm+Y9Zbr62K3uFwAAnTEQL\n1MohIEpk7V9FOtPUDCCXRxByzXchdfi3TvI+i14bmX+Idn2jjOArkrOtzqSvmZ/J\nHnOitmpCgPE/Pg54grYnxYtIET5lu2IN8ZPnU8Tj3AxpCAxzRtYTj7cT0f5XdGQp\nNJ5xLpWR/gGGB/mhabgANRmux+gxubxZJfZ1vnJw1UJYDE7tbI0otiJDIcZ9THhm\n8+zh5KaFgJcz/681wtjiGiLRzC6W2U4qXH68ks3R9q+K5tsgl9r7m65RAFjJ0rXQ\n2wB7taBmKFA0kwZ20FALERPOUrrMAiS8pMDNYJntqt2BCkMgNJFs4k4NUNPlE6If\nJ8losuZ//A46EzOOddeTlEFpl2vJ9zJhE6SjLAbJjTpXStMATP49hAAuUNiVxc4Q\nUaqoRx97ZTDhLgUtG7ulbvTOa/m11mFSTy2viLYRqqGxktntNZnZk3Hn\n-----END CERTIFICATE-----\n"
;

static const char* x509privatekey =
"sr=eyJrZXlfaWQiOnsiS2V5UGFpciI6ImRldmljZS1pZCJ9LCJub25jZSI6IlVNeG1meTZKVEhJejFnenhic1FaMElPTUdVUXVQeW1RWGM2SE9CRjZyYVZlMzF1MGZXaFN6ekkvcUpVQjFWYUZ6UUY5blBxU2lzUTd4SU90alZLbzNBPT0ifQ==&sig=5cV7TSn2VqR5soUxjUZzQ87M1abUS37aiUFE3zl0WnA="
;

#ifdef SAMPLE_OPENSSL_ENGINE
static const char* opensslEngine = SAMPLE_OPENSSL_ENGINE;
static const OPTION_OPENSSL_KEY_TYPE x509_key_from_engine = KEY_TYPE_ENGINE;
#endif

#define MESSAGE_COUNT        5
static bool g_continueRunning = true;
static size_t g_message_count_send_confirmations = 0;

typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

static void send_confirm_callback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    (void)userContextCallback;
    // When a message is sent this callback will get invoked
    g_message_count_send_confirmations++;
    (void)printf("Confirmation callback received for message %zu with result %s\r\n", g_message_count_send_confirmations, MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
}

int main(void)
{
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol;
    IOTHUB_MESSAGE_HANDLE message_handle;
    size_t messages_sent = 0;
    const char* telemetry_msg = "test_message";

    // Select the Protocol to use with the connection
#ifdef SAMPLE_MQTT
    protocol = MQTT_Protocol;
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    protocol = MQTT_WebSocket_Protocol;
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    protocol = AMQP_Protocol;
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    protocol = AMQP_Protocol_over_WebSocketsTls;
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    protocol = HTTP_Protocol;
#endif // SAMPLE_HTTP

    IOTHUB_DEVICE_CLIENT_LL_HANDLE device_ll_handle;

    // Used to initialize IoTHub SDK subsystem
    (void)IoTHub_Init();

    (void)printf("Creating IoTHub handle\r\n");
    // Create the iothub handle here
    device_ll_handle = IoTHubDeviceClient_LL_CreateFromConnectionString(connectionString, protocol);
    if (device_ll_handle == NULL)
    {
        (void)printf("Failure creating IotHub device. Hint: Check your connection string.\r\n");
    }
    else
    {
        // Set any option that are necessary.
        // For available options please see the iothub_sdk_options.md documentation
        //bool traceOn = true;
        //IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_LOG_TRACE, &traceOn);

        // Setting the Trusted Certificate. This is only necessary on systems without
        // built in certificate stores.
#ifdef SET_TRUSTED_CERT_IN_SAMPLES
        IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_TRUSTED_CERT, certificates);
#endif // SET_TRUSTED_CERT_IN_SAMPLES

#if defined SAMPLE_MQTT || defined SAMPLE_MQTT_OVER_WEBSOCKETS
        //Setting the auto URL Encoder (recommended for MQTT). Please use this option unless
        //you are URL Encoding inputs yourself.
        //ONLY valid for use with MQTT
        bool urlEncodeOn = true;
        (void)IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_AUTO_URL_ENCODE_DECODE, &urlEncodeOn);
#endif
        // Set the X509 certificates in the SDK
        if (
#ifdef SAMPLE_OPENSSL_ENGINE
            (IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_OPENSSL_ENGINE, opensslEngine) != IOTHUB_CLIENT_OK) ||
            (IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_OPENSSL_PRIVATE_KEY_TYPE, &x509_key_from_engine) != IOTHUB_CLIENT_OK) ||
#endif
            (IoTHubDeviceClient_LL_SetOption(device_ll_handle, OPTION_X509_CERT, x509certificate) != IOTHUB_CLIENT_OK) ||
            (IoTHubDeviceClient_LL_SetOption(device_ll_handle, SU_OPTION_X509_PRIVATE_KEY, x509privatekey) != IOTHUB_CLIENT_OK)
            )
        {
            printf("failure to set options for x509, aborting\r\n");
        }
        else
        {
            do
            {
                if (messages_sent < MESSAGE_COUNT)
                {
                    // Construct the iothub message from a string or a byte array
                    message_handle = IoTHubMessage_CreateFromString(telemetry_msg);
                    //message_handle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText)));

                    // Set Message property
                    (void)IoTHubMessage_SetMessageId(message_handle, "MSG_ID");
                    (void)IoTHubMessage_SetCorrelationId(message_handle, "CORE_ID");
                    (void)IoTHubMessage_SetContentTypeSystemProperty(message_handle, "application%2Fjson");
                    (void)IoTHubMessage_SetContentEncodingSystemProperty(message_handle, "utf-8");

                    // Add custom properties to message
                    (void)IoTHubMessage_SetProperty(message_handle, "property_key", "property_value");

                    (void)printf("Sending message %d to IoTHub\r\n", (int)(messages_sent + 1));
                    IoTHubDeviceClient_LL_SendEventAsync(device_ll_handle, message_handle, send_confirm_callback, NULL);

                    // The message is copied to the sdk so the we can destroy it
                    IoTHubMessage_Destroy(message_handle);

                    messages_sent++;
                }
                else if (g_message_count_send_confirmations >= MESSAGE_COUNT)
                {
                    // After all messages are all received stop running
                    g_continueRunning = false;
                }

                IoTHubDeviceClient_LL_DoWork(device_ll_handle);
                ThreadAPI_Sleep(1);

            } while (g_continueRunning);
        }
        // Clean up the iothub sdk handle
        IoTHubDeviceClient_LL_Destroy(device_ll_handle);
    }
    // Free all the sdk subsystem
    IoTHub_Deinit();

    printf("Press any key to continue");
    (void)getchar();

    return 0;
}
