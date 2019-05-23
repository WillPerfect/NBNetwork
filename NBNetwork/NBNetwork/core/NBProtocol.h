#pragma once
#include <cstdint>

namespace nbnetwork {
#pragma pack(1)

	struct NBHead
	{
		std::uint16_t sign() const { return ntohs(m_sign); }
		std::uint16_t version() const { return ntohs(m_version); }
		std::uint32_t datasize() const { return ntohl(m_dataSize); }

		void setSign(std::uint16_t _sign) { m_sign = htons(_sign); }
		void setVersion(std::uint16_t _version) { m_version = htons(_version); }
		void setDataSize(std::uint32_t _datasize) { m_dataSize = htonl(_datasize); }
	private:
		std::uint16_t m_sign = 0;
		std::uint16_t m_version = 0x32;
		std::uint32_t m_dataSize = 0;
	};
	
#pragma pack()
}
