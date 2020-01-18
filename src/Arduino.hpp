#pragma once

#include <Henry/Serial.hpp>

namespace Henry {
    struct Arduino {
      private:
        static constexpr unsigned int TICKS_PER_SECOND = 100;
        static constexpr double TICK_INTERVAL = 1.0 / TICKS_PER_SECOND;
        static constexpr unsigned int HANDSHAKE_ATTEMPTS = 10;
        enum Flags { NONE = 0, INIT_STATUS = 1, CONNECTION_STATUS = 2, VALID_STATUS = 4 };

        Serial mSerialDevice;
        double mTickBegin;
        unsigned int mRSize, mTSize;
        unsigned char *mRDataBuffer, *mTDataBuffer;
        unsigned short mFlags;
        unsigned char mKey, mTick;
        void (*mOnUpdate)(Arduino &);

      public:
        unsigned char *mRData, *mTData;

        Arduino();
        Arduino(const unsigned int aRSize, const unsigned int aTSize, const unsigned int aBaudrate, const unsigned char aKey);
        ~Arduino();

        void Connect(const unsigned int aRSize, const unsigned int aTSize, const unsigned int aBaudrate,
                     const unsigned char aKey);
        void Disconnect();
        void Update();
        inline void OnUpdate(void (*const aFunc)(Arduino &)) { mOnUpdate = aFunc; }
        inline bool IsConnected() const { return mFlags & CONNECTION_STATUS; }
        inline bool IsValid() const { return mFlags & VALID_STATUS; }
    };
} // namespace Henry
