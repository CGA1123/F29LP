#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "node.h"

extern char * showSymb(int);

void prettytree(NODE * tree, int depth)
{
	int i;
	if(tree==NULL) return;

	for(i=0;i<depth;i++)putchar('-');

	switch(tree->tag){
	case NAME:
		printf("%s\n",tree->f.id);
		return;
	case NUMBER:
		printf("%d\n",tree->f.value);
		return;
	default:
		printf("%s\n",showSymb(tree->tag));
		prettytree(tree->f.b.n1,depth+1);
		prettytree(tree->f.b.n2,depth+1);
	}
}

