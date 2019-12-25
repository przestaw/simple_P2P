//
// Created by przemek on 20.11.2019.
//

#ifndef SIMPLE_P2P_GENERALTYPES_H
#define SIMPLE_P2P_GENERALTYPES_H

#include <cinttypes>

#define Uint8 uint_fast8_t
#define Uint16 uint_fast16_t
#define Uint32 uint_fast32_t
#define Uint64 uint_fast64_t

#define Int8 int_fast8_t
#define Int16 int_fast16_t
#define Int32 int_fast32_t
#define Int64 int_fast64_t

#define REQ_SEGMENT 8
#define REVOKE 16
#define FILE_LIST 32

#define BROADCAST_ADDRESS "192.168.1.255"
#define SegmentId Uint16

#endif //SIMPLE_P2P_GENERALTYPES_H
