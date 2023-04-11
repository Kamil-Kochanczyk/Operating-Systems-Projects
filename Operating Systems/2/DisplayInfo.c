#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Program wypisujący informacje o procesie, który go wywołał
int main()
{
	printf("------------------------------------\n");
	printf("------------------------------------\n");
	
	int uid = getuid();
	int gid = getgid();
	int pid = getpid();
	int ppid = getppid();
	int pgid = getpgid(0);
	
	printf("UID = %d\n", uid);
	printf("GID = %d\n", gid);
	printf("PID = %d\n", pid);
	printf("PPID = %d\n", ppid);
	
	if (pgid == -1)
	{
		perror("PGID error");
	}
	else
	{
		printf("PGID = %d\n", pgid);
	}

	printf("------------------------------------\n");
	printf("------------------------------------\n");
	
	printf("\n\n\n");
	
	return 0;
}
