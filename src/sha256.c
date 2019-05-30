#include "ft_ssl.h"
#include <libft.h>
#include <ft_printf.h>

uint32_t g_k256[64] = {
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define SHR(x, n) (x >> n)

static uint32_t	declare_chunk(uint8_t *ck, int i, uint32_t *w)
{
	uint32_t		s0;
	uint32_t		s1;
	uint32_t		x;

	if (i < 16) {
		i *= 4;
		s0 = (ck[i] << 24) + (ck[i + 1] << 16) + (ck[i + 2] << 8) + ck[i + 3];
		ft_printf("%0.8b %0.8b %0.8b %0.8b %0.32b\n", ck[i],ck[i+1],ck[i+2],ck[i+3],s0);
		return (s0);
	}
	x = w[i - 15];
	s0 = (ROTR(x, 7)) ^ (ROTR(x, 18)) ^ (SHR(x, 3));
	x = w[i - 2];
	s1 = (ROTR(x, 17)) ^ (ROTR(x, 19)) ^ (SHR(x, 10));
	return (w[i - 16] + s0 + w[i - 7] + s1);
}

static void to_bytes(uint32_t val, uint8_t *bytes)
{
	bytes[3] = (uint8_t) val;
	bytes[2] = (uint8_t) (val >> 8);
	bytes[1] = (uint8_t) (val >> 16);
	bytes[0] = (uint8_t) (val >> 24);
}

#define WTF 4

static void		lol(uint8_t *ck, int loop, uint8_t *ret, int type)
{
	uint32_t m[64];
	uint32_t h0;
	uint32_t h1;
	uint32_t h2;
	uint32_t h3;
	uint32_t h4;
	uint32_t h5;
	uint32_t h6;
	uint32_t h7;

	if (type == 256) {
	h0 = 0x6a09e667;
	h1 = 0xbb67ae85;
	h2 = 0x3c6ef372;
	h3 = 0xa54ff53a;
	h4 = 0x510e527f;
	h5 = 0x9b05688c;
	h6 = 0x1f83d9ab;
	h7 = 0x5be0cd19;
	} else {
	h0 = 0xc1059ed8;
	h1 = 0x367cd507;
	h2 = 0x3070dd17;
	h3 = 0xf70e5939;
	h4 = 0xffc00b31;
	h5 = 0x68581511;
	h6 = 0x64f98fa7;
	h7 = 0xbefa4fa4;
	}

	for (int y = 0; y < loop; ++y) {
		for (int i = 0; i < 64; ++i)
			m[i] = declare_chunk(i < 16 ? ck + 64 * y : NULL, i, m);
		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;
		uint32_t f = h5;
		uint32_t g = h6;
		uint32_t h = h7;
//			printf(" ---------- BEF: %x %x %x %x %x %x %x %x\n", a, b, c, d, e, f, g, h);
		for (int i = 0; i < 64; ++i) {
			uint32_t s1 = ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25);
			uint32_t ch = (e & f) ^ ((~e) & g);
			uint32_t temp1 = h + s1 + ch + g_k256[i] + m[i];
			uint32_t s0 = ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp2 = s0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
			if (i == WTF)
			printf(" ---------- IN: %x %x %x %x %x %x %x %x\n", a, b, c, d, e, f, temp1,m[i]);
		}
//			printf(" ---------- AFT: %x %x %x %x %x %x %x %x\n", a, b, c, d, e, f, g, h);
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
	to_bytes(h1, ret + 4);
	to_bytes(h2, ret + 8);
	to_bytes(h3, ret + 12);
	to_bytes(h4, ret + 16);
	to_bytes(h5, ret + 20);
	to_bytes(h6, ret + 24);
	if (type == 256)
		to_bytes(h7, ret + 28);
}

static void		digest(uint64_t *h, uint8_t *ret)
{
	to_bytes(h[0], ret);
	to_bytes(h[1], ret + 4);
	to_bytes(h[2], ret + 8);
	to_bytes(h[3], ret + 12);
	to_bytes(h[4], ret + 16);
	to_bytes(h[5], ret + 20);
	to_bytes(h[6], ret + 24);
	to_bytes(h[7], ret + 28);
}

static void		binop(uint64_t *lul, uint64_t *m)
{
	uint32_t s1;
	uint32_t ch;
	uint32_t temp1;
	uint32_t s0;
	uint32_t maj;
	uint32_t temp2;
	uint32_t tmp[8] = {lul[0], lul[1], lul[2], lul[3], lul[4], lul[5], lul[6], lul[7]};



//		printf(" ---------- BEF: %x %x %x %x %x %x %x %x\n", tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6], tmp[7]);
	for (int i = 0; i < 64; ++i) {
		s1 = ROTR(tmp[4], 6) ^ ROTR(tmp[4], 11) ^ ROTR(tmp[4], 25);
		ch = (tmp[4] & tmp[5]) ^ ((~tmp[4]) & tmp[6]);
		temp1 = tmp[7] + s1 + ch + g_k256[i] + ((uint32_t)m[i]);
		s0 = ROTR(tmp[0], 2) ^ ROTR(tmp[0], 13) ^ ROTR(tmp[0], 22);
		maj = (tmp[0] & tmp[1]) ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]);
		temp2 = s0 + maj;

		tmp[7] = tmp[6];
		tmp[6] = tmp[5];
		tmp[5] = tmp[4];
		tmp[4] = tmp[3] + temp1;
		tmp[3] = tmp[2];
		tmp[2] = tmp[1];
		tmp[1] = tmp[0];
		tmp[0] = temp1 + temp2;
//		if (i == WTF)
//		printf(" ---------- IN: %x %x %x %x %x %x %x %llx\n", tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],temp1,m[i]);
	}
	lul[0] = tmp[0];
	lul[1] = tmp[1];
	lul[2] = tmp[2];
	lul[3] = tmp[3];
	lul[4] = tmp[4];
	lul[5] = tmp[5];
	lul[6] = tmp[6];
	lul[7] = tmp[7];
