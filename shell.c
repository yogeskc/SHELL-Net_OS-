/**************************************************************
* Class: CSC-415-03 Spring 2020
* Project: Simple Shell (NET_OS --version1)
*
**************************************************************/
    #include <sys/wait.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	
    //function delclaration for specifying certain commands

    int lsh_exit(char **args); 
    

    //listing the commands
    
    char *extra_str[]={
        "exit"
    }; 

    int (*extra_func[])(char **) ={
        &lsh_exit
    };

    int lsh_nums(){
        return sizeof (extra_str)/sizeof(char *); 

    }

    //xtra function implementations
    int lsh_exit(char **args){
        return 0; 
    }

  
	int lsh_launch(char **args)
	{
	  pid_t pid, wpid;
	  int status;
	
	  pid = fork();
	  if (pid == 0) {
	    // Child process
	    if (execvp(args[0], args) == -1) {
            perror("ls");
           printf("Child process, pid = %u\n",getpid());
	       
	    }
	    exit(EXIT_FAILURE);
	  } else if (pid < 0) {
	    // Error forking
	    perror("ls");
	  } else {
	    // Parent process
	    do {
	      wpid = waitpid(pid, &status, WUNTRACED);
          printf("Parent process, pid = %u\n",getpid());
	    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	  }
	
	  return 1;
	}

	int lsh_execute(char **args)
	{
	  int i;
	
	  if (args[0] == NULL) {
	    // An empty command was entered.
	    return 1;
	  }
	    for (i=0; i< lsh_nums();i++){
           if(strcmp(args[0],extra_str[i])== 0){
               return (*extra_func[i])(args);
           }
       }
	  return lsh_launch(args);
	}
	
	#define LSH_RL_BUFSIZE 1024
	/**
	   @brief Read a line of input from stdin.
	   @return The line from stdin.
	 */
	char *lsh_read_line(void)
	{
	  int bufsize = LSH_RL_BUFSIZE;
	  int position = 0;
	  char *buffer = malloc(sizeof(char) * bufsize);
	  int c;
	
	  if (!buffer) {
	    fprintf(stderr, "lsh: allocation error\n");
	    exit(EXIT_FAILURE);
	  }
	
	  while (1) {
	    // Read a character
	    c = getchar();
	
	    // If we hit EOF, replace it with a null character and return.
	    if (c == EOF || c == '\n') {
	      buffer[position] = '\0';
	      return buffer;
	    } else {
	      buffer[position] = c;
	    }
	    position++;
	
	    // If we have exceeded the buffer, reallocate.
	    if (position >= bufsize) {
	      bufsize += LSH_RL_BUFSIZE;
	      buffer = realloc(buffer, bufsize);
	      if (!buffer) {
	        fprintf(stderr, "lsh: allocation error\n");
	        exit(EXIT_FAILURE);
	      }
	    }
	  }
	}
	
	#define LSH_TOK_BUFSIZE 64
	#define LSH_TOKEN " \t\r\n\a"
	
    //splits line into tokens 
	char **lsh_split_line(char *line)
	{
	  int bufsize = LSH_TOK_BUFSIZE, position = 0;
	  char **tokens = malloc(bufsize * sizeof(char*));
	  char *token;
	
	  if (!tokens) {
	    fprintf(stderr, "ls: allocation error\n");
	    exit(EXIT_FAILURE);
	  }
	
	  token = strtok(line, LSH_TOKEN);
	  while (token != NULL) {
	    tokens[position] = token;
	    position++;
	
	    if (position >= bufsize) {
	      bufsize += LSH_TOK_BUFSIZE;
	      tokens = realloc(tokens, bufsize * sizeof(char*));
	      if (!tokens) {
	        fprintf(stderr, "ls: allocation error\n");
	        exit(EXIT_FAILURE);
	      }
	    }
	
	    token = strtok(NULL, LSH_TOKEN);
	  }
	  tokens[position] = NULL;
	  return tokens;
	}
	
	//MAIN FUNCTION FOR THE PROGRAM
	void lsh_loop(void)
	{
	  char *line;
	  char **args;
	  int status;
	
	  do {
	    printf("> ");
	    line = lsh_read_line();
	    args = lsh_split_line(line);
	    status = lsh_execute(args);
	
	    free(line);
	    free(args);
	  } while (status);
	}
	
	
	int main(int argc, char **argv)
	{
	 
	  // Run command loop.
	  lsh_loop();
	
	  // Perform any shutdown/cleanup.
	
	  return EXIT_SUCCESS;
	}
