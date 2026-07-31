#ifndef SHA3_H
#define SHA3_H

// SHA3-256 (Keccak, FIPS 202), adapted from the public-domain
// reference implementation by Markku-Juhani O. Saarinen (tiny_sha3).
// Kept dependency-free and header-only to match this project's
// other hand-rolled ciphers (see md5.h).

#include <stdint.h>
#include <string.h>
#include <stdio.h>

class SHA3_256
{
private:
	static const int KECCAK_ROUNDS = 24;
	static const int RATE_BYTES = 136;		// 200 - 2*32, rate for a 256-bit digest
	static const int DIGEST_BYTES = 32;	// 256-bit digest

	static uint64_t Rotl64(uint64_t x, int y)
	{
		return (x << y) | (x >> (64 - y));
	}

	static void Keccakf(uint64_t st[25])
	{
		static const uint64_t rndc[24] = {
			0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
			0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
			0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
			0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
			0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
			0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
			0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
			0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
		};
		static const int rotc[24] = {
			1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
			27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44
		};
		static const int piln[24] = {
			10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
			15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1
		};

		int i, j, r;
		uint64_t t, bc[5];

		for (r = 0; r < KECCAK_ROUNDS; r++)
		{
			// Theta
			for (i = 0; i < 5; i++)
				bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];

			for (i = 0; i < 5; i++)
			{
				t = bc[(i + 4) % 5] ^ Rotl64(bc[(i + 1) % 5], 1);
				for (j = 0; j < 25; j += 5)
					st[j + i] ^= t;
			}

			// Rho Pi
			t = st[1];
			for (i = 0; i < 24; i++)
			{
				j = piln[i];
				bc[0] = st[j];
				st[j] = Rotl64(t, rotc[i]);
				t = bc[0];
			}

			// Chi
			for (j = 0; j < 25; j += 5)
			{
				for (i = 0; i < 5; i++)
					bc[i] = st[j + i];
				for (i = 0; i < 5; i++)
					st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
			}

			// Iota
			st[0] ^= rndc[r];
		}
	}

	union
	{
		uint8_t b[200];
		uint64_t q[25];
	} state;

	int pt;

public:
	// a SHA3-256 digest is a 32-byte number (64 hex digits)
	uint8_t digestRaw[DIGEST_BYTES];

	// This version of the digest is actually
	// a "printf'd" version of the digest.
	char digestChars[DIGEST_BYTES * 2 + 1];

	SHA3_256()
	{
		Init();
	}

	void Init()
	{
		memset(state.b, 0, sizeof(state.b));
		pt = 0;
	}

	void Update(unsigned char *input, unsigned int inputLen)
	{
		for (unsigned int i = 0; i < inputLen; i++)
		{
			state.b[pt++] ^= input[i];
			if (pt >= RATE_BYTES)
			{
				Keccakf(state.q);
				pt = 0;
			}
		}
	}

	void Final()
	{
		state.b[pt] ^= 0x06;
		state.b[RATE_BYTES - 1] ^= 0x80;
		Keccakf(state.q);

		memcpy(digestRaw, state.b, DIGEST_BYTES);

		for (int pos = 0; pos < DIGEST_BYTES; pos++)
			sprintf(digestChars + (pos * 2), "%02x", digestRaw[pos]);

		// Zeroize sensitive information.
		memset(&state, 0, sizeof(state));
	}

	// Digests a string and prints the result.
	char* digestString(char *string)
	{
		Init();
		Update((unsigned char*)string, (unsigned int)strlen(string));
		Final();

		return digestChars;
	}
};

#endif
