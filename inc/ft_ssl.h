/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 17:32:20 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/19 17:35:23 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <stdint.h>
# include <stdbool.h>
# include <vector.h>

typedef struct	s_hash_ops
{
	const char	*name;
	bool		endian;
	int			loop;
	int			message_len;
	int			encodage_len;
	void		(*init_h)(uint64_t *a);
	void		(*declare_chunk)(t_vec *a, int b, uint64_t *c);
	void		(*binary_operation)(uint64_t *a, uint64_t *b);
	void		(*digest)(uint64_t *a, uint8_t *b);
	int			fd;
}				t_ops;

typedef struct	s_hash
{
	int			arg;
	t_vec		folder;
	t_vec		str;
	void		(*f)(struct s_hash *);
	t_ops		ops;
}				t_hash;

# define BUFF_SIZE 12
# define NB_HASH 7
# define P_FLAG 1
# define Q_FLAG 1 << 1
# define R_FLAG 1 << 2
# define S_FLAG 1 << 3
# define D_FLAG 1 << 4
# define E_FLAG 1 << 5
# define I_FLAG 1 << 6
# define O_FLAG 1 << 7
# define ROTL(x, c, size) (((x) << (c)) | ((x) >> (size - (c))))
# define ROTR(x, n, size) ((x >> n) | (x << (size - n)))
# define SHR(x, n) (x >> n)

/*
** All Hash Functions
*/
void			md5(t_hash *a);
void			sha256(t_hash *a);
void			sha224(t_hash *a);
void			sha512(t_hash *a);
void			sha384(t_hash *a);

/*
** All Cipher Functions
*/
void			base64(t_hash *a);
void			base64url(t_hash *a);

/*
** Generic Functions
*/
void			launch_hash(t_hash *a);
void			print_usage(char *a);
int				pad_message(t_vec *a, bool b, int c);
void			print_hash(t_vec *a, t_hash *b, int c, t_ops d);
void			to_bytes32(uint32_t a, uint8_t *b);
void			to_bytes64(uint64_t a, uint8_t *c);

/*
** SHA Functions
*/
void			declare_chunk256(t_vec *a, int b, uint64_t *c);
void			binop256(uint64_t *a, uint64_t *b);
void			declare_chunk512(t_vec *a, int b, uint64_t *c);
void			binop512(uint64_t *a, uint64_t *b);
#endif
