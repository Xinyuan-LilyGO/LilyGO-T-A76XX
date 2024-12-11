/**
 * @file      TinyGsmTextToSpeech.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co.,
 * Ltd
 * @date      2024-12-05
 *
 */

#ifndef SRC_TINYGSMTEXTTOSPEECH_H_
#define SRC_TINYGSMTEXTTOSPEECH_H_

#include "TinyGsmCommon.h"

#define TINY_GSM_MODEM_HAS_TTS

template <class modemType>
class TinyGsmTextToSpeech {
 public:
  /*
   * TTS functions
   */

  /**
   * @param  mode:
   * 1 - Start to synth and play,<text> is in UCS2 coding format.
   * * 2 - Start to synth and play,<text> is in ASCII coding format,Chinese text
   * is in GBK coding format.
   */
  bool textToSpeech(String& text, uint8_t mode = 2) {
    if (mode > 2) {
      DBG("No support other mode!");
      return false;
    }
    return thisModem().textToSpeechImpl(text, mode);
  }

  /*
   * CRTP Helper
   */
 protected:
  inline const modemType& thisModem() const {
    return static_cast<const modemType&>(*this);
  }
  inline modemType& thisModem() {
    return static_cast<modemType&>(*this);
  }

  bool textToSpeechImpl(String& text,
                        uint8_t mode) TINY_GSM_ATTR_NOT_IMPLEMENTED;
};

#endif  // SRC_TINYGSMTEXTTOSPEECH_H_
