#include "Arduino.hpp"

namespace Henry {
	static void defaultArduinoUpdate(Arduino &) {}

	Arduino::Arduino() :
	mSerialDevice(), mTickBegin(0), mRsize(0), mTsize(0), mRdataBuffer(nullptr), mTdataBuffer(nullptr), mFlags(NONE),
	mKey(0), mTick(0), mOnUpdate(defaultArduinoUpdate), mRdata(nullptr), mTdata(nullptr) {}

	Arduino::Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key) :
	mSerialDevice(), mTickBegin(0), mRsize(rsize + 2), mTsize(tsize + 2), mRdataBuffer(new unsigned char[mRsize]),
	mTdataBuffer(new unsigned char[mTsize]), mFlags(INIT_STATUS), mKey(key), mTick(0), mOnUpdate(defaultArduinoUpdate),
	mRdata(mRdataBuffer + 1), mTdata(mTdataBuffer + 1) {
		connect(rsize, tsize, baudrate, key);
	}

	Arduino::~Arduino() { disconnect(); }

	void Arduino::connect(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key) {
		if (!(mFlags & INIT_STATUS)) {
			mRsize = rsize + 2, mTsize = tsize + 2, mKey = key;
			mRdataBuffer = new unsigned char[mRsize];
			mTdataBuffer = new unsigned char[mRsize];
		}

		mRdataBuffer[0] = mKey, mRdataBuffer[mRsize] = mKey;
		for (mRdata = mRdataBuffer + 1; mRdata < mRdataBuffer - 1; ++mRdata)
			*mRdata = 0;
		mTdataBuffer[0] = mKey, mTdataBuffer[mTsize] = mKey;
		for (mTdata = mTdataBuffer + 1; mTdata < mTdataBuffer - 1; ++mTdata)
			*mTdata = 0;
		mRdata = mRdataBuffer + 1, mTdata = mTdataBuffer + 1;

		if (!mOnUpdate) mOnUpdate = defaultArduinoUpdate;

		for (unsigned int i = 0; i < Platform::getDeviceCount(); ++i) {
			const char *const tFilepath = Platform::getDevicePathByIndex(i);
			if (mSerialDevice.open(tFilepath, baudrate)) {
				for (unsigned int j = 0; j < HANDSHAKE_ATTEMPTS; ++j) {
					mSerialDevice.writeBuffer(mTdataBuffer, mTsize);
					// Debug::Timer::sleep(TICK_INTERVAL);
					mSerialDevice.readBuffer(mRdataBuffer, mRsize);
					// Debug::Timer::sleep(TICK_INTERVAL);
					if (mRdataBuffer[0] == mKey && mRdataBuffer[mRsize - 1] == mKey) {
						// mTickBegin = Debug::Timer::now();
						mFlags |= CONNECTION_STATUS;
						return;
					}
				}
				mSerialDevice.close();
			}
		}
		mFlags &= ~CONNECTION_STATUS;
	}

	void Arduino::disconnect() {
		if (isConnected()) {
			if (mRdataBuffer) delete[] mRdataBuffer;
			if (mTdataBuffer) delete[] mTdataBuffer;
			mRdata = nullptr, mRdataBuffer = nullptr;
			mTdata = nullptr, mTdataBuffer = nullptr;
			mFlags = NONE;
			mSerialDevice.close();
		}
	}

	void Arduino::update() {
		const double tCurrentTime = 0; //Debug::Timer::now();
		if (tCurrentTime - mTickBegin > TICK_INTERVAL) {
			switch (mTick) {
			case 0:
				mSerialDevice.writeBuffer(mTdataBuffer, mTsize);
				mTick = 1;
				break;
			case 1:
				mSerialDevice.readBuffer(mRdataBuffer, mRsize);
				if (mRdataBuffer[0] == mKey && mRdataBuffer[mRsize - 1] == mKey) {
					mFlags |= VALID_STATUS;
				} else {
					mFlags &= ~VALID_STATUS;
				}
				mOnUpdate(*this);
				mTick = 0;
				break;
			}
			mTickBegin += TICK_INTERVAL;
		}
	}
} // namespace Henry
