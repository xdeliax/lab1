#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int n = argc - 1;
	int pipes[n-1][2]; // 0 = read end; 1 = write end
	int i;
	int fd_in = STDIN_FILENO;

	if (argc <2)
	{
		return EINVAL; // invalid arguments
	}

	if (argc == 2) 
	{
        if (execlp(argv[1], argv[1], NULL) == -1) {
            perror("execlp");
            exit(errno);
        } 
    }

	for (i = 0; i < n - 1; i++) 
	{
        if (pipe(pipes[i]) == -1) 
		{
            perror("pipe");
            exit(errno);
        }
    }

	pipes[0][0] = STDIN_FILENO; // first process reads from standard input

	for (i = 0; i < n; i++) // for each process
	{
		pid_t pid = fork(); // fork the process
		if (pid < 0)
		{
			perror("fork");
			exit(errno);
		}
		else if (pid == 0) // for the child process
		{
			if (i > 0) // set up read end of pipe (after the first command)
			{
				if (dup2(pipes[i-1][0], STDIN_FILENO) == -1) // redirect standard input of current process to read from the read end of the last pipe
				{
					perror("dup2");
                    exit(errno);
				}
				close(pipes[i-1][0]);
			}

			if (i < n - 1) // set up write end of pipe (before the last command)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1) // redirect standard output of current process to write to the write end of the pipe
				{
					perror("dup2");
                    exit(errno);
				}
				close(pipes[i][1]);
			}

            if (execlp(argv[i + 1], argv[i + 1], NULL) == -1) // execute the executable
			{
				perror("execlp");
            	exit(errno);
			} 
           
		}
		else
		{
			close(pipes[i][1]); // close write end and wait for children processes to terminate
			waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) // if child process failed
			{
                int exit_status = WEXITSTATUS(status);
                close(pipes[i][0]);
                exit(exit_status);
            }
			close(pipes[i][0]);

		}
	}

	return 0;
}
