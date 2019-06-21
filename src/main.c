/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 14:48:56 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/21 13:48:42 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

void			read_file(t_hash *tab, int fd, bool print)
{
	t_vec	vec;
	char	ret[BUFF_SIZE];
	int		val;

	if (fd == -1)
		return ;
	vec = v_new(sizeof(char));
	while ((val = read(fd, ret, BUFF_SIZE)) > 0)
		v_append_raw(&vec, ret, val);
	if (print == true)
		write(1, v_raw(&vec), v_size(&vec));
	if (fd == 0)
		v_push_first(&tab->str, &vec);
	else
		v_push(&tab->str, &vec);
}

int				open_file(char *argv, int flag, int perm)
{
	int		fd;
	int		o_flag;
	int		s_perm;

	o_flag = O_RDONLY | flag;
	s_perm = 0 | perm;
	if ((fd = open(argv, o_flag, s_perm)) == -1)
	{
		if (perm)
			printf("ft_ssl: %s: Permition denied\n", argv);
		else
			printf("ft_ssl: %s: No such file or directory\n", argv);
		return (fd);
	}
	return (fd);
}

void			into_vec(t_vec *to_push, char *str)
{
	t_vec vec;

	if (str)
	{
		vec = v_new(sizeof(char));
		v_append_raw(&vec, str, ft_strlen(str));
		v_push(to_push, &vec);
	}
	else
	{
		vec = v_new_null(sizeof(char));
		v_push_first(to_push, &vec);
	}
}

void			o_flag(t_hash *tab, char *argv)
{
	int fd;

	fd = open_file(argv, O_RDWR | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IWGRP | S_IROTH);
	if (fd == -1)
		print_usage(NULL);
	tab->ops.fd = fd;
	tab->arg ^= O_FLAG;
}

int				main(int argc, char *argv[])
{
	parse_argv(argc, argv);
	return (0);
}
