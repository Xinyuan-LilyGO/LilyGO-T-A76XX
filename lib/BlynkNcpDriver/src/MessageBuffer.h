#ifndef MESSAGE_BUFFER_H
#define MESSAGE_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct buffer_t {
    uint8_t* data;
    size_t   length;
} buffer_t;

typedef struct MessageBuffer {
    uint8_t*  _buffer;
    size_t    _size;        /* buffer size */
    size_t    _wpos;        /* write position */
    size_t    _rpos;        /* read position */
    bool      _error;       /* error flag */
} MessageBuffer;

void    MessageBuffer_init(MessageBuffer* self, uint8_t* buffer, size_t size);
void    MessageBuffer_rewind(MessageBuffer* self);
void    MessageBuffer_reset(MessageBuffer* self);
void    MessageBuffer_setBuffer(MessageBuffer* self, uint8_t* buffer, size_t size);
uint8_t* MessageBuffer_getBuffer(MessageBuffer* self);
size_t  MessageBuffer_getSize(MessageBuffer* self);
size_t  MessageBuffer_getWritten(MessageBuffer* self);
void    MessageBuffer_setWritten(MessageBuffer* self, size_t s);
size_t  MessageBuffer_availableToRead(MessageBuffer* self);
size_t  MessageBuffer_availableToWrite(MessageBuffer* self);
bool    MessageBuffer_getError(MessageBuffer* self);

/* Read */
size_t MessageBuffer_readString(MessageBuffer* self, const char** value);
size_t MessageBuffer_readBinary(MessageBuffer* self, buffer_t* value);
size_t MessageBuffer_readFixedBuffer(MessageBuffer* self, buffer_t* value, unsigned len);
size_t MessageBuffer_readBool(MessageBuffer* self, bool* value);
size_t MessageBuffer_readInt8(MessageBuffer* self, int8_t* value);
size_t MessageBuffer_readInt16(MessageBuffer* self, int16_t* value);
size_t MessageBuffer_readInt32(MessageBuffer* self, int32_t* value);
size_t MessageBuffer_readInt64(MessageBuffer* self, int64_t* value);
size_t MessageBuffer_readUInt8(MessageBuffer* self, uint8_t* value);
size_t MessageBuffer_readUInt16(MessageBuffer* self, uint16_t* value);
size_t MessageBuffer_readUInt32(MessageBuffer* self, uint32_t* value);
size_t MessageBuffer_readUInt64(MessageBuffer* self, uint64_t* value);
size_t MessageBuffer_readFloat(MessageBuffer* self, float* value);

size_t MessageBuffer_read(MessageBuffer* self, void* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* MESSAGE_BUFFER_H */
