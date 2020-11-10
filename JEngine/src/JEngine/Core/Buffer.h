#pragma once
#include "core.h"

namespace JEngine {
	struct Buffer {
		byte* Data;
		uint32_t Size;

		Buffer():Data(nullptr),Size(0){}
		Buffer(byte* data,uint32_t size):Data(data),Size(size){}
		
		void Allocate(uint32_t size) {
			delete[] Data;
			Data = nullptr;
			if (size == 0) return;
			Data = new byte[size];
			Size = size;
		}

		static Buffer Copy(void* data, uint32_t size) {
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void ZeroInitialize() {
			if (Data) memset(Data, 0, Size);
		}

		void Write(byte* data, uint32_t size, uint32_t offset = 0) {
			JE_CORE_ASSERT(offset + size <= Size, "Buffer Write Overflow");
			memcpy(Data + offset, data, size);
		}

		explicit operator bool() const {
			return Data;
		}

		byte& operator[](int index) {
			return Data[index];
		}

		byte operator[](int index) const {
			return Data[index];
		}

		template<typename T>
		T* As() {
			return static_cast<T>(Data);
		}
		
		inline uint32_t GetSize() const { return Size; }
	};
}