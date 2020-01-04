//
// Created by przemek on 20.11.2019.
//

#ifndef SIMPLE_P2P_GENERALTYPES_H
#define SIMPLE_P2P_GENERALTYPES_H

#include <cinttypes>

#define Uint8  uint8_t
#define Uint16 uint16_t
#define Uint32 uint32_t
#define Uint64 uint64_t

#define Int8 int8_t
#define Int16 int16_t
#define Int32 int32_t
#define Int64 int64_t

#define REQ_SEGMENT 8
#define REVOKE 16
#define FILE_LIST 32

#define BEACON_MAX_COUNT 3

#define RESOURCE_HEADER_SIZE 264

#define UDP_SERV_BUFFER_SIZE (sizeof(Uint8)+sizeof(Uint64)+(BEACON_MAX_COUNT*RESOURCE_HEADER_SIZE))

#define BROADCAST_ADDRESS "192.168.1.255"

#endif //SIMPLE_P2P_GENERALTYPES_H
