#pragma once

#include <Serial.hpp>

namespace Henry {
	namespace Platform {
		unsigned int getDeviceCount();
		char *getDevicePathByIndex(const unsigned int index);
	} // namespace Platform

	struct Arduino {
	  private:
		static constexpr unsigned int TICKS_PER_SECOND = 100;
		static constexpr double TICK_INTERVAL = 1.0 / TICKS_PER_SECOND;
		static constexpr unsigned int HANDSHAKE_ATTEMPTS = 10;
		enum Flags {
			NONE = 0,
			INIT_STATUS = 1,
			CONNECTION_STATUS = 2,
			VALID_STATUS = 4
		};

		SerialDevice mSerialDevice;
		double mTickBegin;
		unsigned int mRsize, mTsize;
		unsigned char *mRdataBuffer, *mTdataBuffer;
		unsigned short mFlags;
		unsigned char mKey, mTick;
		void (*mOnUpdate)(Arduino &);

	  public:
		unsigned char *mRdata, *mTdata;

		Arduino();
		Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key);
		Arduino(const Arduino &a) = delete;
		Arduino(Arduino &&a) = delete;
		~Arduino();

		void connect(const unsigned int rsize, const unsigned int tsize, const unsigned int baudrate, const unsigned char key);
		void disconnect();
		void update();
		inline void onUpdate(void (*const f)(Arduino &)) { mOnUpdate = f; }
		inline bool isConnected() const { return mFlags & CONNECTION_STATUS; }
		inline bool isValid() const { return mFlags & VALID_STATUS; }
		void operator=(const Arduino &a) = delete;
		void operator=(Arduino &&a) = delete;
	};
} // namespace Henry
