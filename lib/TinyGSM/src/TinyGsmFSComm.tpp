/**
 * @file      TinyGsmFSComm.tpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2025  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2025-07-20
 *
 */
#ifndef SRC_TINYGSMFS_H_
#define SRC_TINYGSMFS_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_FS

template <class modemType, ModemPlatform platform>
class TinyGsmFSComm
{
public:
    /**
     * @brief Sets the filesystem path (partition) for file operations
     *
     * Configures the default partition (C or D) used for subsequent file operations.
     * Only accepts 'c', 'd' (case-insensitive) as valid partitions.
     *
     * @param path Character representing the partition ('c', 'd', 'C', or 'D')
     * @return true if the path was set successfully, false otherwise
     */
    bool fs_set_path(const char path)
    {
        if (path == 'c' || path == 'd' || path == 'C' || path == 'D' ) {
            PATH = path;
            return true;
        }
        return false;
    }

    /**
     * @brief Retrieves attributes (filesize) of a specified file
     *
     * Queries the filesystem for attributes of the specified file,
     * specifically retrieving the file size.
     *
     * @param filename Name of the file to check attributes for
     * @param size Reference to store the retrieved file size
     * @return 0 on success, -1 if the operation failed
     */
    int fs_attri(String filename, size_t &size)
    {
        thisModem().sendAT("+FSATTRI=", "\"",  PATH, ":", filename, "\"");
        if (thisModem().waitResponse("+FSATTRI: ", "ERROR") == 1) {
            size = thisModem().streamGetLongLongBefore('\r');
            return 0;
        }
        return -1;
    }

    /**
     * @brief Deletes a specified file from the filesystem
     *
     * Removes the specified file from the currently set partition.
     *
     * @param filename Name of the file to delete
     * @return 0 on successful deletion, -1 if the operation failed
     */
    int fs_del(String filename)
    {
        thisModem().sendAT("+FSDEL=", "\"",  PATH, ":", filename, "\"");
        return (thisModem().waitResponse() == 1)  ? 0 : -1;
    }

    /**
     * @brief Retrieves memory usage information of the filesystem
     *
     * Gets the total and used memory bytes of the currently set partition.
     *
     * @param totalBytes Reference to store the total available memory
     * @param usedBytes Reference to store the used memory
     */
    void fs_mem(size_t &totalBytes, size_t &usedBytes)
    {
        totalBytes = 0;
        usedBytes = 0;
        thisModem().sendAT("+FSMEM");
        if (thisModem().waitResponse("+FSMEM: ", "ERROR") == 1) {
            thisModem().streamSkipUntil('(');
            totalBytes = thisModem().streamGetLongLongBefore(',');
            usedBytes = thisModem().streamGetLongLongBefore(')');
            thisModem().waitResponse();
        }
    }

    /**
     * @brief Writes data to a file in chunks
     *
     * Transfers data to a file on the filesystem using chunked upload.
     * Supports resuming from a specified offset and allows configuring chunk size.
     *
     * @param filename Name of the file to write to
     * @param buffer Pointer to the data buffer containing content to write
     * @param size Total number of bytes to write
     * @param startOffset Offset from the beginning of the file to start writing (default: 0)
     * @param chunkSize Size of each data chunk to transfer (default: 512 bytes)
     * @return Number of bytes successfully written
     */
    size_t fs_write(String filename, const uint8_t *buffer, size_t size, size_t startOffset = 0, size_t chunkSize = 512)
    {
        size_t bytesWrite = startOffset;

        while (bytesWrite < size) {
            size_t currentChunkSize = min(chunkSize, size - bytesWrite);
            log_d("Writing chunk: %u-%u bytes (total: %u%%)\n", bytesWrite, bytesWrite + currentChunkSize - 1, (bytesWrite * 100) / size);
            thisModem().sendAT("+CFTRANRX=", "\"", PATH, ":", filename, "\",", currentChunkSize, ",", 100, ",", bytesWrite);
            if (thisModem().waitResponse(10000, ">") != 1) {
                log_e("Timeout waiting for data");
                goto cleanup;
            }
            thisModem().stream.write(&buffer[bytesWrite], currentChunkSize);
            if (thisModem().waitResponse(10000) != 1) {
                log_e("Chunk upload failed");
                goto cleanup;
            }
            bytesWrite += currentChunkSize;
        }
cleanup:
        return bytesWrite;
    }
    /**
     * @brief Reads data from a file in chunks
     *
     * Retrieves content from a file on the filesystem using chunked download.
     * Supports resuming from a specified offset and allows configuring chunk size.
     *
     * @param filename Name of the file to read from
     * @param buffer Pointer to the buffer where read data will be stored
     * @param size Maximum number of bytes to read
     * @param startOffset Offset from the beginning of the file to start reading (default: 0)
     * @param chunkSize Size of each data chunk to retrieve (default: 512 bytes)
     * @return Number of bytes successfully read
     */
    size_t fs_read(String filename, uint8_t *buffer, size_t size, size_t startOffset = 0, size_t chunkSize = 512)
    {
        size_t fileSize = 0;
        size_t bytesRead = startOffset;
        if (fs_attri(filename, fileSize) < 0) {
            return -1;
        }
        while (bytesRead < fileSize) {
            size_t currentChunkSize = min(chunkSize, fileSize - bytesRead);
            log_d("Reading chunk: %u-%u bytes (total: %u%%)\n", bytesRead, bytesRead + currentChunkSize - 1, (bytesRead * 100) / fileSize);
            thisModem().sendAT("+CFTRANTX=", "\"", PATH, ":", filename, "\",", bytesRead, ",", currentChunkSize, ",", 0);
            if (thisModem().waitResponse(10000, "+CFTRANTX: DATA,", "ERROR") != 1) {
                log_e("Timeout waiting for data");
                goto cleanup;
            }
            thisModem().streamSkipUntil('\n');
            thisModem().stream.readBytes(&buffer[bytesRead], currentChunkSize);
            if (thisModem().waitResponse(10000, "+CFTRANTX: 0", "ERROR") != 1) {
                log_e("Reading data failed");
                goto cleanup;
            }
            bytesRead += currentChunkSize;
        }
cleanup:
        return bytesRead;
    }


    /*
     * CRTP Helper
     */
protected:

    String PATH = "C";

    inline const modemType &thisModem() const
    {
        return static_cast<const modemType &>(*this);
    }
    inline modemType &thisModem()
    {
        return static_cast<modemType &>(*this);
    }
};


#endif  // SRC_TINYGSMFS_H_
