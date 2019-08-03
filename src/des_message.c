#include "ft_ssl.h"
#include <libft.h>

#include <ft_printf.h>

int const g_ip[64] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7 };

void	ip(uint8_t *str)
{
	int i;
	uint8_t ret[8] = {0};

	i = 0;
	while (i < 64)
	{
		bit(ret, i + 1, str, g_ip[i]);
		++i;
	}
	for (i = 0; i < 8; ++i)
		str[i] = ret[i];
}

uint32_t		u8_to_b32(uint8_t *val)
{
	uint32_t bytes;
	int i;

	i = 0;
	while (i < 4)
	{
		bytes = bytes << 8;
		bytes = bytes | val[i];
		++i;
	}
	return (bytes);
}

int const g_e_bit[48] = {
		32, 1, 2, 3,4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1 };

void		e_bit(uint8_t *ret, uint32_t ln)
{
	int i;
	uint8_t str[4] = {0};

	to_bytes32(ln, str);
	i = 0;
	while (i < 48)
	{
		bit(ret, i + 1, str, g_e_bit[i]);
		++i;
	}
}

int const g_p[32] =
{
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25
};

int const g_sbox[8][4][16] =
{
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};

void		u8_to_u6(uint8_t *six, uint8_t *eight)
{
	int i;
	int y;
	int x;

	six[0] = (eight[0] & 0xfc) >> 2;
	six[1] = ((eight[0] & 0x3) << 4) | ((eight[1] & 0xf0) >> 4);
	six[2] = ((eight[1] & 0xf) << 2) | ((eight[2] & 0xc0) >> 6);
	six[3] = (eight[2] & 0x3f);
	six[4] = (eight[3] & 0xfc) >> 2;
	six[5] = ((eight[3] & 0x3) << 4) | ((eight[4] & 0xf0) >> 4);
	six[6] = ((eight[4] & 0xf) << 2) | ((eight[5] & 0xc0) >> 6);
	six[7] = (eight[5] & 0x3f);
	i = 0;
	while (i < 8)
	{
		y = ((six[i] & 0x20) >> 4) | (six[i] & 0x1);
		x = (six[i] & 0x1e) >> 1;
		six[i] = g_sbox[i][y][x];
		++i;
	}
}

uint32_t	p(uint8_t *six)
{
	uint32_t start;
	uint32_t ret;
	int i;

	start = 0;
	ret = 0;
	i = 0;
	while (i < 8)
	{
		start = start << 4;
		start = start | (six[i] & 0xf);
		++i;
	}
	i = 0;
	while (i < 32)
	{
		ret = ret << 1;
		ret = ret | ((start >> (32 - g_p[i])) & 0x1);
		++i;
	}
	return (ret);
}

uint32_t	f(uint32_t ln, uint8_t *key)
{
	uint8_t tmp[6] = {0};
	uint8_t six_bit[8] = {0};
	int i;

	i = 0;
	e_bit(tmp, ln);
	while (i < 6)
	{
		tmp[i] = tmp[i] ^ key[i];
		++i;
	}
	u8_to_u6(six_bit, tmp);
	return (p(six_bit));
}

int const g_ip1[64] =
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

uint64_t	divide_message(uint8_t *str, uint8_t fin_keys[16][6], bool rev)
{
	uint32_t ln;
	uint32_t rn;
	uint64_t tmp;
	uint64_t end;
	int i;

	ln = u8_to_b32(str);
	rn = u8_to_b32(str + 4);
	i = 0;
	while (i < 16)
	{
		tmp = ln;
		ln = rn;
		rn = tmp ^ f(ln, fin_keys[rev ? 15 - i : i]);
		++i;
	}

	tmp = rn;
	tmp = (tmp << 32) | ln;
	end = 0;
	i = 0;
	while (i < 64)
		end = (end << 1) | ((tmp >> (64 - g_ip1[i++])) & 0x1);
	return (end);
}

void		pkcs5_pad(t_vec *vec)
{
	int len;
	int i;

	i = 0;
	len = 8 - (v_size(vec) % 8);
	while (i < len)
	{
		v_push_int(vec, len);
		++i;
	}
}

uint8_t		*print_des_message(t_hash *hash, t_vec *print, bool rev, bool bp)
{
	if (hash->arg & A_FLAG && !rev)
	{
		t_hash tmp;
		t_vec vec;

		vec = v_new(sizeof(t_vec));
		v_push(&vec, print);
		tmp = *hash;
		tmp.str = vec;
		tmp.arg |= Q_FLAG;
		return (base64(&tmp, bp));
	}
	else if (!bp)
		return (v_raw(print));
	else
		write(hash->ops.fd, v_raw(print), v_size(print));
//	write(1, "\n", 1);
	return (NULL);
}

