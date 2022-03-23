// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hsm_client_data.h"

// This sample is provided for sample only.  Please do not use this in production
// For more information please see the devdoc using_custom_hsm.md
static const char* const COMMON_NAME = "692d3e29-0c64-48a0-b989-5407121f8cda";
static const char* const CERTIFICATE = "-----BEGIN CERTIFICATE-----\n"
"MIIEZjCCA06gAwIBAgIURT0gkso+qvDa6o7iUTWqyDDiP64wDQYJKoZIhvcNAQEL\n"
"BQAwSDELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSMwIQYDVQQD\n"
"ExpCLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMjAeFw0yMjAzMTUwOTE5NDRaFw0y\n"
"NDAzMTUwOTE5NDRaMC8xLTArBgNVBAMTJDY5MmQzZTI5LTBjNjQtNDhhMC1iOTg5\n"
"LTU0MDcxMjFmOGNkYTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMTo\n"
"CSFS/lSl9QZCHnJENIsJcMDCov5Br4/lm0likZy6gmlHaXrvB4DcHuP7QnJkWPCH\n"
"PzsvMB6sRr+0zx48uVmMGgl5pePmg6mJAfQbY5luAQfTKddMuTLlqD/3jIrbocVB\n"
"HwqD+0ZfoYyL3X/gL4Vpr3ILIcs+4vZ8RggGTRDE7Uu4tR+vtAoZVLPeba+usofW\n"
"bHHMUF3SpCYm0NBEdRMqGfwDa7kpZxdpWhWOhGnoU+sJXsjwVMrL4UrTycYHRgiS\n"
"DEgjHCvxXA5uVdFxz4igIPM5z3DUABAWcsKcOItaeso+A39LEY/b9jD0T9fzsUM9\n"
"vKgUSEiiaEC3In4ZEQUCAwEAAaOCAV8wggFbMAwGA1UdEwEB/wQCMAAwHQYDVR0O\n"
"BBYEFBjPBWealeHobDsUxVcG0t5Sul8TMB8GA1UdIwQYMBaAFDb0QaEeb2kGoOTb\n"
"GYEqtDf6fxixMA4GA1UdDwEB/wQEAwIFoDAWBgNVHSUBAf8EDDAKBggrBgEFBQcD\n"
"AjCBkAYIKwYBBQUHAQEEgYMwgYAwLQYIKwYBBQUHMAGGIWh0dHA6Ly9vY3NwLmRl\n"
"bW8ub25lLmRpZ2ljZXJ0LmNvbTBPBggrBgEFBQcwAoZDaHR0cDovL2NhY2VydHMu\n"
"ZGVtby5vbmUuZGlnaWNlcnQuY29tL0IuQnJhdW4tUE9DLVJTQS1Jb1QtSUNBLUcy\n"
"LmNydDBQBgNVHR8ESTBHMEWgQ6BBhj9odHRwOi8vY3JsLmRlbW8ub25lLmRpZ2lj\n"
"ZXJ0LmNvbS9CLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMi5jcmwwDQYJKoZIhvcN\n"
"AQELBQADggEBAC+FigXe/FuAJdV+FUhhIYXO6Y1+tzqJFmQHh89n/qaJg7hSbOLb\n"
"hss4DkAYmEs8mnyfn0ws48tAFgTSNG0uGSZ7yuNl3BdlASak7oJZXezsgDIvr3Hy\n"
"MQJc+70TMVYG2mfdcq6Vhv4HWNvJXPvOOh7hMTIoxLgjNPT+mT043emufJdEnOpN\n"
"GeC11DgRzGk4mlsA6JKMY6z+KB8J6jECC6wSGjlog3X3mePoUeOHbUo+UhuYdFIe\n"
"AQm2reFdV5cH7fVn/tIJZTABAttFxSU+MqAOyZkgsRPcQROKRfA7Gi9w5si3qLIg\n"
"g8RrDmdnYpvc79LURNmdOoKuZ94hINf8SUk=\n"
"-----END CERTIFICATE-----\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIFYDCCA0igAwIBAgIUMnX35u2iceM49L9cU0LHS290RkcwDQYJKoZIhvcNAQEL\n"
"BQAwSjELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSUwIwYDVQQD\n"
"ExxCIEJyYXVuIFBPQyAtIFJTQSBSb290IENBIEcyMB4XDTIyMDMwNDEzMjYwOFoX\n"
"DTQxMDMzMDIzMDAwMFowSDELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVu\n"
"IFNFMSMwIQYDVQQDExpCLkJyYXVuLVBPQy1SU0EtSW9ULUlDQS1HMjCCASIwDQYJ\n"
"KoZIhvcNAQEBBQADggEPADCCAQoCggEBALdEOcXP0KTOcgFN742D9y3k1JQOYJDX\n"
"EXdAXc4bKBPwDVHlSrbURB1dw/RXBrLzKb8o6/ZReUGGQEQpLcBJoa+4eP6hwmgH\n"
"fapbY/aGvMC2zRLOliniwsxisTp6KBkhr9kalaFKRg2NManupfHqsIXsj89pEkiL\n"
"qg8t07aiN8CQXW8j4oWxyPkWHzqJBpv5PzEy/ovZiKlrWme7J/04UxaRahucKFEe\n"
"QM0F5WB7B9xFXskqBwEH6R0o39JYKC9iACkLdhTZW7KMKn9ZNU1nV+ick6syxPmR\n"
"dPDI4z0B8e3HcVEm8Qq3PbCuosZnURjdJv+erWdHW2HqVFZ9ZQVN0X0CAwEAAaOC\n"
"AT4wggE6MA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFDb0QaEeb2kGoOTbGYEq\n"
"tDf6fxixMB8GA1UdIwQYMBaAFB0rS6ZK9yah92zluFaTVIsf35DnMA4GA1UdDwEB\n"
"/wQEAwIBhjCBiQYIKwYBBQUHAQEEfTB7MC0GCCsGAQUFBzABhiFodHRwOi8vb2Nz\n"
"cC5kZW1vLm9uZS5kaWdpY2VydC5jb20wSgYIKwYBBQUHMAKGPmh0dHA6Ly9jYWNl\n"
"cnRzLmRlbW8ub25lLmRpZ2ljZXJ0LmNvbS9CQnJhdW5QT0MtUlNBUm9vdENBRzIu\n"
"Y3J0MEsGA1UdHwREMEIwQKA+oDyGOmh0dHA6Ly9jcmwuZGVtby5vbmUuZGlnaWNl\n"
"cnQuY29tL0JCcmF1blBPQy1SU0FSb290Q0FHMi5jcmwwDQYJKoZIhvcNAQELBQAD\n"
"ggIBAFZgWRr0mWfz9eClCjnyE1w8zLUhkbsOPhF7+IoEWu5iYh5Z11l44GUx1k3W\n"
"/eoq/kjM8hgJaoGBsVuYcGs/rMBLh9m9VRDx2TUAKfUfSo2vUsNHUeWz+lONYaNw\n"
"ck/YNyobuFcR5SXctHMVyV+dlAiD5CntL/uUf69Yrm5bh3YRqj4Se4qWF+V46vjP\n"
"8qymtma5gnPR2KweiqmS0oLGNbjomEIWX0ObFgjjdOLPsrx/OH0EMxp1vq0LKvlM\n"
"2aJK+k8VN4JhVJczBYElIanO7GAPTLPHnev+GU6pYl7r++eXUcX8qDUeiVlsOlYg\n"
"XKh7fD//sH2n/vDQiPaVbakZscmoXVqxgKeF/eyhoLH4rT7+6Xd2LELV0+YjFszj\n"
"udh+FEGOUEg7Z6hOa2xFj7AC2zuHjwemu7mTcV7Y4lxP3e1M8KIpWszkEtMjXdJ0\n"
"qlXO5/gxkjXcCzh+kpCilknigxV03ASe9J3w8m8GsAO30ggF9fe0q7FTVZTvub3Z\n"
"OdbufMOd7XsJabfT/pDJSj5Pfgo4NsBCVaqaqnGs+HdMwwX5h0GBWTdAGO9eZzTb\n"
"Vzx2pl6IHw7hIj+LK7BSyqP1VTV2SiDYe1Xm9LHZRyd3fu9lpUosJ3cOTC9S8y9O\n"
"NgfSkdaXEupq5YuZmPvt8az0KmWjcex/3pCJOCvK+2mg3joI\n"
"-----END CERTIFICATE-----\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIFZjCCA06gAwIBAgIUcGx1PLOwiEVnvF+cs9RIe98YiWgwDQYJKoZIhvcNAQEL\n"
"BQAwSjELMAkGA1UEBhMCREUxFDASBgNVBAoTC0IuIEJyYXVuIFNFMSUwIwYDVQQD\n"
"ExxCIEJyYXVuIFBPQyAtIFJTQSBSb290IENBIEcyMCAXDTIyMDMwNDEzMjE1NVoY\n"
"DzIwNTIwMzA0MTMxOTAwWjBKMQswCQYDVQQGEwJERTEUMBIGA1UEChMLQi4gQnJh\n"
"dW4gU0UxJTAjBgNVBAMTHEIgQnJhdW4gUE9DIC0gUlNBIFJvb3QgQ0EgRzIwggIi\n"
"MA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDVkyTSWxg9ii9Z/41rOelcWDaS\n"
"zvG7zHaHqiG1FJM0BUhjhz8nRVEfCM5XqbxoKsky+az6TcKsUwFw4Zh4bbeT5cLR\n"
"qw7R/9SOWg7qDByMVr4NfbeZvIhQ/PDRsOvQ2rJYqsq1qsIIHapNGYbt66nwH6vz\n"
"W0V3v93CtT6pUGXli3l4cc+52H6zjiFd9KyKQEiLR3s+2M+kgN1DrFiilskEb9vq\n"
"9oy/H7dnixTBryTEkFb6ITv0jXq56ty/LZiK9CNw+QOBg9qUqIbxKs1q96NPfjqq\n"
"iJwKVR7mQQdhZTaX5+n0vmxZ3b/nWWv7/DdQ+TKpLXVtz4qRyk5ugAqZL/Utl76c\n"
"3MVkzm4qmnEAt3LJCkln6NKu1yDs9otYhZSDRJDkELQWSRDR0t115X0ImUIz9anL\n"
"vga515yUGr57/H3AWmQiJX4QhHsfnL5CeC0hhPQCx8Dn9XMWM6Oft1Tzw4JgX+93\n"
"204Hgu4ty+dZVPmCQPdWlNCCLPnQnvGOFeiJ8zSKSZwp7PyIzq4iMeXlGteqdQfX\n"
"V1k48DQA0ddU0ebvxxhBsYiJwC3f9UHx1TVWYAxxJ04bWgySKK1c/ubVxGniiges\n"
"/rkHKXQN2EA1uS//3VA1iRij0DxSm+SJFL+tSJX0nizlU/LORzF5z+xGucgoO108\n"
"6VYxAkC8wR7bvZ67DwIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW\n"
"BBQdK0umSvcmofds5bhWk1SLH9+Q5zAOBgNVHQ8BAf8EBAMCAYYwDQYJKoZIhvcN\n"
"AQELBQADggIBAMdZN7ZHvoH9H+og70zvkH3t0ReoSXaIMCIVfeIe1N5w34z43mi8\n"
"7NGDZX/CYH6WxqwODfRPDVe/Uwpol1hB/MhFfU5LOAMR/uw9aOMgUBYRjPXcytjt\n"
"G4m/dQ8Rj0yQB0z5S4gnLNuqaAbamOHBeHC1FxqhEfMpECvDig6LxOIldJ7Uzv6a\n"
"4nbzOJb/SuLRqNFvtoJiedn1KE1PnLt+p1pZDIRZvjgm+Y9Zbr62K3uFwAAnTEQL\n"
"1MohIEpk7V9FOtPUDCCXRxByzXchdfi3TvI+i14bmX+Idn2jjOArkrOtzqSvmZ/J\n"
"HnOitmpCgPE/Pg54grYnxYtIET5lu2IN8ZPnU8Tj3AxpCAxzRtYTj7cT0f5XdGQp\n"
"NJ5xLpWR/gGGB/mhabgANRmux+gxubxZJfZ1vnJw1UJYDE7tbI0otiJDIcZ9THhm\n"
"8+zh5KaFgJcz/681wtjiGiLRzC6W2U4qXH68ks3R9q+K5tsgl9r7m65RAFjJ0rXQ\n"
"2wB7taBmKFA0kwZ20FALERPOUrrMAiS8pMDNYJntqt2BCkMgNJFs4k4NUNPlE6If\n"
"J8losuZ//A46EzOOddeTlEFpl2vJ9zJhE6SjLAbJjTpXStMATP49hAAuUNiVxc4Q\n"
"UaqoRx97ZTDhLgUtG7ulbvTOa/m11mFSTy2viLYRqqGxktntNZnZk3Hn\n"
"-----END CERTIFICATE-----\n"
;
static const char* const PRIVATE_KEY = "-----BEGIN PRIVATE KEY-----\n"
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDE6AkhUv5UpfUG\n"
"Qh5yRDSLCXDAwqL+Qa+P5ZtJYpGcuoJpR2l67weA3B7j+0JyZFjwhz87LzAerEa/\n"
"tM8ePLlZjBoJeaXj5oOpiQH0G2OZbgEH0ynXTLky5ag/94yK26HFQR8Kg/tGX6GM\n"
"i91/4C+Faa9yCyHLPuL2fEYIBk0QxO1LuLUfr7QKGVSz3m2vrrKH1mxxzFBd0qQm\n"
"JtDQRHUTKhn8A2u5KWcXaVoVjoRp6FPrCV7I8FTKy+FK08nGB0YIkgxIIxwr8VwO\n"
"blXRcc+IoCDzOc9w1AAQFnLCnDiLWnrKPgN/SxGP2/Yw9E/X87FDPbyoFEhIomhA\n"
"tyJ+GREFAgMBAAECggEACcqsIY8Z/KWgEKabEv2XS06137AOIOytSU+WsEiJiyyv\n"
"LYMSmL1oNx07IAUvobahwZxIJa5MC0pv3jqkjhFg+jYAWXZGLJYa+ARLJ5NxGYEh\n"
"5fy3OPCkyQYonr21rHYNjlyVqZU0mqRIyU9vPVutoUnHIrmw/YVsDevSI7YEgQ/j\n"
"knew211EKS04nc4sWUlbjK0Wj2Nb4WqprT7ShBfcaWWUECB+1VeXwavMjZVwz13X\n"
"fgI0OezENxFSgYGPvGFhVIAfrXig4KLhVwNI+LkgZqHbBOK2ZaLR9pZZnU4l4Owz\n"
"VvDo1KIiFYt+G5LgeH2MTyuX/OOK1v+wEpja4PHU5QKBgQDmssAMZgKoFZmZA8eT\n"
"Hc0hJ30X0BSE7si9Oqz64svKPoKsYneBUzKZQEVQMNSCUnvVY2Q1GRQfjnXlhewn\n"
"YqI8Cm6WSzDWSJHxn7DH0GKUesgHXWAU1kSsU4mbkDLu6NLJ9EV1gPbw5I9WSMLs\n"
"GKVMmPmbVg4PlLRx4EnfGviCRwKBgQDagIVl87dLtErOOVXrL8sJUnlIVxmmI6o3\n"
"/g2wTtiKDetCu9RsBXgXqIQW+0V8LQ2JTZ6LybRK7dIM1A9cy8fhO9gxwipo+uDz\n"
"CtomFGAhEol/QGhHLxWGXtA43R9vQW8VXvINhM/eZZQiTDDB+wEJGI9y5dEkUMF/\n"
"HODGgXiMUwKBgDVdc/9qOYMYPAehUDE/24unztALWjPF6LmdMRZ4EBwgianGvU41\n"
"KKc6rs31/bKH0g1yiGOgAX+rPKpzaEVUE6M/5oTtr8Zh0yDJh+Mw2RzvqgXKG8e2\n"
"KPyT9mj92kpZoSkGUGr2AimHNYEII87phJNLb7H3HhmMlJEjCPOLEaFRAoGAW3qf\n"
"XvXQccOD5h8fF54k1TxQRrcgp+waoeKdfIkmI3Z8Xq4g2zDW/KCy3KRKjRiXqDp0\n"
"/HVwJEa5QGGwJNu0TVouw3ZVJyqR729Fsdep/26xXjNq9RKDYoRjjMsXFA9DIKFl\n"
"mb9W//drM4jOiQg7gMFlnWcWTqv4tu0yNimSfL0CgYAp6WDt+kNgUzRw1vEZXIRu\n"
"5kEAgpp5w4sOxTFYw85Q4MR7xJg0U25/G/LhLRvASx17JiQHvMrjg+wIGdBDjjCJ\n"
"vSzsvCOknSoVYWkOaKEne2G0p0IMQ6Mk/Uzn0Izf6sK9oRqBQit/jfO0ChWhrqpb\n"
"TmobMtivKpa7nax4YfX6og==\n"
"-----END PRIVATE KEY-----\n";

