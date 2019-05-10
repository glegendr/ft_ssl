/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 14:48:56 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/10 19:11:36 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <unistd.h>
#include <ft_printf.h>
#include "get_next_line.h"
#include <stdio.h>
#include "const.h"
#include <stdint.h>

#define NB_HASH 2
#define P_FLAG 1
#define Q_FLAG 1 << 1
#define R_FLAG 1 << 2
#define S_FLAG 1 << 3

typedef struct	s_hash
{
	int	arg;
	void	(*f)(struct s_hash);
	uint8_t	**str;
}		t_hash;

void		md5(t_hash tab);
void		sha256(t_hash tab);
void		print_usage(char *name);

char *const g_tab[NB_HASH] = {"md5", "sha256"};
void (*const g_hash_fct[NB_HASH])(t_hash) = {md5, sha256};


void		push_original_len(uint8_t **str, int len, uint64_t original_len)
{

	printf("ori_len : %llu\n", original_len);
	(*str)[len + 0] = ((original_len >> 56) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 56) & 0xff));
	(*str)[len + 1] = ((original_len >> 48) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 48) & 0xff));
	(*str)[len + 2] = ((original_len >> 40) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 40) & 0xff));
	(*str)[len + 3] = ((original_len >> 32) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 32) & 0xff));
	(*str)[len + 4] = ((original_len >> 24) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 24) & 0xff));
	(*str)[len + 5] = ((original_len >> 16) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 16) & 0xff));
	(*str)[len + 6] = ((original_len >> 8) & 0xff);
	ft_printf("ori_len : %0.8b\n",((original_len >> 8) & 0xff));
	(*str)[len + 7] = ((original_len & 0xff));
	ft_printf("ori_len : %0.8b\n", (original_len & 0xff));
}

uint32_t to_int32(const uint8_t *bytes)
{
	return (uint32_t) bytes[0]
	| ((uint32_t) bytes[1] << 8)
	| ((uint32_t) bytes[2] << 16)
	| ((uint32_t) bytes[3] << 24);
}

int		pad_message(uint8_t **tab)
{
	uint8_t *str;
	int len;
	int original_len;

	if (*tab == NULL)
		original_len = 0;
	else
		original_len = ft_strlen((char *)(*tab));
	len = original_len + 1;
	while (len % 64)
		++len;
	*tab = ft_realloc(*tab, original_len, len);
	str = *tab;
	str[original_len] = (char)128;
	len = 1;
	while ((original_len + len) % 56)
	{
		str[original_len + len] = (char)0;
		++len;
	}
	push_original_len(&str, original_len + len, (uint64_t)original_len);
	*tab = str;
	return (original_len + len + 8);
}


uint32_t	declare_chunk(uint8_t *ck, int i)
{
	int32_t val;
	val = (ck[i + 3] << 24) + (ck[i + 2] << 16) + (ck[i + 1] << 8) + ck[i + 0];
	ft_printf("1: %0.8b 2: %0.8b 3: %0.8b 4: %0.8b final: %0.32b\n", ck[i + 3], ck[i + 2], ck[i + 1], ck[i + 0], val);
//	ft_printf("1: %.3c 2: %.3c 3: %.3c 4: %.3c final: %0.32b\n", ck[i + 0], ck[i + 1], ck[i + 2], ck[i + 3], val);
	return (val);
}

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void		lol(uint8_t *ck)
{
	int32_t a0 = 0x67452301;
	int32_t b0 = 0xefcdab89;
	int32_t c0 = 0x98badcfe;
	int32_t d0 = 0x10325476;
	int32_t M[16];

//	for (int y = 0; y < 4; y++) { // FOR 512 MESSAGE
	for (int i = 0; i < 16; i++)
	{
		M[i] = declare_chunk(ck, i * 4);
//		M[i] = to_int32(ck + i * 4);
//		ft_printf("%0.32b\n", M[i]);
	}
	printf("\n");
	int32_t A = a0;
	int32_t B = b0;
	int32_t C = c0;
	int32_t D = d0;

	for (int i = 0; i < 64; i++) {
		int32_t F, g;
		if (i < 16) {
			F = (B & C) | ((~B) & D);
			g = i;
		} else if (i < 32) {
			F = (D & B) | ((~D) & C);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			F = B ^ C ^ D;
			g = (3 * i + 5) % 16;
		} else {
			F = C ^ (B | (~D));
			g = (7 * i) % 16;
		}
	//Be wary of the below definitions of a,b,c,d
	
//		F = F + A + g_k[i] + M[g];
//		A = D;
//		D = C;
//		C = B;
//		B = B + ((F << g_s[i]) | (F >> (32 - g_s[i])));
		uint32_t temp = D;
		D = C;
		C = B;
		B = B + LEFTROTATE((A + F + g_k[i] + M[g]), g_s[i]);
		A = temp;
	}
	//Add this chunk's hash to result so far:
	a0 = a0 + A;
	printf("%0.8x\n", a0);
	b0 = b0 + B;
	printf("%0.8x\n", b0);
	c0 = c0 + C;
	printf("%0.8x\n", c0);
	d0 = d0 + D;
	printf("%0.8x\n", d0);
//		}
	uint8_t digest[16];
	digest[0] = (a0 & 0xff);
	digest[1] = ((a0 >> 8) & 0xff);
	digest[2] = ((a0 >> 16) & 0xff);
	digest[3] = ((a0 >> 24) & 0xff);
	digest[4] = (b0 & 0xff);
	digest[5] = ((b0 >> 8) & 0xff);
	digest[6] = ((b0 >> 16) & 0xff);
	digest[7] = ((b0 >> 24) & 0xff);
	digest[8] = (c0 & 0xff);
	digest[9] = ((c0 >> 8) & 0xff);
	digest[10] = ((c0 >> 16) & 0xff);
	digest[11] = ((c0 >> 24) & 0xff);
	digest[12] = (d0 & 0xff);
	digest[13] = ((d0 >> 8) & 0xff);
	digest[14] = ((d0 >> 16) & 0xff);
	digest[15] = ((d0 >> 24) & 0xff);
	printf("MD5(moi)= ");
	for (int i = 0; i < 16; i++)
		printf("%0.2x", digest[i]);
	printf("\n");
}

