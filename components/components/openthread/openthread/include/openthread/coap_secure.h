/*
 *  Copyright (c) 2018, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief
 *  This file defines the top-level functions for the OpenThread CoAP Secure implementation.
 *
 *  @note
 *   The functions in this module require the build-time feature `OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE=1`.
 *
 *  @note
 *   To enable cipher suite DTLS_PSK_WITH_AES_128_CCM_8, MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
 *    must be enabled in mbedtls-config.h
 *   To enable cipher suite DTLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,
 *    MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED must be enabled in mbedtls-config.h.
 */

#ifndef OPENTHREAD_COAP_SECURE_H_
#define OPENTHREAD_COAP_SECURE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <openthread/coap.h>
#include <openthread/error.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/message.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup api-coap-secure
 *
 * @brief
 *   This module includes functions that control CoAP Secure (CoAP over DTLS) communication.
 *
 *   The functions in this module are available when CoAP Secure API feature
 *   (`OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE`) is enabled.
 *
 * @{
 */

#define OT_DEFAULT_COAP_SECURE_PORT 5684 ///< Default CoAP Secure port, as specified in RFC 7252

/**
 * CoAP secure connection event types.
 */
typedef enum otCoapSecureConnectEvent
{
    OT_COAP_SECURE_CONNECTED = 0,             ///< Connection established
    OT_COAP_SECURE_DISCONNECTED_PEER_CLOSED,  ///< Disconnected by peer
    OT_COAP_SECURE_DISCONNECTED_LOCAL_CLOSED, ///< Disconnected locally
    OT_COAP_SECURE_DISCONNECTED_MAX_ATTEMPTS, ///< Disconnected due to reaching the max connection attempts
    OT_COAP_SECURE_DISCONNECTED_ERROR,        ///< Disconnected due to an error
    OT_COAP_SECURE_DISCONNECTED_TIMEOUT,      ///< Disconnected locally due to session timeout
} otCoapSecureConnectEvent;

/**
 * Pointer is called when the DTLS connection state changes.
 *
 * @param[in]  aEvent      The connection event.
 * @param[in]  aContext    A pointer to arbitrary context information.
 */
typedef void (*otHandleCoapSecureClientConnect)(otCoapSecureConnectEvent aEvent, void *aContext);

/**
 * Callback function pointer to notify when the CoAP secure agent is automatically stopped due to reaching the maximum
 * number of connection attempts.
 *
 * @param[in] aContext    A pointer to arbitrary context information.
 */
typedef void (*otCoapSecureAutoStopCallback)(void *aContext);

/**
 * Starts the CoAP Secure service.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aPort      The local UDP port to bind to.
 *
 * @retval OT_ERROR_NONE  Successfully started the CoAP Secure server.
 */
otError otCoapSecureStart(otInstance *aInstance, uint16_t aPort);

/**
 * Starts the CoAP secure service and sets the maximum number of allowed connection attempts before stopping the
 * agent automatically.
 *
 * @param[in] aInstance       A pointer to an OpenThread instance.
 * @param[in] aPort           The local UDP port to bind to.
 * @param[in] aMaxAttempts    Maximum number of allowed connection request attempts. Zero indicates no limit.
 * @param[in] aCallback       Callback to notify if max number of attempts has reached and agent is stopped.
 * @param[in] aContext        A pointer to arbitrary context to use with @p aCallback.
 *
 * @retval OT_ERROR_NONE        Successfully started the CoAP agent.
 * @retval OT_ERROR_ALREADY     Already started.
 */
otError otCoapSecureStartWithMaxConnAttempts(otInstance                  *aInstance,
                                             uint16_t                     aPort,
                                             uint16_t                     aMaxAttempts,
                                             otCoapSecureAutoStopCallback aCallback,
                                             void                        *aContext);

/**
 * Stops the CoAP Secure server.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 */
void otCoapSecureStop(otInstance *aInstance);

/**
 * Sets the Pre-Shared Key (PSK) and cipher suite
 * DTLS_PSK_WITH_AES_128_CCM_8.
 *
 * @note This function requires the build-time feature `MBEDTLS_KEY_EXCHANGE_PSK_ENABLED` to be enabled.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aPsk          A pointer to the PSK.
 * @param[in]  aPskLength    The PSK length.
 * @param[in]  aPskIdentity  The Identity Name for the PSK.
 * @param[in]  aPskIdLength  The PSK Identity Length.
 */
void otCoapSecureSetPsk(otInstance    *aInstance,
                        const uint8_t *aPsk,
                        uint16_t       aPskLength,
                        const uint8_t *aPskIdentity,
                        uint16_t       aPskIdLength);

/**
 * Returns the peer x509 certificate base64 encoded.
 *
 * @note This function requires the build-time features `MBEDTLS_BASE64_C` and
 *       `MBEDTLS_SSL_KEEP_PEER_CERTIFICATE` to be enabled.
 *
 * @param[in]   aInstance        A pointer to an OpenThread instance.
 * @param[out]  aPeerCert        A pointer to the base64 encoded certificate buffer.
 * @param[out]  aCertLength      The length of the base64 encoded peer certificate.
 * @param[in]   aCertBufferSize  The buffer size of aPeerCert.
 *
 * @retval OT_ERROR_INVALID_STATE   Not connected yet.
 * @retval OT_ERROR_NONE            Successfully get the peer certificate.
 * @retval OT_ERROR_NO_BUFS         Can't allocate memory for certificate.
 */
otError otCoapSecureGetPeerCertificateBase64(otInstance    *aInstance,
                                             unsigned char *aPeerCert,
                                             size_t        *aCertLength,
                                             size_t         aCertBufferSize);

/**
 * Sets the authentication mode for the coap secure connection.
 *
 * Disable or enable the verification of peer certificate.
 * Must be called before start.
 *
 * @param[in]   aInstance               A pointer to an OpenThread instance.
 * @param[in]   aVerifyPeerCertificate  true, to verify the peer certificate.
 */
void otCoapSecureSetSslAuthMode(otInstance *aInstance, bool aVerifyPeerCertificate);

/**
 * Sets the local device's X509 certificate with corresponding private key for
 * DTLS session with DTLS_ECDHE_ECDSA_WITH_AES_128_CCM_8.
 *
 * @note This function requires `MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED=1`.
 *
 * @param[in]  aInstance          A pointer to an OpenThread instance.
 * @param[in]  aX509Cert          A pointer to the PEM formatted X509 certificate.
 * @param[in]  aX509Length        The length of certificate.
 * @param[in]  aPrivateKey        A pointer to the PEM formatted private key.
 * @param[in]  aPrivateKeyLength  The length of the private key.
 */
void otCoapSecureSetCertificate(otInstance    *aInstance,
                                const uint8_t *aX509Cert,
                                uint32_t       aX509Length,
                                const uint8_t *aPrivateKey,
                                uint32_t       aPrivateKeyLength);

/**
 * Sets the trusted top level CAs. It is needed for validating the
 * certificate of the peer.
 *
 * DTLS mode "ECDHE ECDSA with AES 128 CCM 8" for Application CoAPS.
 *
 * @note This function requires `MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED=1`.
 *
 * @param[in]  aInstance                A pointer to an OpenThread instance.
 * @param[in]  aX509CaCertificateChain  A pointer to the PEM formatted X509 CA chain.
 * @param[in]  aX509CaCertChainLength   The length of chain.
 */
void otCoapSecureSetCaCertificateChain(otInstance    *aInstance,
                                       const uint8_t *aX509CaCertificateChain,
                                       uint32_t       aX509CaCertChainLength);

/**
 * Initializes DTLS session with a peer.
 *
 * @param[in]  aInstance               A pointer to an OpenThread instance.
 * @param[in]  aSockAddr               A pointer to the remote socket address.
 * @param[in]  aHandler                A pointer to a function that will be called when the DTLS connection
 *                                     state changes.
 * @param[in]  aContext                A pointer to arbitrary context information.
 *
 * @retval OT_ERROR_NONE  Successfully started DTLS connection.
 */
otError otCoapSecureConnect(otInstance                     *aInstance,
                            const otSockAddr               *aSockAddr,
                            otHandleCoapSecureClientConnect aHandler,
                            void                           *aContext);

/**
 * Stops the DTLS connection.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 */
void otCoapSecureDisconnect(otInstance *aInstance);

/**
 * Indicates whether or not the DTLS session is connected.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 *
 * @retval TRUE   The DTLS session is connected.
 * @retval FALSE  The DTLS session is not connected.
 */
bool otCoapSecureIsConnected(otInstance *aInstance);

/**
 * Indicates whether or not the DTLS session is active.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 *
 * @retval TRUE  If DTLS session is active.
 * @retval FALSE If DTLS session is not active.
 */
bool otCoapSecureIsConnectionActive(otInstance *aInstance);

/**
 * Indicates whether or not the DTLS session is closed.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 *
 * @retval TRUE   The DTLS session is closed.
 * @retval FALSE  The DTLS session is not closed.
 */
bool otCoapSecureIsClosed(otInstance *aInstance);

/**
 * Sends a CoAP request block-wise over secure DTLS connection.
 *
 * Is available when OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE configuration
 * is enabled.
 *
 * If a response for a request is expected, respective function and context information should be provided.
 * If no response is expected, these arguments should be NULL pointers.
 * If Message Id was not set in the header (equal to 0), this function will assign unique Message Id to the message.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aMessage      A reference to the message to send.
 * @param[in]  aHandler      A function pointer that shall be called on response reception or time-out.
 * @param[in]  aContext      A pointer to arbitrary context information.
 * @param[in]  aTransmitHook A function pointer that is called on Block1 response reception.
 * @param[in]  aReceiveHook  A function pointer that is called on Block2 response reception.
 *
 * @retval OT_ERROR_NONE           Successfully sent CoAP message.
 * @retval OT_ERROR_NO_BUFS        Failed to allocate retransmission data.
 * @retval OT_ERROR_INVALID_STATE  DTLS connection was not initialized.
 */
otError otCoapSecureSendRequestBlockWise(otInstance                 *aInstance,
                                         otMessage                  *aMessage,
                                         otCoapResponseHandler       aHandler,
                                         void                       *aContext,
                                         otCoapBlockwiseTransmitHook aTransmitHook,
                                         otCoapBlockwiseReceiveHook  aReceiveHook);

/**
 * Sends a CoAP request over secure DTLS connection.
 *
 * If a response for a request is expected, respective function and context information should be provided.
 * If no response is expected, these arguments should be NULL pointers.
 * If Message Id was not set in the header (equal to 0), this function will assign unique Message Id to the message.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aMessage      A reference to the message to send.
 * @param[in]  aHandler      A function pointer that shall be called on response reception or time-out.
 * @param[in]  aContext      A pointer to arbitrary context information.
 *
 * @retval OT_ERROR_NONE           Successfully sent CoAP message.
 * @retval OT_ERROR_NO_BUFS        Failed to allocate retransmission data.
 * @retval OT_ERROR_INVALID_STATE  DTLS connection was not initialized.
 */
otError otCoapSecureSendRequest(otInstance           *aInstance,
                                otMessage            *aMessage,
                                otCoapResponseHandler aHandler,
                                void                 *aContext);

/**
 * Adds a resource to the CoAP Secure server.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aResource  A pointer to the resource.
 */
void otCoapSecureAddResource(otInstance *aInstance, otCoapResource *aResource);

/**
 * Removes a resource from the CoAP Secure server.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aResource  A pointer to the resource.
 */
void otCoapSecureRemoveResource(otInstance *aInstance, otCoapResource *aResource);

/**
 * Adds a block-wise resource to the CoAP Secure server.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aResource  A pointer to the resource.
 */
void otCoapSecureAddBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource);