// Provided for sample only
static const char* const SYMMETRIC_KEY = "Symmetric Key value";
static const char* const REGISTRATION_NAME = "Registration Name";

// Provided for sample only, canned values
static const unsigned char EK[] = { 0x45, 0x6e, 0x64, 0x6f, 0x72, 0x73, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x20, 0x6b, 0x65, 0x79, 0x0d, 0x0a };
static const size_t EK_LEN = sizeof(EK)/sizeof(EK[0]);
static const unsigned char SRK[] = { 0x53, 0x74, 0x6f, 0x72, 0x65, 0x20, 0x72, 0x6f, 0x6f, 0x74, 0x20, 0x6b, 0x65, 0x79, 0x0d, 0x0a };
static const size_t SRK_LEN = sizeof(SRK) / sizeof(SRK[0]);
static const unsigned char ENCRYPTED_DATA[] = { 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x65, 0x64, 0x20, 0x64, 0x61, 0x74, 0x61, 0x0d, 0x0a };

typedef struct CUSTOM_HSM_SAMPLE_INFO_TAG
{
    const char* certificate;
    const char* common_name;
    const char* key;
    const unsigned char* endorsement_key;
    size_t ek_length;
    const unsigned char* storage_root_key;
    size_t srk_len;
    const char* symm_key;
    const char* registration_name;
} CUSTOM_HSM_SAMPLE_INFO;