void		xor_message(uint8_t *str, uint8_t *xor, enum des_mode mode, int y)
{
	uint8_t *tmp;

	if (mode == PCBC && y != 0)
	{
		tmp = str + 8 * (y - 1);
		for (int i = 0; i < 8; ++i)
			xor[i] ^= tmp[i];
	}
	tmp = str + 8 * y;
	if (mode == ECB)
		in_u8(tmp, xor);
	else
		for (int i = 0; i < 8; ++i)
			xor[i] ^= tmp[i];
}

uint8_t		*hash_des_message(t_hash *hash, uint8_t div_key[16][6], bool bp, enum des_mode mode)
{
	t_ops ops;
	int i;
	uint64_t div_ret;
	uint8_t *str;
	t_vec print;
	uint8_t xor[8] = {0};

	print = v_new(sizeof(uint8_t));
	ops = hash->ops;
	if (mode != ECB)
		in_u8(ops.init_vec, xor);
	i = 0;
	if (ops.salt)
	{
		v_append_raw(&print, "Salted__", 8);
		v_append_raw(&print, ops.salt, 8);
	}
	while (i < v_size(&hash->str))
	{
		pkcs5_pad(v_get(&hash->str, i));
		str = v_raw(v_get(&hash->str, i));
		for (int y = 0; y < v_size(v_get(&hash->str, i)) / 8; ++y)
		{
			xor_message(str, xor, mode, y);
			ip(xor);
			div_ret = divide_message(xor, div_key, false);
			to_bytes64(div_ret, xor);
			for (int z = 0; z < 8; ++z)
				v_push_int(&print, ((div_ret >> (64 - ( 8 * (z + 1)))) & 0xff));
		}
		if (!bp)
			return (print_des_message(hash, &print, false, bp));
		print_des_message(hash, &print, false, bp);
		v_reset(&print);
		++i;
	}
	if (ops.key)
		free(ops.key);
	if (ops.salt)
		free(ops.salt);
	if (ops.init_vec)
		free(ops.init_vec);
	if (ops.pwd)
		free(ops.pwd);
	v_del(&print);
	v_del_all(&hash->folder);
	v_del_all(&hash->str);
	return (NULL);
}

uint64_t	xor_message64(uint64_t mess, uint8_t *xor, enum des_mode mode, uint8_t *str, int y)
{
	uint64_t ret;
	uint64_t pcbc;

	pcbc = 0;
	ret = 0;
	if (mode == ECB)
		return (mess);
	for (int i = 0; i < 8; ++i)
	{
		ret = ret << 8;
		ret |= str[i];
	}
	if (mode == PCBC && y != 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			pcbc = pcbc << 8;
			pcbc |= xor[i];
		}
		ret ^= pcbc;
	}
	return (ret ^ mess);
}

uint8_t		*unhash_des_message(t_hash *hash, uint8_t div_key[16][6], bool bp, enum des_mode mode)
{
	uint64_t mess;
	t_hash tmp;
	t_vec vec;
	uint8_t *str;
	uint8_t xor[8] = {0};
	t_vec print;
	int i;

	i = 0;
	if (hash->ops.init_vec)
		in_u8(hash->ops.init_vec, xor);
	print = v_new(sizeof(uint8_t));
	while (i < v_size(&hash->str))
	{
		if (hash->arg & A_FLAG)
		{
			vec = v_new(sizeof(t_vec));
			v_push(&vec, v_get(&hash->str, i));
			tmp = *hash;
			tmp.str = vec;
			tmp.arg |= D_FLAG;
			str = base64(&tmp, false);
		}
		else
			str = v_raw(v_get(&hash->str, i));
		if (!hash->ops.key)
			str += 16;
		for (unsigned long int y = 0; y < (ft_strlen((char *)str)) / 8; ++y)
		{
			uint8_t tmp[8] = {0};

			in_u8(str + 8 * y, tmp);
			ip(tmp);
			mess = divide_message(tmp, div_key, true);
			mess = xor_message64(mess, xor, mode, y == 0 ? xor : str + 8 * (y - 1), y);
			to_bytes64(mess, xor);
			for (int z = 0; z < 8; ++z)
				v_push_int(&print, ((mess >> (56 - (8 * z))) & 0xff));
		}
		if (!bp)
			return (print_des_message(hash, &print, true, bp));
		print_des_message(hash, &print, true, bp);
		v_reset(&print);
		++i;
	}
	if (hash->ops.key)
		free(hash->ops.key);
	if (hash->ops.salt)
		free(hash->ops.salt);
	if (hash->ops.init_vec)
		free(hash->ops.init_vec);
//	if (hash->ops.pwd)
//		free(hash->ops.pwd);
	v_del(&print);
	v_del_all(&hash->folder);
	v_del_all(&hash->str);
	while (42);
	return (NULL);
}
