#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("Hello, world!\n");
  int fd[2];
    int mypid=getpid();
    // Create the pipe
    if (pipe(fd) == -1) {
        printf("pipe");
        return 1;
    }
    //int *s =malloc(sizeof(int));
    // Fork the process
    int pid = fork();

    if (pid < 0) {
        printf("fork");
        return 1;
    }

    if (pid > 0) {
        close(fd[0]); 
        //printf("before\n");
        char c='d';
        //printf("after");
        void *test= (void*)map_shared_pages( mypid,pid, (uint64)&c, sizeof(char));
        printf("the address is: %p\n", test);
        //printf("test value is: %d\nat address: %p\n", *s, s);
        write(fd[1], &test,sizeof(uint64));
        close(fd[1]);
        wait(0); 
        printf("doneF\n");
    } else {
        sleep(3);
        // Child process
        close(fd[1]); // Close unused write end
        char* val;
        read(fd[0], &val, sizeof(uint64));
        printf("Value received from parent: %c address is: %p\n", *val,val);
        close(fd[0]); // Close read end after reading
        printf("this is it\n");
    }

    return 0;
}

// #define PGSIZE 4096

// int main(void) {
//     int pid;
//     char *shared_memory;
//     struct proc *src_proc;
//     // Allocate a page of memory in the parent process
//     shared_memory = sbrk(PGSIZE);
//     if (shared_memory == (char *)-1) {
//         printf("sbrk failed\n");
//         exit(1);
//     }

//     src_proc = myproc();
//     dst_proc = myproc(); // Placeholder, to be replaced by child's proc

//     // Write a message to the shared memory
//     strcpy(shared_memory, "Hello from parent!");

//     pid = fork();
//     if (pid < 0) {
//         printf("fork failed\n");
//         exit(1);
//     }

//     if (pid == 0) {
//         // Child process
//         dst_proc = myproc();

//         // Map the shared memory from the parent process
//         if (map_shared_pages(src_proc, (uint64)shared_memory, PGSIZE) < 0) {
//             printf("map_shared_pages failed\n");
//             exit(1);
//         }

//         // Read and print the message from the shared memory
//         printf("Child read: %s\n", shared_memory);

//         exit(0);
//     } else {
//         // Parent process waits for the child to finish
//         wait(0);
//     }

//     exit(0);
// }



// int
// main(int argc, char *argv[]){
//     char* shared=sbrk(4096);
//     strcpy(shared,"Hello from parent!");
//     printf("Parent process pid: %d\n", getpid());
//     int pid=fork();
//     if(pid<0){
//         printf("fork failed\n");
//         exit(1);
//     }
//     if(pid==0){
//         printf("Child process pid: %d\n", getpid());
//     }
//     wait(0);
//     return 1;
// }