int hsm_client_x509_init(void)
{
    return 0;
}

void hsm_client_x509_deinit(void)
{
}

int hsm_client_tpm_init(void)
{
    return 0;
}

void hsm_client_tpm_deinit(void)
{
}

HSM_CLIENT_HANDLE custom_hsm_create(void)
{
    HSM_CLIENT_HANDLE result;
    CUSTOM_HSM_SAMPLE_INFO* hsm_info = malloc(sizeof(CUSTOM_HSM_SAMPLE_INFO));
    if (hsm_info == NULL)
    {
        (void)printf("Failed allocating hsm info\r\n");
        result = NULL;
    }
    else
    {
        // TODO: initialize any variables here
        hsm_info->certificate = CERTIFICATE;
        hsm_info->key = PRIVATE_KEY;
        hsm_info->common_name = COMMON_NAME;
        hsm_info->endorsement_key = EK;
        hsm_info->ek_length = EK_LEN;
        hsm_info->storage_root_key = SRK;
        hsm_info->srk_len = SRK_LEN;
        hsm_info->symm_key = SYMMETRIC_KEY;
        hsm_info->registration_name = REGISTRATION_NAME;
        result = hsm_info;
    }
    return result;
}

void custom_hsm_destroy(HSM_CLIENT_HANDLE handle)
{
    if (handle != NULL)
    {
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        // Free anything that has been allocated in this module
        free(hsm_info);
    }
}

