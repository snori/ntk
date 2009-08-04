/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: hash.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/hash.h"


#include <ntk/support/status.h>


namespace ntk {


//########################################################


// table of prime numbers (2^n+a) --- éQçl: ruby/st.c
static const uint PRIME_NUMBERS[] = {
	5,
	8 + 3,
	16 + 3,
	32 + 5,
	64 + 3,
	128 + 3,
	256 + 29,
	512 + 17,
	1024 + 9,
	2048 + 5,
	4096 + 83,
	8192 + 27,
	16384 + 43,
	32768 + 3,
	65536 + 45,
	131072 + 9,
	262144 + 39,
	524288 + 39,
	1048576 + 9,
	2097152 + 5,
	4194304 + 3,
	8388608 + 33,
	16777216 + 27,
	33554432 + 9,
	67108864 + 71,
	134217728 + 39,
	268435456 + 9,
	536870912 + 5,
	1073741824 + 83,
	0
};

int
hash_table_new_size(uint size)
{
	enum{ NUM_OF_NUMBERS = sizeof(PRIME_NUMBERS)/sizeof(PRIME_NUMBERS[0]) };

	for(int i=0; i<NUM_OF_NUMBERS; ++i)
		if(size <= PRIME_NUMBERS[i])
			return PRIME_NUMBERS[i];

	status_t(st::ERR, "óvëfêîÇ™ëΩÇ∑Ç¨Ç‹Ç∑").show_error();

	return -1;
}

// to instantiate the template class
//HashTable<int> table_for_test;


//########################################################
}// namespace ntk
