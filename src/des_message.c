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
	uint8_t ret[8];

	for (i = 0; i < 8; ++i)
		ft_printf("%0.8b ", str[i]);
	printf("\n");
	i = 0;
	while (i < 64)
	{
		bit(ret, i, str, g_ip[i]);
		++i;
	}
	for (i = 0; i < 8; ++i)
		ft_printf("%0.8b ", ret[i]);
	printf("\n");
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

void		divide_message(uint8_t *str, uint32_t *div_key)
{
	uint32_t l0;
	uint32_t r0;
	uint32_t ln;
	uint32_t rn;
	int i;

	l0 = u8_to_b32(str);
	r0 = u8_to_b32(str + 4);
	ln = r0;
	rn = 0;
	i = 0;
	while (i < 16)
	{
		
		++i;
	}
}

void		hash_des_message(uint8_t *str, uint32_t *div_key)
{
	(void)div_key;
	ip(str);
	divide_message(str, div_key);
}
