/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 11:32:37 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 11:39:35 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <fcntl.h>
#include <libft.h>

void		a_fct(t_hash *tab)
{
	tab->arg |= A_FLAG;
}

void		o_flag(t_hash *tab, char *argv)
{
	int fd;

	fd = open_file(argv, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH);
	if (fd == -1)
		print_usage(NULL);
	tab->ops.fd = fd;
	tab->arg ^= O_FLAG;
}

void		s_flag(t_hash *tab, char *str)
{
	if (tab->f != des_ecb)
	{
		into_vec(&tab->folder, str);
		into_vec(&tab->str, str);
		tab->arg ^= S_FLAG;
		return ;
	}
	hex_flag(tab, str, S_FLAG);
}

static void	hex_to_u8(char *str, int len, uint8_t *key, int i)
{
	int in;

	while (i < 16)
	{
		if (i < len)
		{
			in = str[i];
			if (in >= 'a' && in <= 'f')
				in -= 'a' - 10;
			else if (in >= 'A' && in <= 'F')
				in -= 'A' - 10;
			else if (in >= '0' && in <= '9')
				in -= '0';
			else
			{
				write(2, "non-hex digit\n", 14);
				exit(1);
			}
		}
		else
			in = 0;
		key[i / 2] = key[i / 2] << 4;
		key[i / 2] = key[i / 2] | in;
		++i;
	}
}

void		hex_flag(t_hash *hash, char *str, int flag)
{
	uint8_t	*key;
	int		len;
	int		i;

	if (!(key = malloc(sizeof(uint8_t) * 8)))
		return ;
	i = 0;
	len = ft_strlen(str);
	hex_to_u8(str, len, key, 0);
	if (flag == K_FLAG)
		hash->ops.key = key;
	else if (flag == V_FLAG)
		hash->ops.init_vec = key;
	else
		hash->ops.salt = key;
	hash->arg ^= flag;
}
