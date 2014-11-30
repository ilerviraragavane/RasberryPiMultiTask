#include <unistd.h>
#include <sched.h>
#include <stdio.h>

static void change_scheduler ( pid_t child, int sched);

int main ( )
{
	pid_t child1, child2;
	child1 = fork ( );
	if ( child1 == 0 )
	{
		// Fils1
		int n1 = execl ( "./play_midi_file", "play_midi_file", "tune.mid", NULL );
		//int n1 = execl ( "./calcul2", "calcul2", NULL );
		printf("execution fils1 = %d.\n",n1);
	}
	else
	{
		child2 = fork ( );
		if ( child2 == 0 )
		{
			// Fils2			
			int n2 = execl ( "./calcul1", "calcul1", NULL );
			printf("execution fils2 = %d.\n",n2);
		}
		else
		{
			// Père
			change_scheduler ( child1, SCHED_OTHER);
			change_scheduler ( child2, SCHED_OTHER);

			for ( ; ; );
		}
	}

	return 0;
}

void change_scheduler ( pid_t child, int sched)
{
	// Changement d'ordonnaceur
	//int sched_type = sched_getscheduler ( child );
	//printf ( "L'ordonnanceur de %d est %d.\n", child, sched_type );

	struct sched_param param;

	int success = sched_setscheduler ( child, sched, &param );
	//printf ( "sched_setscheduler a renvoyé : %d.\n", success );
	//perror ( "sched_setscheduler" );

	int sched_type = sched_getscheduler ( child );
	printf ( "L'ordonnanceur de %d est %d.\n", child, sched_type );
}
