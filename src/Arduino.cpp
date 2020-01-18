#include "Arduino.hpp"

#define DEBUG_ENABLE_TIMING
#define DEBUG_ENABLE_LOGGING
#include <Debug/Debug.hpp>

namespace Henry {
    namespace Platform {
        unsigned int getDeviceCount();
        char *getDevicePathByIndex(const unsigned int aIndex);
    } // namespace Platform

    static void defaultArduinoUpdate(Arduino &) {}

    Arduino::Arduino()
        : mSerialDevice(), mTickBegin(0), mRSize(0), mTSize(0), mRDataBuffer(nullptr), mTDataBuffer(nullptr), mFlags(NONE),
          mKey(0), mTick(0), mOnUpdate(defaultArduinoUpdate), mRData(nullptr), mTData(nullptr) {}
    Arduino::Arduino(const unsigned int aRSize, const unsigned int aTSize, const unsigned int aBaudrate,
                     const unsigned char aKey)
        : mSerialDevice(), mTickBegin(0), mRSize(aRSize + 2), mTSize(aTSize + 2), mRDataBuffer(new unsigned char[mRSize]),
          mTDataBuffer(new unsigned char[mTSize]), mFlags(INIT_STATUS), mKey(aKey), mTick(0), mOnUpdate(defaultArduinoUpdate),
          mRData(mRDataBuffer + 1), mTData(mTDataBuffer + 1) {
        Connect(aRSize, aTSize, aBaudrate, aKey);
    }
    Arduino::~Arduino() { Disconnect(); }

    void Arduino::Connect(const unsigned int aRSize, const unsigned int aTSize, const unsigned int aBaudrate,
                          const unsigned char aKey) {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (!(mFlags & INIT_STATUS)) {
            mRSize = aRSize + 2, mTSize = aTSize + 2, mKey = aKey;
            mRDataBuffer = new unsigned char[mRSize];
            mTDataBuffer = new unsigned char[mRSize];
        }

        mRDataBuffer[0] = mKey, mRDataBuffer[mRSize] = mKey;
        for (mRData = mRDataBuffer + 1; mRData < mRDataBuffer - 1; ++mRData)
            *mRData = 0;
        mTDataBuffer[0] = mKey, mTDataBuffer[mTSize] = mKey;
        for (mTData = mTDataBuffer + 1; mTData < mTDataBuffer - 1; ++mTData)
            *mTData = 0;
        mRData = mRDataBuffer + 1, mTData = mTDataBuffer + 1;

        if (!mOnUpdate) mOnUpdate = defaultArduinoUpdate;

        for (unsigned int tIndex = 0; tIndex < Platform::getDeviceCount(); ++tIndex) {
            const char *const tFilepath = Platform::getDevicePathByIndex(tIndex);
            if (mSerialDevice.Open(tFilepath, aBaudrate)) {
                Debug::Log::message("Attempting to shake hands with %s...", tFilepath);
                for (unsigned int i = 0; i < HANDSHAKE_ATTEMPTS; ++i) {
                    mSerialDevice.Write(mTDataBuffer, mTSize);
                    Debug::Timer::sleep(TICK_INTERVAL);
                    mSerialDevice.Read(mRDataBuffer, mRSize);
                    Debug::Timer::sleep(TICK_INTERVAL);

                    for (unsigned int i = 0; i < mRSize; ++i)
                        Debug::Log::info("%0*x ", 2, mRDataBuffer[i]);
                    Debug::Log::info(": %0*x\n", 2, mKey);

                    if (mRDataBuffer[0] == mKey && mRDataBuffer[mRSize - 1] == mKey) {
                        Debug::Log::success("Opened arduino '%s' at %d baud, using key '%x'.", tFilepath, aBaudrate, mKey);
                        mTickBegin = Debug::Timer::now();
                        mFlags |= CONNECTION_STATUS;
                        return;
                    }
                }
                mSerialDevice.Close();
            }
        }
        mFlags &= ~CONNECTION_STATUS;
        Debug::Log::error("Failed to find arduino at %d baud with the key '%x'", aBaudrate, mKey);
    }

    void Arduino::Disconnect() {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (IsConnected()) {
            if (mRDataBuffer) delete mRDataBuffer;
            if (mTDataBuffer) delete mTDataBuffer;
            mRData = nullptr, mRDataBuffer = nullptr;
            mTData = nullptr, mTDataBuffer = nullptr;
            mFlags = NONE;
            mSerialDevice.Close();
        }
    }

    void Arduino::Update() {
        const double tCurrentTime = Debug::Timer::now();
        if (tCurrentTime - mTickBegin > TICK_INTERVAL) {
            DEBUG_BEGIN_FUNC_PROFILE;

            switch (mTick) {
            case 0:
                mSerialDevice.Write(mTDataBuffer, mTSize);
                mTick = 1;
                break;
            case 1:
                mSerialDevice.Read(mRDataBuffer, mRSize);
                if (mRDataBuffer[0] == mKey && mRDataBuffer[mRSize - 1] == mKey) {
                    // mTickBegin = Debug::Timer::now();
                    mFlags |= VALID_STATUS;
                } else {
                    Debug::Log::error("failed to find the key (%x) when reading the device.", mKey);
                    mFlags &= ~VALID_STATUS;
                }
                DEBUG_BEGIN_PROFILE(User_defined_update_function);
                mOnUpdate(*this);
                DEBUG_END_PROFILE(User_defined_update_function);
                mTick = 0;
                break;
            }

            mTickBegin += TICK_INTERVAL;
        }
    }
} // namespace Henry