/**
 * Removes a block-wise resource from the CoAP Secure server.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aResource  A pointer to the resource.
 */
void otCoapSecureRemoveBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource);

/**
 * Sets the default handler for unhandled CoAP Secure requests.
 *
 * @param[in]  aInstance  A pointer to an OpenThread instance.
 * @param[in]  aHandler   A function pointer that shall be called when an unhandled request arrives.
 * @param[in]  aContext   A pointer to arbitrary context information. May be NULL if not used.
 */
void otCoapSecureSetDefaultHandler(otInstance *aInstance, otCoapRequestHandler aHandler, void *aContext);

/**
 * Sets the connect event callback to indicate when
 * a Client connection to the CoAP Secure server has changed.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aHandler      A pointer to a function that will be called once DTLS connection has changed.
 * @param[in]  aContext      A pointer to arbitrary context information. May be NULL if not used.
 */
void otCoapSecureSetClientConnectEventCallback(otInstance                     *aInstance,
                                               otHandleCoapSecureClientConnect aHandler,
                                               void                           *aContext);

/**
 * Sends a CoAP response block-wise from the CoAP Secure server.
 *
 * Is available when OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE configuration
 * is enabled.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aMessage      A pointer to the CoAP response to send.
 * @param[in]  aMessageInfo  A pointer to the message info associated with @p aMessage.
 * @param[in]  aContext      A pointer to arbitrary context information. May be NULL if not used.
 * @param[in]  aTransmitHook A function pointer that is called on Block1 request reception.
 *
 * @retval OT_ERROR_NONE     Successfully enqueued the CoAP response message.
 * @retval OT_ERROR_NO_BUFS  Insufficient buffers available to send the CoAP response.
 */
otError otCoapSecureSendResponseBlockWise(otInstance                 *aInstance,
                                          otMessage                  *aMessage,
                                          const otMessageInfo        *aMessageInfo,
                                          void                       *aContext,
                                          otCoapBlockwiseTransmitHook aTransmitHook);

/**
 * Sends a CoAP response from the CoAP Secure server.
 *
 * @param[in]  aInstance     A pointer to an OpenThread instance.
 * @param[in]  aMessage      A pointer to the CoAP response to send.
 * @param[in]  aMessageInfo  A pointer to the message info associated with @p aMessage.
 *
 * @retval OT_ERROR_NONE     Successfully enqueued the CoAP response message.
 * @retval OT_ERROR_NO_BUFS  Insufficient buffers available to send the CoAP response.
 */
otError otCoapSecureSendResponse(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo);

/**
 * @}
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OPENTHREAD_COAP_SECURE_H_ */
