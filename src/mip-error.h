/* Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

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
/* This header file describes the public API that an application can use to communicate with the WowWee MiP
   self-balancing robot.
*/
#ifndef MIP_ERROR_H_
#define MIP_ERROR_H_

// Integer error codes that can be encountered by MiP API functions.
#define MIP_ERROR_NONE          0 // Success
#define MIP_ERROR_TIMEOUT       1 // Timed out waiting for response.
#define MIP_ERROR_NO_EVENT      2 // No event has arrived from MiP yet.
#define MIP_ERROR_BAD_RESPONSE  3 // Unexpected response from MiP.
#define MIP_ERROR_MAX_RETRIES   4 // Exceeded maximum number of retries to get this operation to succeed.
#define MIP_ERROR_EMPTY         5 // The response queue is empty.
#define MIP_ERROR_PARAM         6 // Invalid parameter used in API call.
#define MIP_ERROR_CONNECT       7 // Failed to connect o MiP.
#define MIP_ERROR_NOT_CONNECTED 8 // Not connected to MiP.
#define MIP_ERROR_MEMORY        9 // Failed to allocate memory.

#endif // MIP_ERROR_H_
