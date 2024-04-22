#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int n = argc - 1;
	int pipes[n][2]; // 0 = read end; 1 = write end
	int i;

	if (argc <2)
	{
		return EINVAL; // invalid arguments
	}

	for (i = 0; i < n - 1; i++) // create pipes; # pipes = # processes - 1
	{
		if (pipe[pipes[i]] < 0)
			{
				perror("pipe");
				return errno; // or exit/abort ??
			}
	}

	for (i = 0; i < n; i++) // for each process
	{
		pid_t pid = fork(); // fork the process
		if (pid != 0)
		{
			perror("fork");
			return errno;
		}
		else if (pid == 0) // for the child process
		{
			if (i > 0) // set up read end of pipe (after the first command)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) // redirect standard input of current process to read from the read end of the last pipe
				{
					perror("dup2");
                    return errno;
				}
			}

			if (i < n - 1) // set up write end of pipe (before the last command)
			{
				if (dup2(pipes[i][1], STDIN_FILENO) == -1) // redirect standard output of current process to write to the write end of the pipe
				{
					perror("dup2");
                    return errno;
				}
			}

			for (int j = 0; j < n - 1; j++) { // close or no here?
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execlp(argv[i + 1], argv[i + 1], NULL); // execute the executable
            perror("execlp");
            return errno;
		}
	}

	for (i = 0; i < n - 1; i++) 
	{
		close(pipes[i][0]);
    	close(pipes[i][1]);
	}

	for (i = 0; i < n; i++) // wait for all child processes to terminate
	{
        int status;
        pid_t pid = wait(&status);
        if (pid == -1) {
            if (errno == ECHILD) {
                return errno; // there are no more child processes
            } else {
                perror("wait");
                return errno;
            }
        } else {
            if (!WIFEXITED(status)) {
                perror("Child process did not exit normally");
    			return errno;
            }
        }
    }


	return 0;
}
