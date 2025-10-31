## Arena

Une library implémentant des arenas.
Une arena est un bloc mémoire alloué en une fois, on redistribue ensuite la mémoire de cette arena via un allocateur spécifique.

Cette library implémente les arenas suivantes :

 - arena RAM :
     - utilise un allocateur RAM (heap / tas)
     - allocations moins rapides que celles du malloc() standard

 - arena LIFO :
     - utilise un allocateur LIFO (stack / pile)
     - allocations beaucoup plus rapides que celles du malloc() standard (ce qui est normal)
     - à utiliser soit dans des cas spécifiques, soit lorsque l'on ne libère pas la mémoire que l'on alloue,
     en effet, la nature de l'allocateur implique que la libération d'un bloc mémoire libère par la même
     occasion tous les blocs alloués après lui

#### Installation / Utilisation :

Chaque arena comprends une entête `arena_xxx.h` et une implémentation `arena_xxx.c` situés dans `src/`.

Vous pouvez copier dans votre projet : 
 - Une entête spécifique et son implémentation, puis inclure dans votre code `arena_xxx.h`
 - Toutes les entêtes / implémentations + `arena.h`, puis inclure dans votre code `arena.h`

Un makefile est fourni avec cette library, n'hésitez pas à l'utiliser pour build vos projets.

#### Demo :

    $ make run
Affiche une comparaison de temps d'execution pour les différentes fonctions des allocateurs :
 - `malloc_lifo()`, `malloc_ram()`, `malloc()`
 - `free_lifo()`, `free_ram()`, `free()`

#### Projets d'améliorations :

 - Améliorer les performances de `malloc_ram()`
 - Ajouter les fonctions `realloc_ram()`, `calloc_ram()`, `calloc_lifo()` et `memlen_ram()`
 - Ajouter une nouvelle arena avec un allocateur FSC (Fixed-Size-Chunks : comme RAM mais avec des blocs d'une taille spécifique à l'arena)