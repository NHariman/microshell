#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void	printer(char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	write(2, str, i);
}

int		execute(int i, char **av, char **env, int tmp_fd)
{
	close(tmp_fd);
	av[i] = NULL;
	execve(av[0], av, env);
	printer("error: cannot execute ");
	printer(av[0]);
	printer("\n");
	return (1);
}

int main(int ac, char **av, char **env)
{
	int fd[2];
	int tmp_fd = 0;
	int i = 0;
	int pid = 0;

	if (ac < 2)
		return (0);
	tmp_fd = dup(STDIN_FILENO);
	while (av[i] != NULL && av[i + 1] != NULL)
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (!strcmp(av[0], "cd"))
		{
			if (i != 1)
				printer("error: cd: bad arguments\n");
			else if (chdir(av[1]))
			{
				printer("error: cd: cannot change directory to ");
				printer(av[1]);
				printer("\n");
			}
		}
		else if (av[i] == NULL || !strcmp(av[i], ";"))
		{
			pid = fork();
			if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				if (execute(i, av, env, tmp_fd));
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
			if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (execute(i, av, env, tmp_fd);)// missing the if statement
					return (1);
			}
			else
			{
				close(tmp_fd);
				close(fd[1]);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(fd[0]);
				close(fd[0]);
			}			
		}
	}
	close(tmp_fd);
	return (0);
}