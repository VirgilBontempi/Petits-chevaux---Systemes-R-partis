#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	srand(time(NULL));
	int r = (rand() % 6)+1;
	
	switch (r)
	{
		case 1 :
			
			printf("┌───────┐\n");
			printf("│       │\n");
			printf("│   O   │\n");
			printf("│       │\n");
			printf("└───────┘\n");	
			break;
		case 2 :
			printf("┌───────┐\n");
			printf("│     O │\n");
			printf("│       │\n");
			printf("│ O     │\n");
			printf("└───────┘\n");			
			break;
		case 3 :
            printf("┌───────┐\n");
			printf("│     O │\n");
			printf("│   O   │\n");
			printf("│ O     │\n");
			printf("└───────┘\n");				
			break;
		case 4 :
			printf("┌───────┐\n");
			printf("│ O   O │\n");
			printf("│       │\n");
			printf("│ O   O │\n");
			printf("└───────┘\n");				
			break;
		case 5 :
			printf("┌───────┐\n");
			printf("│ O   O │\n");
			printf("│   O   │\n");
			printf("│ O   O │\n");
			printf("└───────┘\n");					
			break;
		case 6 :	
			printf("┌───────┐\n");
			printf("│ O   O │\n");
			printf("│ O   O │\n");
			printf("│ O   O │\n");
			printf("└───────┘\n");					
			break;
	}
	return r;
}

