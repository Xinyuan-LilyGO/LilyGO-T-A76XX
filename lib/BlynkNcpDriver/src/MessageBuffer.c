#include "MessageBuffer.h"

void MessageBuffer_init(MessageBuffer* self, uint8_t* buffer, size_t size) {
    self->_buffer = buffer;
    self->_size = size;
    MessageBuffer_reset(self);
}

void MessageBuffer_rewind(MessageBuffer* self) {
    self->_rpos = 0;
    self->_error = false;
}

void MessageBuffer_reset(MessageBuffer* self) {
    self->_wpos = self->_rpos = 0;
    self->_error = false;
}

void MessageBuffer_setBuffer(MessageBuffer* self, uint8_t* buffer, size_t size) {
    self->_buffer = buffer;
    self->_size = size;
    MessageBuffer_reset(self);
}

uint8_t* MessageBuffer_getBuffer(MessageBuffer* self) {
    return self->_buffer;
}

size_t MessageBuffer_getSize(MessageBuffer* self) {
    return self->_size;
}

size_t MessageBuffer_getWritten(MessageBuffer* self) {
    return self->_wpos;
}

void MessageBuffer_setWritten(MessageBuffer* self, size_t s) {
    self->_wpos = s;
}

size_t MessageBuffer_availableToRead(MessageBuffer* self) {
    return self->_wpos - self->_rpos;
}

size_t MessageBuffer_availableToWrite(MessageBuffer* self) {
    return self->_size - self->_wpos;
}

bool MessageBuffer_getError(MessageBuffer* self) {
    return self->_error;
}

size_t MessageBuffer_readString(MessageBuffer* self, const char** value) {
    char* ptr = (char*)(self->_buffer + self->_rpos);
    size_t len = strlen(ptr) + 1;
    if (MessageBuffer_availableToRead(self) < len) {
        if (value) {
            *value = "";
        }
        self->_error = true;
        return 0;
    }
    if (value) {
        *value = ptr;
    }
    self->_rpos += len;
    return len;
}

size_t MessageBuffer_readBinary(MessageBuffer* self, buffer_t* value) {
    uint16_t len;
    if (!MessageBuffer_readUInt16(self, &len)) {
        if (value) {
            memset(value, 0, sizeof(buffer_t));
        }
        return 0;
    }
    return (sizeof(len) + MessageBuffer_readFixedBuffer(self, value, len));
}

size_t MessageBuffer_readFixedBuffer(MessageBuffer* self, buffer_t* value, unsigned len) {
    if (MessageBuffer_availableToRead(self) < len) {
        if (value) {
            memset(value, 0, sizeof(buffer_t));
        }
        self->_error = true;
        return 0;
    }
    if (value) {
        value->data = self->_buffer + self->_rpos;
        value->length = len;
    }
    self->_rpos += len;
    return len;
}

size_t MessageBuffer_readBool(MessageBuffer* self, bool* value) {
    uint8_t raw_value;
    if (!MessageBuffer_readUInt8(self, &raw_value)) {
        if (value) {
            *value = false;
        }
        return 0;
    }
    *value = (raw_value != 0);
    return 1;
}

size_t MessageBuffer_readInt8(MessageBuffer* self, int8_t* value) {
    return MessageBuffer_read(self, value, sizeof(int8_t));
}

size_t MessageBuffer_readInt16(MessageBuffer* self, int16_t* value) {
    return MessageBuffer_read(self, value, sizeof(int16_t));
}

size_t MessageBuffer_readInt32(MessageBuffer* self, int32_t* value) {
    return MessageBuffer_read(self, value, sizeof(int32_t));
}

size_t MessageBuffer_readInt64(MessageBuffer* self, int64_t* value) {
    return MessageBuffer_read(self, value, sizeof(int64_t));
}

size_t MessageBuffer_readUInt8(MessageBuffer* self, uint8_t* value) {
    return MessageBuffer_read(self, value, sizeof(uint8_t));
}

size_t MessageBuffer_readUInt16(MessageBuffer* self, uint16_t* value) {
    return MessageBuffer_read(self, value, sizeof(uint16_t));
}

size_t MessageBuffer_readUInt32(MessageBuffer* self, uint32_t* value) {
    return MessageBuffer_read(self, value, sizeof(uint32_t));
}

size_t MessageBuffer_readUInt64(MessageBuffer* self, uint64_t* value) {
    return MessageBuffer_read(self, value, sizeof(uint64_t));
}

size_t MessageBuffer_readFloat(MessageBuffer* self, float* value) {
    return MessageBuffer_read(self, value, sizeof(float));
}

size_t MessageBuffer_read(MessageBuffer* self, void* data, size_t size) {
    if (MessageBuffer_availableToRead(self) < size) {
        memset(data, 0, size);
        self->_error = true;
        return 0;
    }
    if (data) {
        memcpy(data, self->_buffer + self->_rpos, size);
    }
    self->_rpos += size;
    return size;
}
