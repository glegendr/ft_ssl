#include "ft_ssl.h"
#include <libft.h>
#include <ft_printf.h>

uint64_t g_k3[80] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 
	0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 
	0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 
	0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 
	0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
	0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 
	0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 
	0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 
	0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 
	0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

#define ROTR(x, n) ((x >> n) | (x << (64 - n)))
#define SHR(x, n) (x >> n)

static void		print_hash(uint8_t *hash, t_hash *tab, int i)
{
	if ((tab->arg & P_FLAG) && !(tab->arg & Q_FLAG))
		printf("%s\n", tab->str[i]);
	if (!(tab->arg & R_FLAG) && !(tab->arg & Q_FLAG))
		printf("SHA256(%s)= ", tab->folder[i]);
	for (int i = 0; i < 64; i++)
		printf("%0.2x", hash[i]);
	if ((tab->arg & R_FLAG) && !(tab->arg & Q_FLAG))
		printf(" %s", tab->folder[i]);
	printf("\n");
}

static uint64_t	declare_chunk(uint8_t *ck, int i, uint64_t *w)
{
	uint64_t		s0;
	uint64_t		s1;
	uint64_t		x;
	int				y;

		s0 = 0;
		y = 0;
	if (i < 16) {
		i *= 4;
		while (y < 8)
		{
			s0 = s0 << 8;
			s0 = s0 + ck[i + y];
			++y;
		}
		ft_printf("%c %c %c %c %c %c %c %c %0.64b\n", ck[i], ck[i + 1], ck[i + 2], ck[i + 3], ck[i + 4], ck[i + 5], ck[i + 6], ck[i + 7], s0);
		return (s0);
	}
	x = w[i - 15];
	s0 = (ROTR(x, 1)) ^ (ROTR(x, 8)) ^ (SHR(x, 7));
	x = w[i - 2];
	s1 = (ROTR(x, 19)) ^ (ROTR(x, 61)) ^ (SHR(x, 6));
	return (w[i - 16] + s0 + w[i - 7] + s1);
}

static void to_bytes(uint64_t val, uint8_t *bytes)
{
	bytes[7] = (uint8_t) val;
	bytes[6] = (uint8_t) (val >> 8);
	bytes[5] = (uint8_t) (val >> 16);
	bytes[4] = (uint8_t) (val >> 24);
	bytes[3] = (uint8_t) (val >> 32);
	bytes[2] = (uint8_t) (val >> 40);
	bytes[1] = (uint8_t) (val >> 48);
	bytes[0] = (uint8_t) (val >> 56);
}

static void		lol(uint8_t *ck, int loop, uint8_t *ret)
{
	uint64_t m[80];
	uint64_t h0 = 0x6a09e667f3bcc908;
	uint64_t h1 = 0xbb67ae8584caa73b;
	uint64_t h2 = 0x3c6ef372fe94f82b;
	uint64_t h3 = 0xa54ff53a5f1d36f1;
	uint64_t h4 = 0x510e527fade682d1;
	uint64_t h5 = 0x9b05688c2b3e6c1f;
	uint64_t h6 = 0x1f83d9abfb41bd6b;
	uint64_t h7 = 0x5be0cd19137e2179;

	for (int y = 0; y < loop; ++y) {
		for (int i = 0; i < 80; ++i)
			m[i] = declare_chunk(i < 16 ? ck + 128 * y : NULL, i * 2, m);
		uint64_t a = h0;
		uint64_t b = h1;
		uint64_t c = h2;
		uint64_t d = h3;
		uint64_t e = h4;
		uint64_t f = h5;
		uint64_t g = h6;
		uint64_t h = h7;
		for (int i = 0; i < 64; ++i) {
			uint64_t s1 = ROTR(e, 14) ^ ROTR(e, 18) ^ ROTR(e, 41);
			uint64_t ch = (e & f) ^ ((~e) & g);
			uint64_t temp1 = h + s1 + ch + g_k3[i] + m[i];
			uint64_t s0 = ROTR(a, 28) ^ ROTR(a, 34) ^ ROTR(a, 39);
			uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint64_t temp2 = s0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}
		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
		h5 = h5 + f;
		h6 = h6 + g;
		h7 = h7 + h;
	}
	to_bytes(h0, ret);
	to_bytes(h1, ret + 8);
	to_bytes(h2, ret + 16);
	to_bytes(h3, ret + 24);
	to_bytes(h4, ret + 32);
	to_bytes(h5, ret + 40);
	to_bytes(h6, ret + 48);
	to_bytes(h7, ret + 56);
}

void		sha512(t_hash *tab)
{
	int		i;
	int		tmp;
	uint8_t	ret[64];

	i = 0;
	if (!tab->folder)
		print_usage(NULL);
	while (tab->folder[i])
	{
		tmp = pad_message(&(tab->str[i]), false, 128);
		for (int y = 0; y < 128; ++y)
			ft_printf("%0.8b ", tab->str[i][y]);
		printf("\n");
		lol(tab->str[i], tmp / 128, ret);
		print_hash(ret, tab, i);
//		free(ret);
		++i;
	}
	del_tab(tab->folder);
	del_tab((char **)tab->str);
}