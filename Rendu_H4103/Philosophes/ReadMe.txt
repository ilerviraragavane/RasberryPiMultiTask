Ce dossier contient le code des philosophes
- Pour enlever le processus supplémentaire, il faut commenter l'instruction "create_process(&balayer,...)" dans start_kernel() de kernel.c
- L'ordonnanceur et le préemptif sans priorité basé sur l'ordonnanceur de M.Marquet et complété avec semaphores et mutex.
- En cas d'interblocage, on allume la led a l'infini : on le voit grace au breakpoint ajouté à kernel.c ligne 16 
