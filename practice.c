#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>

void	printerr(char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	write(2, str, i);
}

int		ft_execute(int i, char **av, char **envp, int tmp_fd)
{
	close(tmp_fd);
	av[i] = NULL;
	execve(av[0], av, envp);
	printerr("error: cannot execute ");
	printerr(av[0]);
	printerr("\n");
	return (1);
}

int		main(int ac, char **av, char **envp)
{
	int fd[2]; // pipes
	int tmp_fd; // tmp STDIN
	int i = 0;
	int pid = 0; // forks

	if (ac < 2)
		return (0);
	tmp_fd = dup(STDIN_FILENO);
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (!strcmp(av[0], "cd"))
		{
			if (i < 2)
				printerr("error: cd: bad arguments\n");
			else if (chdir(av[i]) < 0)
			{
				printerr("error: cd: cannot change directory to ");
				printerr(av[1]);
				printerr("\n");
			}
		}
		else if (av[i] == NULL || !strcmp(av[i], ";"))
		{
			pid = fork();
			if (pid < 0)
			{
				printerr("error: fatal\n");
				exit(1);
			}
			else if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				if (ft_execute(i, av, envp, tmp_fd))
					return (1);
			}
			else
			{
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if (!strcmp(av[i], "|"))
		{
			pipe(fd);
			pid = fork();
			if (pid < 0)
			{
				printerr("error: fatal\n");
				exit(1);
			}
			else if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_execute(i, av, envp, tmp_fd))
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