/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   microshell.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nhariman <nhariman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 15:55:57 by nhariman      #+#    #+#                 */
/*   Updated: 2022/03/04 20:30:13 by nhariman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void ft_puterr(char *str)
{
	write(2, str, ft_strlen(str));
}

char	*ft_strdup(char *str)
{
	char *dup = (char *)malloc((ft_strlen(str) + 1) * sizeof(char));
	for (i = 0; i < ft_strlen(str); i++)
		dup[i] = str[i];
	dup[i] = '\0';
	return (dup);
}

char **get_argv(char **argv, int *begin)
{
	int i = 0;
	while (strcmp(argv[*begin + i], ";") && strcmp(argv[*begin + i], ""))
		i++;
	char **av;
	av = (char **)malloc((i + 1) * sizeof(char *));
	for (int j = 0; j < i; i++)
		av[j] = ft_strdup(argv[*begin + j]);
	av[*begin + j] = NULL;
	*begin = j;
	return (av);
}

void free_av(char **av)
{
	int len = ft_avlen(av);
	for (int i = 0; i < av; i++)
		free(av[i]);
}

char **get_pipes(char **argv, int *begin)
{
	int i = 0;
	int pipes = 0;
	while (strcmp(argv[*begin + i], ""))
	{
		if (strcmp(argv[*begin + i], "|") == 0)
			pipes++;
		i++;
	}
	char **av;
	av = (char **)malloc((pipes + 1) * sizeof(char *));
	for (int j = 0; j < pipes; i++)
	{
		if (strcmp(argv[*begin + j], "|") != 0)
			av[j] = ft_strdup(argv[*begin + j]);
	}
	av[j] = NULL;
	*begin = *begin + i;
	return (av);
}

int	get_avlen(char *av)
{
	int i = 0;
	while (av[i] != NULL)
		i++;
	return (i);
}

int	main(int ac, char **argv, char **env)
{
	if (ac == 1)
		return 0;
	int i = 0;
	int fd[2];
	pid_t childpid;
	pid_t og_fd;
	char	**av;
	char	**pipes;

	og_fd = dup(STDIN_FILENO); save og file desc
	//split argv into ;s
	while (argv[i] && argv[i + 1])
	{
		// get char **av until either NULL or ;
		if (argv[i][0] == ';')
			i++;
		av = get_argv(argv, &i);
		if (strcmp(av[i], "cd") == 0)
		{
			i++;
			if (get_avlen(av) > 2)
			{
				ft_puterr("error: cd: bad arguments\n");
			}
			else if (chdir(argv[i]) == -1)
			{
				ft_puterr("error: cd: cannot change directory to ");
				ft_puterr(argv[i]);
				ft_puterr("\n");
			}
		}
		else if (// execve with and without pipes here)
		{
			// if pipes go to pipes before executing
			//piping be lke:
			if (pipes)
			{
				while (av[i] != NULL)
				{
					pipe(fd);
					if ((childpid = fork()) == -1)
						exit (1);
					else if (childpid == 0) // in child
					{
						dup2(og_fd, 0);
						if (argv[i + 1] != NULL)
							dup2(fd[1], 1);
						close(fd[0]);
						execve(//command, //argv for command, //env );
						exit(1);
					}
					else // in parent
					{
						wait(NULL);
						close(fd[1]);
						og_fd = fd[0];
						i++; // increment the argv to the next pipe
					}
					free_av(//pipe array);
				}
			}
			else
			{
				// just execute
			}
			// research how pipes work
			// research how execve works
		}
		free(av);
		i++;
	}
}