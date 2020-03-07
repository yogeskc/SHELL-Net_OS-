/**************************************************************
* Class: CSC-415-03 Spring 2020
* Project: <project name, like Assignment 1 – Simple Shell
*
* File: Shell.c
* Description:
*
**************************************************************/

void loop(void){
  //temporary variable
   char *line;
   char **args; //array of strings
   int status;

   //read the commands entered by the user
   //after input it will parse the commands so computer can understand the commands
   //execute the commands
   do{
        printf("%s","SHELL@root >");
        line = read();
        args = parse(line);

        status = execute(args);

         //freeing the memory
   free(line);
   free(args);

   }while(status);


}


int main(int argc, char** argv){

/* code */
//load the configuration


//run an REPL loop
loop();

//Perform any shutdown cleanup
return 0;

}
