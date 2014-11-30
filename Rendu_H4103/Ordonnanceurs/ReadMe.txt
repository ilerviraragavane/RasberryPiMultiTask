Les dossiers contiennent les ordonnanceurs :
	- Collaboratif :
		Il y a un problème avec cet ordonnanceur, il fonctionne mais avec un nombre limité de processus. Il y a donc des versions avec 2 et 4 processus :
			* 2 : Fonctionnel
			* 4 : Non fonctionnel

	- Préemptif :
		Notre ordonnanceur gère les processus en attente, et la terminaison complète (ce qui n'est pas le cas dans les autres ordonnanceurs).

	- Préemptif avec priorités :
		Cet ordonnanceur est celui fourni par M. Marquet avec des modifications afin d'y intégrer la gestion des priorités.
		Cependant, nous n'avons pas réussi à intégrer cet ordonnanceur avec le lecteur de musique, nous avons donc intégrer les mêmes modifications dans notre ordonnanceur préemptif.