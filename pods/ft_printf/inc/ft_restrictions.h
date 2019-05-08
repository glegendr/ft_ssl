/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_restrictions.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/11 21:29:06 by glegendr          #+#    #+#             */
/*   Updated: 2018/01/26 20:40:06 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RESTRICTIONS_H
# define FT_RESTRICTIONS_H
# define PRE 1<<0
# define ZERO 1<<1
# define PLUS 1<<2
# define SPACE 1<<3
# define SHARP 1<<4
# define STRING 1<<5
# define POINT 1<<6
# define DEC 1<<7
# define OCT 1<<8
# define UNSIGNED 1<<9
# define HEXA 1<<10
# define HEXAMAJ 1<<11
# define CHAR 1<<12
# define HH 1<<13
# define H 1<<14
# define L 1<<15
# define LL 1<<16
# define J 1<<17
# define Z 1<<18
# define PC 1<<19
# define BASICS (PRE | ZERO | PLUS | SPACE | SHARP)
# define CONV (HH | H | L | LL | J | Z)
#endif
