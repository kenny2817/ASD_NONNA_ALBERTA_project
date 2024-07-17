# ASD_NONNA_ALBERTA_project

this was a project to put into practice what we learned in the Algorithms and data structures course (unitn - A.Montresor)

- [curse page](https://cricca.disi.unitn.it/montresor/teaching/asd/)
- [text](prog2.testo.pdf)
- [solution](solution.cpp)

## My group made **99.24** pts using this logic:
- F = number of strings (Fili)
- G = number of clews (Gomitoli)

### cross counting
- Data structure: Fenwick tree
- Complexity: O(F*log(G))

### Brute force
- Random swap

### Heuristics
- Mean
- Median
- Range limit for random swaps

## What we could have done better?

we found that the brute force was lacking and that some improving could be done:

knowing that by swapping two Gs the crossings are modified only between them leaving all the others untouched, we could have mantained the fenwick tree in memory to perform smaller calculations in the bruteforce phase

another possible approach is to do swaps between G that are adjacent making the cross counting a lot faster