/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 12:06:00 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:10:45 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <libft.h>

void		print_hash(t_vec *ret, t_hash *hash, int i, t_ops ops)
{
	t_vec vec;
	t_vec *folder;

	vec = v_new(sizeof(char));
	if ((folder = v_get(&hash->folder, i)) && !v_raw(folder))
		v_append_raw(&vec, v_raw(ret), v_size(ret));
	else
	{
		if (!(hash->arg & R_FLAG) && !(hash->arg & Q_FLAG))
		{
			v_append_raw(&vec, (void *)ops.name, ft_strlen(ops.name));
			v_push_int(&vec, '(');
			v_append_raw(&vec, v_raw(folder), v_size(folder));
			v_append_raw(&vec, ")= ", 3);
		}
		v_append_raw(&vec, v_raw(ret), v_size(ret));
		if ((hash->arg & R_FLAG) && !(hash->arg & Q_FLAG))
		{
			v_push_int(&vec, ' ');
			v_append_raw(&vec, v_raw(folder), v_size(folder));
		}
	}
	v_push_int(&vec, '\n');
	write(ops.fd, v_raw(&vec), v_size(&vec));
	v_del(&vec);
}

void		print_usage(char *name)
{
	t_vec		vec;
	char		*s;

	if (!name)
		write(2, "usage: ft_ssl command [command opts] [command args]\n"
				"ft_ssl -h for more help\n", 76);
	else
	{
		vec = v_new(sizeof(char));
		s = "ft_ssl: Error: '";
		v_append_raw(&vec, s, ft_strlen(s));
		v_append_raw(&vec, name, ft_strlen(name));
		s = "' is an invalid command\n"
			"Usage: ft_ssl hash_fct [-pqrde] [-s string] [-i in_file]"
			" [-o out_file]\nAllowed hash_fct are:\n";
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_all_hash();
		v_append_raw(&vec, s, ft_strlen(s));
		free(s);
		s = get_usage();
		v_append_raw(&vec, s, ft_strlen(s));
		write(2, (char *)v_raw(&vec), v_size(&vec));
		v_del(&vec);
	}
	exit(1);
}
