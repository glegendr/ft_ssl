#ifndef FT_SSL_H
# define FT_SSL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct	s_hash
{
	int			arg;
	void		(*f)(struct s_hash *);
	char		**folder;
	uint8_t		**str;
}				t_hash;

#define BUFF_SIZE 12
#define NB_HASH 5
#define P_FLAG 1
#define Q_FLAG 1 << 1
#define R_FLAG 1 << 2
#define S_FLAG 1 << 3

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
void		print_usage(char *);
int			pad_message(uint8_t **, bool, int);
void		print_hash(uint8_t *, t_hash *, int, int);
#endif
