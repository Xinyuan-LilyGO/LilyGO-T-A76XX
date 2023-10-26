#ifndef MESSAGE_WRITER_H
#define MESSAGE_WRITER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t MessageWriter_begin          (void);
size_t MessageWriter_end            (void);

size_t MessageWriter_writeString    (const char* value);
size_t MessageWriter_writeBinary    (buffer_t value);
size_t MessageWriter_writeBool      (bool value);
size_t MessageWriter_writeInt8      (const int8_t value);
size_t MessageWriter_writeInt16     (const int16_t value);
size_t MessageWriter_writeInt32     (const int32_t value);
size_t MessageWriter_writeInt64     (const int64_t value);
size_t MessageWriter_writeUInt8     (const uint8_t value);
size_t MessageWriter_writeUInt16    (const uint16_t value);
size_t MessageWriter_writeUInt32    (const uint32_t value);
size_t MessageWriter_writeUInt64    (const uint64_t value);
size_t MessageWriter_writeFloat     (const float value);
size_t MessageWriter_write          (const void* data, size_t size);

uint16_t MessageWriter_beginInvoke        (uint16_t uid);
void     MessageWriter_beginOneway        (uint16_t uid);
void     MessageWriter_beginResult        (uint16_t seq, uint8_t status);
void     MessageWriter_sendResultStatus   (uint16_t seq, uint8_t status);

#ifdef __cplusplus
}
#endif

#endif /* MESSAGE_WRITER_H */