//		printf(" ---------- AFT: %llx %llx %llx %llx %llx %llx %llx %llx\n", lul[0],lul[1],lul[2],lul[3],lul[4],lul[5],lul[6],lul[7]);
}

static void		declare_chunk2(uint8_t *ck_init, int y, uint64_t *m)
{
	uint32_t		s0;
	uint32_t		s1;
	uint32_t		x;
	uint8_t			*ck;
	uint32_t		w[64];

	ck = ck_init + 64 * y;
	for (int i = 0; i < 64; ++i) {
		if (i < 16) {
			s0 = (ck[i*4] << 24) + (ck[i*4 + 1] << 16) + (ck[i*4 + 2] << 8) + ck[i*4 + 3];
			w[i] = s0;
			m[i] = w[i];
			continue ;
		}
		x = w[i - 15];
		s0 = (ROTR(x, 7)) ^ (ROTR(x, 18)) ^ (SHR(x, 3));
		x = w[i - 2];
		s1 = (ROTR(x, 17)) ^ (ROTR(x, 19)) ^ (SHR(x, 10));
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		m[i] = w[i];
	}
}

static void		init_h(uint64_t *h)
{
	h[0] = 0x6a09e667;
	h[1] = 0xbb67ae85;
	h[2] = 0x3c6ef372;
	h[3] = 0xa54ff53a;
	h[4] = 0x510e527f;
	h[5] = 0x9b05688c;
	h[6] = 0x1f83d9ab;
	h[7] = 0x5be0cd19;
}

void			sha256(t_hash *tab)
{
	t_ops ops;

	ops.name = "sha256";
	ops.endian = false;
	ops.loop = 64;
	ops.message_len = 32;
	ops.encodage_len = 64;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk2;
	ops.binary_operation = binop;
	ops.digest = digest;
	tab->ops = ops;
	launch_hash(tab);
}

void		sha224(t_hash *tab)
{
	int		i;
	int		tmp;
	uint8_t	ret[32];

	i = 0;
	if (!tab->folder)
		print_usage(NULL);
	while (tab->folder[i])
	{
		tmp = pad_message(&(tab->str[i]), false, 64);
		lol(tab->str[i], tmp / 64, ret, 224);
		print_hash(ret, tab, i, 224);
//		free(ret);
		++i;
	}
	del_tab(tab->folder);
	del_tab((char **)tab->str);
}
