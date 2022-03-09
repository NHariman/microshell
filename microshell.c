/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   microshell.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nhariman <nhariman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 15:55:57 by nhariman      #+#    #+#                 */
/*   Updated: 2022/03/09 18:09:32 by nhariman      ########   odam.nl         */
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

int		ft_execute(int i, char **av, char **envp, int tmp_fd)
{
	av[i] = NULL;
	close(tmp_fd);
	execve(av[0], av, envp);
	ft_print(2, "error: cannot execute ");
	ft_print(2, av[0]);
	ft_print(2, "\n");
	return (1);
}

int		main(int argc, char **argv, char **envp)
{
	int fd[2];
	int tmp_fd;
	int child_pid;
	int i = 0;

	if (argc == 1)
		return 0;
	tmp_fd = dup(STDIN_FILENO);
	while (argv[i] && argv[i + 1])
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
		else if (argv != &argv[i] && (argv[i] == NULL || !strcmp(argv[i], ";"))) // HAS TO COME BEFORE | CHECK BC NULL
		{
			child_pid = fork();
			if (child_pid < 0)
			{
				ft_print(2, "error: fatal\n");
				exit(-1);
			}
			else if (child_pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				if (ft_execute(i, argv, envp, tmp_fd))
					return (1);
			}
			else
			{
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if (argv != &argv[i] && !strcmp(argv[i], "|"))
		{
			pipe(fd); // pipe fd
			child_pid = fork();
			if (child_pid < 0)
			{
				ft_print(2, "error: fatal\n");
				exit(-1);
			}
			else if (child_pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_execute(i, argv, envp, tmp_fd))
					return (1);
			}
			else
			{
				close(fd[1]);
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(fd[0]);
				close(fd[0]);
			}
		}
	}
	close(tmp_fd);
	return (0);
}
