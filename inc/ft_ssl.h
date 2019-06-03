#ifndef FT_SSL_H
# define FT_SSL_H

#include <stdint.h>
#include <stdbool.h>
#include <vector.h>

typedef struct	s_hash_ops
{
	const char	*name;
	bool		endian;
	int			loop;
	int			message_len;
	int			encodage_len;
	void		(*init_h)(uint64_t *);
	void		(*declare_chunk)(t_vec *, int, uint64_t *);
	void		(*binary_operation)(uint64_t *, uint64_t *);
	void		(*digest)(uint64_t *, uint8_t *);
}				t_ops;

typedef struct	s_hash
{
	int			arg;
	char		**folder;
	t_vec		str;
	void		(*f)(struct s_hash *);
	t_ops		ops;
}				t_hash;

#define BUFF_SIZE 12
#define NB_HASH 5
#define P_FLAG 1
#define Q_FLAG 1 << 1
#define R_FLAG 1 << 2
#define S_FLAG 1 << 3
#define ROTL(x, c, size) (((x) << (c)) | ((x) >> (size - (c))))
#define ROTR(x, n, size) ((x >> n) | (x << (size - n)))
#define SHR(x, n) (x >> n)

/*
** All Hash Functions
*/
void		md5(t_hash *);
void		sha256(t_hash *);
void		sha224(t_hash *);
void		sha512(t_hash *);
void		sha384(t_hash *);

/*
** Generic Functions
*/
void		launch_hash(t_hash *);
void		print_usage(char *);
int			pad_message(t_vec *, bool, int);
void		print_hash(uint8_t *, t_hash *, int, int);
void		to_bytes32(uint32_t, uint8_t *);
void		to_bytes64(uint64_t, uint8_t *);

/*
** SHA Functions
*/
void		declare_chunk256(t_vec *, int , uint64_t *);
void		binop256(uint64_t *, uint64_t *);
void		declare_chunk512(t_vec *, int , uint64_t *);
void		binop512(uint64_t *, uint64_t *);
#endif
