#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>


int agNodeDegree(Agraph_t *g, Agnode_t *node)
{
    Agedge_t *edge = agfstedge(g, node);
    if (!edge)
        return 0;

    int i = 1;
    while (edge = agnxtedge(g, edge, node))
        i++;

    return i;
}


int agMaxNodeDegree(Agraph_t *g, char *name)
{
    Agnode_t *node;
    int max = 0, degree;
    node = agfstnode(g);

    if (node)
    {
        max = agNodeDegree(g, node);
        strcpy(name, agnameof(node));
    }

    while (node = agnxtnode(g, node))
    {
        degree = agNodeDegree(g, node);
        if (degree > max)
        {
            max = degree;
            strcpy(name, agnameof(node));
        }
    }

    return max;
}


void printNeighbours(Agraph_t *g, char*name)
{
    Agnode_t *node;
    node = agnode(g, name, 0);
    if (!node)
    {
        printf("node doesn't exist");
        return;
    }
    Agedge_t *edge = agfstout(g, node);
    if (edge )
        printf("%s %s\n", agnameof(aghead(edge)), agnameof(agtail(edge)));
    else
        return;
    while (edge = agnxtout(g, edge))
        printf("%s %s\n", agnameof(aghead(edge)), agnameof(agtail(edge)));
}


#define BUFFER_LEN_MAX          (100)
#define STR_LEN_MAX             (40)

/**
 * Problema 1 - 4p
 * Calculati maxInDegree pentru graful dat. Intoarceti valoarea maximă si numele nodului care are aceasta valoare.
 * Modificati crearea grafului (din main) astfel incât graful sa fie orientat.
 */

int MaxInDegree(Agraph_t *g, char *name)
{

    Agnode_t *node;
    int max = 0, degree;
    node = agfstnode(g);

    if (node)
    {
        max = agNodeDegree(g, node);
        strcpy(name, agnameof(node));
    }

    while (node = agnxtnode(g, node))
    {
        degree = agNodeDegree(g, node);
        if (degree > max)
        {
            max = degree;
            strcpy(name, agnameof(node));
        }
    }

    return max;

}

/**
 * Problema 2 - 3p
 * Scrieti o functie care afiseaza nodurile din graf sortate descrescator dupa gradul nodului (intrare + iesire).
 */
void Descrescator(Agraph_t *g)
{
}

/**
 * Problema 3 - 3p
 * Primind numele a doua noduri (sursa si destinatie) verificati daca exista un drum cu maxim 2 pasi intre ele.
 * Functia intoarce 1 daca exista drum, 0 altfel.
 */
int Drum2(char *sursa, char *destinatie)
{
    return 0;
}

/**
 * Bonus - 2p
 * Generati subgraful dat de unul din noduri, salvati-l ca fișier .dot si vizualizati-l folosind webgraphviz.com
 */
Agraph_t *Subgraf(Agraph_t *g, Agnode_t *node)
{
    return NULL;
}


int main(int argc, char **argv)
{
    // sudo apt update && sudo apt install graphviz

    Agraph_t *g;
    char buffer[BUFFER_LEN_MAX];

    // Create a simple undirected graph
    g = agopen("g", Agdirected, NULL);

    // Open the input file
    FILE *fp = fopen("graph.txt", "rt");
    if (!fp)
    {
        printf("can't open file\n");
        return -1;
    }

    // Read the graph edges from the input file
    while (fgets(buffer, BUFFER_LEN_MAX, fp))
    {
        Agnode_t *srcNode, *destNode;
        char src[STR_LEN_MAX], dest[STR_LEN_MAX];
        int weight;

        // Parse a single line of text
        int nb_tokens = sscanf(buffer, "%s %s %d", src, dest, &weight);
        if (nb_tokens != 3)
        {
            printf("input file is not well-formed at line: %s\n", buffer);
            break;
        }

        // Create the node or return it if it exists
        srcNode = agnode(g, src, 1);
        destNode = agnode(g, dest, 1);
        agedge(g, srcNode, destNode, NULL, 1);
    }

    if (1)
    {
        char nameMax[STR_LEN_MAX];
        int degMax;

        degMax = agMaxNodeDegree(g, nameMax);
        printf("name: %s, max degree = %d\n", nameMax, degMax);

        if (0)
        {
            printNeighbours(g, nameMax);
        }
    }
    
    // Just write the graph without layout
    if (0)
    {
        agwrite(g, stdout);
    }

    // Temele de laborator
    char nameMax2[STR_LEN_MAX];
    int degMax2 = agMaxNodeDegree(g, nameMax2);
    printf("Pb1. name: %s, max degree = %d\n", nameMax2, degMax2);

    // Free the graph memory
    agclose(g);

    return 0;
}
