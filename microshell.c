/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   microshell.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nhariman <nhariman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 15:55:57 by nhariman      #+#    #+#                 */
/*   Updated: 2022/03/08 21:57:34 by nhariman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/wait.h>

// create a write to stderr function

void	ft_print(int fd, char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	write(fd, str, i);
}

int		ft_execute(int i, char **av, char **envp)
{
	execve(av[0], av, envp);
	ft_print(2, "error: cannot execute ");
	ft_print(2, av[0]);
	ft_print(2, "\n");
	return (1);
}

int		main(int argc, char **argv, char **envp)
{
	int fd[2];
	int og_fd;
	int i = 0;

	if (argc == 1)
		return 0;
	og_fd = dup(0);
	while (argv[i] != NULL)
	{
		argv = &argv[i + 1];
		i = 0;
		// gather all arguments until ; or |
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		//if cd
		if (!strcmp(argv[0], "cd"))
		{
			if (i != 2)
				ft_print(2, "error: cd: bad arguments\n");
			else if (chdir(argv[1]) != 0)
			{
				ft_print(2, "error: cd: cannot change directory to ");
				ft_print(2, argv[1]);
				ft_print(2, "\n");
			}
		}
		else if (argv[i] && !strcmp(argv[i], "|"))
		{
			ft_print(1, "pipes here\n");
		}
		else
		{
			int child_pid;
			int	child_stat;

			child_pid = fork();
			av[i] = NULL;
			if (child_pid < 0)
			{
				ft_print(2, "error: fatal\n");
				exit(1);
			}
			else if (child_pid == 0)
				ft_execute(i, argv, envp);
			else
			{
				waitpid(-1, NULL, WUNTRACED);
			}
			ft_print(1, "semicolon or regular execute here\n");
		}
		// for(int j = 0; j < i; j++)
		// {
		// 	ft_print(1, argv[j]);
		// 	ft_print(1, "\n");
		// }
		// ft_print(1,"end loop\n");

	}
	return (0);
}
