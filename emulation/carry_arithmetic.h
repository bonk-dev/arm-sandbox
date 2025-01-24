#pragma once
#include "nzcv.h"
#include <cstdint>

namespace Emulation {
	template<typename IntType>
	IntType add_with_carry(IntType a, IntType b, bool carry) {
		uint64_t discarded_nzcv = 0;
		return add_with_carry(a, b, carry, discarded_nzcv);
	}

	template<typename UnsignedIntType, typename SignedIntType>
	UnsignedIntType add_with_carry(UnsignedIntType a, UnsignedIntType b, bool carry, uint64_t& nzcv) {
		const UnsignedIntType u_sum = a + b + static_cast<unsigned int>(carry);
		const UnsignedIntType s_sum = static_cast<SignedIntType>(a) + static_cast<SignedIntType>(b) + static_cast<unsigned int>(carry);

		nzcv = nzcv::negative(nzcv, s_sum < 0);
		nzcv = nzcv::zero(nzcv, u_sum == 0);
		nzcv = nzcv::carry(nzcv, u_sum < a);
		nzcv = nzcv::overflow(nzcv, s_sum < static_cast<SignedIntType>(a));

		return u_sum;
	}
}