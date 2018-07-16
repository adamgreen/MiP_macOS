/* Copyright (C) 2015  Adam Green (https://github.com/adamgreen)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/* This header file describes the API that the MiP C API uses to communicate with the transport specific layer. */
#ifndef MIP_TRANSPORT_H_
#define MIP_TRANSPORT_H_

#include <stdint.h>
#include <stdlib.h>
#include <mip-error.h>

// Maximum length of MiP request and response buffer lengths.
#define MIP_REQUEST_MAX_LEN     (17 + 1)    // Longest request is MPI_CMD_PLAY_SOUND.
#define MIP_RESPONSE_MAX_LEN    (5 + 1)     // Longest response is MPI_CMD_REQUEST_CHEST_LED.

// An abstract object type used by the MiP API to provide transport specific information to each transport function.
// It will be initially created by a call to mipTransportInit() and then passed in as the first parameter to each of the
// other mipTransport*() functions.  It can be freed at the end with a call to mipTransportUninit;
typedef struct MiPTransport MiPTransport;

#ifdef __cplusplus
extern "C"
{
#endif

// Initialize a MiPTransport object.
// Will be the first mipTransport*() function called so it can be used for any setup that the transport needs to take
// care of.  Transport specific data can be stored in the returned in the object pointed to by the returned pointer.
//
//   pInitOptions: A character string which originates with the user.  It could be used for things like serial port,
//                 etc.
//   Returns: NULL on error.
//            A valid pointer to a transport specific object otherwise.
MiPTransport* mipTransportInit(const char* pInitOptions);

// Cleanup a MiPTransport object.
// Will be the last mipTransport*() function called so it can be used to cleanly shutdown any transport resources.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
void          mipTransportUninit(MiPTransport* pTransport);

// Connect to a MiP robot.
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   pRobotName: The name of the robot to which a connection should be made.  This parameter can be NULL to indicate
//               that the first robot discovered.  A list of valid names can be found through the use of the
//               mipTransportStartRobotDiscovery(), mipTransportGetDiscoveredRobotCount(),
//               mipTransportGetDiscoveredRobotName(), and mipTransportStopRobotDiscovery() functions.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportConnectToRobot(MiPTransport* pTransport, const char* pRobotName);

// Disconnect from MiP robot.
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportDisconnectFromRobot(MiPTransport* pTransport);

// Start the process of discovering MiP robots to which a connection can be made.
// This discovery process will continue until mipTransportStopRobotDiscovery() is called.  Once the discovery process
// has started, the mipTransportGetDiscoveredRobotCount() and mipTransportGetDiscoveredRobotName() functions can be
// called to query the current list of robots.  Those functions can still be called after calling
// mipTransportStopRobotDiscovery() but no new robots will be added to the list.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportStartRobotDiscovery(MiPTransport* pTransport);

// Query how many MiP robots the discovery process has found so far.
// The discovery process is started by calling mipTransportStartRobotDiscovery().  The count returned by this function
// can increase (if more and more robots are discovered over time) until mipTransportStopRobotDiscovery() is called.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   pCount: A pointer to where the current count of robots should be placed.  Shouldn't be NULL.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportGetDiscoveredRobotCount(MiPTransport* pTransport, size_t* pCount);

// Query the name of a specific MiP robot which the discovery process has found.
// The discovery process is started by calling mipTransportStartRobotDiscovery().  This function is used to index into
// the list of discovered robots to obtain its name.  This name can be later used as the pRobotName parameter of the
// mipTransportConnectToRobot() function.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   robotIndex: The index of the robot for which the name should be obtained.  It must be >= 0 and < the count returned
//               by mipTransportGetDiscoveredRobotCount().
//   ppRobotName: A pointer to where the robot name should be placed.  Shouldn't be NULL.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportGetDiscoveredRobotName(MiPTransport* pTransport, size_t robotIndex, const char** ppRobotName);

// Stops the process of discovering MiP robots to which a connection can be made.
// The discovery process is started with a call to mipTransportStartRobotDiscovery() and stops when this function is
// called.  MiP robots which were found between these two calls can be listed through the use of the
// mipTransportGetDiscoveredRobotCount() and mipTransportGetDiscoveredRobotName() functions.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportStopRobotDiscovery(MiPTransport* pTransport);

// Send a request to the MiP robot.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   pRequest: Is a pointer to the array of bytes to be sent to the robot.
//   requestLength: Is the number of bytes in the pRequest buffer to be sent to the robot.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportSendBytes(MiPTransport* pTransport, const uint8_t* pRequest, uint8_t requestLength);

// Retrieve MiP robot response data.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   pResponseBuffer: Is a pointer to the array of bytes into which the response should be copied.
//   responseBufferSize: Is the number of bytes in the pResponseBuffer.
//   pResponseLength: Is a pointer to where the actual number of bytes in the response should be placed.  This value
//                    may be truncated to responseBufferSize if the actual response was > responseBufferSize.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportReceiveBytes(MiPTransport* pTransport,
                                uint8_t* pResponseBuffer,
                                uint8_t responseBufferSize,
                                uint8_t* pResponseLength);


// Retrieve one byte of the MiP robot response data.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   pResponse: Is a pointer to byte into which the response should be copied.
//   Returns: MIP_ERROR_NONE on success and a non-zero MIP_ERROR_* code otherwise.
int8_t mipTransportReceiveByte(MiPTransport* pTransport, uint8_t* pResponseBuffer);

// How many bytes of response data does the transport have ready to be read.
//
//   Returns: The number of bytes ready to be processed by mipTransportReadByte(s).
//            non-zero if the response has been received.
uint8_t mipTransportResponseBytesAvailable(MiPTransport* pTransport);

// Discards unprocessed bytes from the response since the higher level code doesn't know how to process it.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   Returns: The number of bytes that were actually discarded.
uint8_t mipTransportDiscardUnusedBytes(MiPTransport* pTransport);

// Delay CPU execution for the specified number of milliseconds.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   milliseconds: Number of milliseconds to delay the CPU.
//   Returns: Nothing.
void mipTransportDelayMilliseconds(MiPTransport* pTransport, uint32_t milliseconds);

// Get the number of milliseconds the computer has been up and running using transport / platform specific
// functionality.
//
//   pTransport: An object that was previously returned from the mipTransportInit() call.
//   Returns: Millisecond count.
uint32_t mipTransportGetMilliseconds(MiPTransport* pTransport);

#ifdef __cplusplus
}
#endif

#endif // MIP_TRANSPORT_H_