char* custom_hsm_get_certificate(HSM_CLIENT_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the certificate for the iothub sdk to free
        // this value will be sent unmodified to the tlsio
        // layer to be processed
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->certificate);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->certificate);
        }
    }
    return result;
}

char* custom_hsm_get_key(HSM_CLIENT_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the private key for the iothub sdk to free
        // this value will be sent unmodified to the tlsio
        // layer to be processed
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->key);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->key);
        }
    }
    return result;
}

char* custom_hsm_get_common_name(HSM_CLIENT_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the common name for the iothub sdk to free
        // this value will be sent to dps
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->common_name);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->common_name);
        }
    }
    return result;
}

int custom_hsm_get_endorsement_key(HSM_CLIENT_HANDLE handle, unsigned char** key, size_t* key_len)
{
    int result;
    if (handle == NULL || key == NULL || key_len == NULL)
    {
        (void)printf("Invalid parameter specified");
        result = __LINE__;
    }
    else
    {
        // TODO: Retrieve the endorsement key and malloc the value and return
        // it to the sdk
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        if ((*key = (unsigned char*)malloc(hsm_info->ek_length)) == NULL)
        {
            (void)printf("Failure allocating endorsement key\r\n");
            result = __LINE__;
        }
        else
        {
            memcpy(*key, hsm_info->endorsement_key, hsm_info->ek_length);
            *key_len = hsm_info->ek_length;
            result = 0;
        }
    }
    return result;
}

