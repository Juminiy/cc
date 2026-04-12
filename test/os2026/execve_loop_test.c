#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]) {
    char command[256];

    for(;;){
        printf("sh> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // EOF or error
        }

        // Remove newline
        command[strcspn(command, "\n")] = '\0';

        // Skip empty commands
        if (strlen(command) == 0) {
            continue;
        }

        int fds[2];
        if (pipe(fds) == -1) {
            perror("pipe");
            continue;
        }

        pid_t chpid = fork();
        if (chpid == -1) {
            perror("fork");
            close(fds[0]);
            close(fds[1]);
            continue;
        }

        if (chpid == 0) { // Child process
            close(fds[0]); // Close read end

            // Redirect stdout to pipe write end
            if (dup2(fds[1], STDOUT_FILENO) == -1) {
                perror("dup2 stdout");
                exit(EXIT_FAILURE);
            }
            close(fds[1]);

            // Simple command parsing (just for testing)
            // For now, execute the command directly
            execlp(command, command, NULL);

            // If execlp fails
            perror("exec error");
            exit(EXIT_FAILURE);
        } else { // Parent process
            close(fds[1]); // Close write end

            int status;
            waitpid(chpid, &status, 0);

            // Read output from pipe
            char buffer[4096];
            ssize_t bytes_read;
            while ((bytes_read = read(fds[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[bytes_read] = '\0';
                printf("%s", buffer);
            }

            close(fds[0]);

            if (WIFEXITED(status)) {
                printf("\n[Exit code: %d]\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("\n[Terminated by signal: %d]\n", WTERMSIG(status));
            }
        }
    }

    printf("\n");
    return 0;
}