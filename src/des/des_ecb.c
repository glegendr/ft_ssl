/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_ecb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 07:23:21 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/29 17:21:29 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

uint8_t		*get_pwd(void)
{
	char	*cmp;
	char	*ret;

	cmp = ft_strdup(getpass("enter des encryption password"));
	if (ft_strcmp((char *)cmp,
		(ret = getpass("Verifying - enter des encryption password"))))
	{
		write(2, "Verify failure\nbad password read\n", 34);
		free(cmp);
		free(ret);
		exit(1);
	}
	free(ret);
	return ((uint8_t *)cmp);
}

void		create_salt(uint8_t *salt, uint8_t *ops_salt)
{
	int fd;

	if (ops_salt)
	{
		in_u8(ops_salt, salt);
		return ;
	}
	fd = open_file("/dev/random", 0, 0);
	read(fd, salt, 8);
}

void		create_key(uint8_t *pwd, uint8_t *salt, uint8_t *key, uint8_t *iv)
{
	t_hash	concat;
	t_vec	concat_vec;
	t_vec	ret_vec;
	uint8_t	*md5_ret;

	ret_vec = v_new(sizeof(t_vec));
	concat_vec = v_new(sizeof(uint8_t));
	v_append_raw(&concat_vec, pwd, ft_strlen((char *)pwd));
	v_append_raw(&concat_vec, salt, 8);
	v_push(&ret_vec, &concat_vec);
	concat.arg = 0;
	concat.folder = v_new_null(sizeof(uint8_t));
	concat.str = ret_vec;
	concat.ops.fd = 0;
	concat.ops.key = NULL;
	concat.ops.salt = NULL;
	concat.ops.init_vec = NULL;
	md5_ret = md5(&concat, false);
	in_u8(md5_ret, key);
	if (iv)
		in_u8(md5_ret + 8, iv);
	free(md5_ret);
	v_del_all(&ret_vec);
}

uint8_t		*core_des(t_hash *hash, bool print, enum e_des_mode mode, int i)
{
	t_ops		ops;
	uint32_t	divided_key[32];
	uint8_t		final_keys[16][6];
	uint8_t		salt[8];
	uint8_t		key[8];

	while (i < 16)
		ft_bzero(final_keys[i++], 6 * sizeof(uint8_t));
	ops = hash->ops;
	if (!ops.key)
	{
		if (!ops.pwd)
			ops.pwd = get_pwd();
		create_salt(salt, ops.salt);
		create_key(ops.pwd, salt, key, NULL);
		ops.salt = salt;
	}
	else
		in_u8(ops.key, key);
	pc1(key);
	rotate_key(key, divided_key);
	pc2(divided_key, final_keys);
	if (hash->arg & D_FLAG)
		return (unhash_des_message(hash, final_keys, print, mode));
	return (hash_des_message(hash, final_keys, print, mode));
}

uint8_t		*des_ecb(t_hash *hash, bool print)
{
	return (core_des(hash, print, ECB, 0));
}