/*
unsigned char		*ft_strrev(unsigned char *mess)
{
	unsigned char *ret = malloc(8 * ft_strlen((char *)mess));
	int y = ft_strlen((char *)mess);

	for (int i = 0; mess[i]; i++)
	{
		ret[y - 1] = mess[i];
		y--;
	}
	return ret;
}*/

void		md5(t_hash tab)
{
	int i;

	if (!tab.str)
	{
		tab.str = malloc(sizeof(uint8_t **) * 2);
		tab.str[0] = NULL;
		int tmp = pad_message(&tab.str[0]);
		for (int y = 0; y < tmp; y++) //                  PRINT
			ft_printf("%0.8b ", tab.str[0][y]);
		printf("\n");
		// Foreach(block de 512 bits)
		lol(tab.str[0]);
		return ;
	}
	i = 0;
	while (tab.str[i])
	{
		int tmp = pad_message(&tab.str[i]);
		for (int y = 0; y < tmp; y++) //                  PRINT
			ft_printf("%0.8b ", tab.str[i][y]);
		printf("\n");
		// Foreach(block de 512 bits)
		lol(tab.str[i]);
		++i;
	}
}

void		sha256(t_hash tab)
{
	int i;

	if (!tab.str)
		print_usage(NULL);
	i = 0;
	while (tab.str[i])
		ft_printf("sha256: %s -- %b\n", tab.str[i++], tab.arg);
}

void		(* get_hash_fct(char *name))(t_hash)
{
	int id;

	id = 0;
	while (id < NB_HASH)
	{
		if (!ft_strcmp(g_tab[id], name))
			break ;
		++id;
	}
	if (id == NB_HASH)
		print_usage(name);
	return (g_hash_fct[id]);
}

char		*get_all_hash(void)
{
	t_vec vec;
	char *ret;
	int i;

	i = 0;
	vec = v_new(sizeof(char));
	while (i < NB_HASH)
	{
		v_append_raw(&vec, g_tab[i], ft_strlen(g_tab[i]));
		v_push_int(&vec, '\n');
		++i;
	}
	v_push_int(&vec, '\0');
	ret = ft_strdup(v_raw(&vec));
	v_del(&vec);
	return (ret);
}

void		print_usage(char *name)
{
	t_vec vec;
	char *s;

	if (!name)
		write(2,
		"usage: ft_ssl command [command opts] [command args]\n", 51);
	else
	{
		vec = v_new(sizeof(char));
		s = "ft_ssl: Error: '";
		v_append_raw(&vec, s, ft_strlen(s));
		v_append_raw(&vec, name, ft_strlen(name));
		s = "' is an invalid command\nallowed commands are:\n";
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_all_hash();
		v_append_raw(&vec, s, ft_strlen(s));
		write(2, (char *)v_raw(&vec), v_size(&vec));
		v_del(&vec);
	}
	exit(1);
}

int		open_folder(char *flag, uint8_t ***tab)
{
	int fd;
	char *ret;

	if ((fd = open(flag, O_RDONLY)) == -1)
		return (1);
	get_next_line(fd, &ret);
	if (ret == NULL)
		*tab = NULL;
	else
		*tab = (uint8_t **)tab_join((char **)(*tab), ret);
	free(ret);
	return (0);
}

int		match_flag(char *flag, int *arg, uint8_t ***tab)
{
	int i;

	if (!flag || !flag[1])
		return (1);
	else if (flag[0] != '-')
		return (open_folder(flag, tab));
	i = 1;
	while (flag[i])
	{
		if (flag[i] == 'p')
			*arg |= P_FLAG;
		else if (flag[i] == 'q')
			*arg |= Q_FLAG;
		else if (flag[i] == 'r')
			*arg |= R_FLAG;
		else if (flag[i] == 's')
			*arg |= S_FLAG;
		else
			return (1);
		++i;
	}
	return (0);
}

t_hash		parse_argv(int argc, char *argv[])
{
	t_hash tab;
	int i;

	if (argc < 2)
		print_usage(NULL);
	tab.f = get_hash_fct(argv[1]);
	tab.str = NULL;
	tab.arg = 0;
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab.arg, &tab.str))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) != 0)
		{
			if (++i >= argc)
				print_usage(NULL);
			tab.str = (uint8_t **)tab_join((char **)tab.str, argv[i]);
			tab.arg ^= S_FLAG;
		}
		++i;
	}
	return (tab);
}

#include <fcntl.h>

int		main(int argc, char *argv[])
{
	t_hash tab;

//	printf("%i -- %s\n", get_next_line(0, &ret), ret);

	tab = parse_argv(argc, argv);
	tab.f(tab);
	return (0);
}