int custom_hsm_get_storage_root_key(HSM_CLIENT_HANDLE handle, unsigned char** key, size_t* key_len)
{
    int result;
    if (handle == NULL || key == NULL || key_len == NULL)
    {
        (void)printf("Invalid handle value specified");
        result = __LINE__;
    }
    else
    {
        // TODO: Retrieve the storage root key and malloc the value and return
        // it to the sdk
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        if ((*key = (unsigned char*)malloc(hsm_info->srk_len)) == NULL)
        {
            (void)printf("Failure allocating storage root key\r\n");
            result = __LINE__;
        }
        else
        {
            memcpy(*key, hsm_info->storage_root_key, hsm_info->srk_len);
            *key_len = hsm_info->srk_len;
            result = 0;
        }
    }
    return result;
}

int custom_hsm_sign_with_identity(HSM_CLIENT_HANDLE handle, const unsigned char* data, size_t data_len, unsigned char** key, size_t* key_len)
{
    int result;
    if (handle == NULL || data == NULL || key == NULL || key_len == NULL)
    {
        (void)printf("Invalid handle value specified");
        result = __LINE__;
    }
    else
    {
        (void)data;
        (void)data_len;

        // TODO: Need to implement signing the data variable and malloc the key and return it to the sdk

        size_t signed_data_len = 10;
        if ((*key = (unsigned char*)malloc(signed_data_len)) == NULL)
        {
            (void)printf("Failure allocating storage root key\r\n");
            result = __LINE__;
        }
        else
        {
            memcpy(*key, ENCRYPTED_DATA, signed_data_len);
            *key_len = signed_data_len;
            result = 0;
        }
    }
    return result;
}

