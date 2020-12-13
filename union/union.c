/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   union.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: exam <marvin@codam.nl>                       +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/18 15:51:53 by exam          #+#    #+#                 */
/*   Updated: 2020/02/18 16:00:14 by exam          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		main(int argc, char **argv)
{
	char *str;
	char *str2;
	int	i;
	int	j;
	char array[255];

	i = 0;
	j = 0;
	if (argc == 3)
	{
		str = argv[1];
		str2 = argv[2];
		while (j < 255)
		{
			array[j] = 0;
			j++;
		}
		while (str[i])
		{
			if (array[(int)str[i]] == 0)
			{
				array[(int)str[i]] = 1;
				write(1, &str[i], 1);
			}
			i++;
		}
		i = 0;
		while (str2[i])
		{
			if (array[(int)str2[i]] == 0)
			{
				array[(int)str2[i]] = 1;
				write(1, &str2[i], 1);
			}
			i++;
		}
	}
	write(1, "\n", 1);
}
