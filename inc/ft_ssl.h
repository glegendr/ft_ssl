/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 17:32:20 by glegendr          #+#    #+#             */
/*   Updated: 2019/09/24 18:18:50 by glegendr         ###   ########.fr       */
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
	uint8_t		*pwd;
	uint8_t		*salt;
	uint8_t		*init_vec;
	uint8_t		*key;
	bool		endian;
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
	uint8_t		*(*f)(struct s_hash *, bool);
	t_ops		ops;
}				t_hash;

typedef uint8_t		*(*t_fct)(t_hash *, bool);
typedef void		(*t_flag)(t_hash *);
# define BUFF_SIZE 12
# define NB_HASH 11
# define NB_FLAGS 12
# define P_FLAG 1
# define Q_FLAG 1 << 1
# define R_FLAG 1 << 2
# define S_FLAG 1 << 3
# define D_FLAG 1 << 4
# define E_FLAG 1 << 5
# define I_FLAG 1 << 6
# define O_FLAG 1 << 7
# define A_FLAG 1 << 8
# define K_FLAG 1 << 9
# define V_FLAG 1 << 10
# define ROTL(x, c, size) (((x) << (c)) | ((x) >> (size - (c))))
# define ROTR(x, n, size) ((x >> n) | (x << (size - n)))
# define SHR(x, n) (x >> n)
# define DEFKEY(a) (ROTL((i % 16 == 0) ? a : div_key[i - 1], x, 28) & 0xfffffff)
# define CLEAN_AND_PRINT(a, b) { clean_des(a, NULL); print_usage(b); }
# define IS_DES(f) (f == des_ecb || f == des_cbc || f == des_pcbc)

/*
** All Hash Functions
*/
uint8_t			*md5(t_hash *a, bool print);
uint8_t			*sha256(t_hash *a, bool print);
uint8_t			*sha224(t_hash *a, bool print);
uint8_t			*sha512(t_hash *a, bool print);
uint8_t			*sha384(t_hash *a, bool print);

/*
** All Cipher Functions
*/
uint8_t			*base64(t_hash *a, bool print);
uint8_t			*base64url(t_hash *a, bool print);
uint8_t			*des_ecb(t_hash *a, bool print);
uint8_t			*des_cbc(t_hash *a, bool print);
uint8_t			*des_pcbc(t_hash *a, bool print);

/*
** Generic Functions
*/
uint8_t			*launch_hash(t_hash *a, bool print);
void			print_usage(char *a);
int				pad_message(t_vec *a, bool b, int c);
void			print_hash(t_vec *a, t_hash *b, int c, t_ops d);
void			to_bytes32(uint32_t a, uint8_t *b);
void			to_bytes64(uint64_t a, uint8_t *c);
void			to_bytes32_endian(uint32_t a, uint8_t *b);
void			bit(uint8_t *ret, int i, uint8_t *str, int pos);
char			*get_usage(void);
void			get_help(t_hash *tab);
char			*get_all_hash(void);
void			clean_des(t_hash *hash, t_vec *print);

/*
** MD5 Functions
*/
uint32_t		make_md5_pseudo_random(int a, uint32_t *b, uint32_t *c);
void			instance_tab(uint32_t *a, uint64_t *b);

/*
** SHA Functions
*/
void			declare_chunk256(t_vec *a, int b, uint64_t *c);
void			binop256(uint64_t *a, uint64_t *b);
void			make_256_pseudo_random(uint32_t *a, uint64_t *b,
										int c, uint32_t d);
void			declare_chunk512(t_vec *a, int b, uint64_t *c);
void			make_512_pseudo_random(uint64_t *a, uint64_t *b,
										int c, uint64_t d);
void			binop512(uint64_t *a, uint64_t *b);
uint64_t		init_s0(uint8_t *ck, int i);
void			in_u8(uint8_t *in, uint8_t *cpy);

/*
** Bases Functions
*/
uint8_t			*bases(t_hash *a, char *b, int c, bool print);
int				get_index(uint8_t *a, char *b, uint8_t *c, int *d);
int				append_error(t_vec *a);
void			encript_base(uint8_t *a, char *b, int c, t_vec *d);
void			generate_base(char *a, bool b);

/*
** Parse Functions
*/
void			parse_argv(int argc, char **argv);
int				read_file(t_hash *tab, int fd, bool print);
int				open_file(char *argv, int flag, int perm);
void			into_vec(t_vec *to_push, char *str);
t_fct			get_hash_fct(char *name);
t_flag			get_flag_fct(char flag);
void			get_help(t_hash *tab);

/*
** Flag Functions
*/
void			p_fct(t_hash *hash);
void			q_fct(t_hash *hash);
void			r_fct(t_hash *hash);
void			s_fct(t_hash *hash);
void			d_fct(t_hash *hash);
void			e_fct(t_hash *hash);
void			i_fct(t_hash *hash);
void			o_fct(t_hash *hash);
void			k_fct(t_hash *hash);
void			v_fct(t_hash *hash);
void			a_fct(t_hash *hash);
void			s_flag(t_hash *tab, char *str);
void			o_flag(t_hash *tab, char *str);
void			hex_flag(t_hash *tab, char *str, int flag);
#endif