int custom_hsm_activate_identity_key(HSM_CLIENT_HANDLE handle, const unsigned char* key, size_t key_len)
{
    int result;
    if (handle == NULL || key == NULL || key_len == 0)
    {
        (void)printf("Invalid handle value specified");
        result = __LINE__;
    }
    else
    {
        // Decrypt the key and store the value in the hsm
        result = 0;
    }
    return result;
}

char* custom_hsm_symm_key(HSM_CLIENT_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the symmetric key for the iothub 
        // The SDK will call free() this value
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->symm_key);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->symm_key);
        }
    }
    return result;
}

char* custom_hsm_get_registration_name(HSM_CLIENT_HANDLE handle)
{
    char* result;
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the registration name for the iothub 
        // The SDK will call free() this value
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->registration_name);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->registration_name);
        }
    }
    return result;
}

// Defining the v-table for the x509 hsm calls
static const HSM_CLIENT_X509_INTERFACE x509_interface =
{
    custom_hsm_create,
    custom_hsm_destroy,
    custom_hsm_get_certificate,
    custom_hsm_get_key,
    custom_hsm_get_common_name
};

// Defining the v-table for the x509 hsm calls
static const HSM_CLIENT_TPM_INTERFACE tpm_interface =
{
    custom_hsm_create,
    custom_hsm_destroy,
    custom_hsm_activate_identity_key,
    custom_hsm_get_endorsement_key,
    custom_hsm_get_storage_root_key,
    custom_hsm_sign_with_identity
};

static const HSM_CLIENT_KEY_INTERFACE symm_key_interface =
{
    custom_hsm_create,
    custom_hsm_destroy,
    custom_hsm_symm_key,
    custom_hsm_get_registration_name
};

const HSM_CLIENT_TPM_INTERFACE* hsm_client_tpm_interface(void)
{
    // tpm interface pointer
    return &tpm_interface;
}

const HSM_CLIENT_X509_INTERFACE* hsm_client_x509_interface(void)
{
    // x509 interface pointer
    return &x509_interface;
}

const HSM_CLIENT_KEY_INTERFACE* hsm_client_key_interface(void)
{
    return &symm_key_interface;
}
