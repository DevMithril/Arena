## Arena

Une library implémentant des arenas.
Une arena est un bloc mémoire alloué en une fois, on redistribue ensuite la mémoire de cette arena via un allocateur spécifique.
Une arena peut être libérée d'un seul coup (comme pour l'allocation initiale) sans devoir nécessairement suivre tous les blocs alloués.
De plus, utiliser une arena réduit le nombre d'appels système (syscall).

Cette library implémente les arenas suivantes :

 - arena RAM :
     - utilise un allocateur RAM (heap / tas)
     - allocations plus rapides que celles du malloc() standard (pas de syscall)

 - arena LIFO :
     - utilise un allocateur LIFO (stack / pile)
     - allocations beaucoup plus rapides que celles du malloc() standard (ce qui est normal pour ce type d'allocateur)
     - à utiliser soit dans des cas spécifiques, soit lorsque l'on ne libère pas la mémoire que l'on alloue,
     en effet, la nature de l'allocateur implique que la libération d'un bloc mémoire libère par la même
     occasion tous les blocs alloués après lui

 - arena FSC (Fixed-Size Chunks) :
     - utilise un allocateur RAM avec une contrainte sur les dimensions des allocations
     - allocations un peu plus lentes qu'avec l'allocateur LIFO
     - avec cet allocateur, les blocs mémoire sont d'une taille spécifique unique pour toute l'arena,
     vous ne pouvez donc allouer que des blocs de la même taille dans une arena donnée

#### Compilation :

(Re)Compilez `libarena.so` avec :

    $ make lib

#### Utilisation :

Copiez `arena.h` et `libarena.so` dans votre projet, `#include "arena.h"` dans votre code.

Puis compilez avec les flags adéquats :

    $ gcc -o your_project your_project.c -L./ -Wl,-rpath=./ -larena

#### Demo :

    $ cd demo
    $ make run
Affiche une comparaison de temps d'execution pour les différentes fonctions des allocateurs :
 - `malloc_lifo()`, `malloc_fsc()`, `malloc_ram()`, `malloc()`
 - `free_lifo()`, `free_fsc()`, `free_ram()`, `free()`