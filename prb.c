#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define  EPLISON 1.0e-20
#define  N_ALPH 256        /* num. of alphabets (EOF is not counted.) */
#define  N_SYM  (N_ALPH+1) /* num. of all symbol codes including "End-Of-File" code */
                           /* 8bit 256 alphabets + 1 EOF */
#define N_MAX_SYM_LEN 200
#define N_MAX_NODE ( 2*N_SYM-1 )
#define CMAX 256

#define N_MAX_CODE_LEN 64

#define NO_NODE -1

typedef struct node NODE;

struct node {
    int  parent,    /* parent node */
         child_0,   /* child 0 node */
         child_1;   /* child 1 node */
    int code;
    double prob;
    char sym[ N_MAX_SYM_LEN ]; /* symbol */
};

NODE node[ N_MAX_NODE ];

int find_min_prob_nodes(int max_node, int exclude_node)
{
  int id_min = -1;
  for (int i_node = 0; i_node < max_node; i_node++)
  {
    if (i_node != exclude_node && node[i_node].parent == -1)
    {
      if (id_min == -1) id_min = i_node;
      if (node[i_node].prob < node[id_min].prob) id_min = i_node;
    }
  }
  return id_min;
}

void makeHuffmanTree()
{
    int i, counter = 0;
    int node_counter = 257;
    int left, right;
    left = right = -1;

    for (int i_node = N_SYM; i_node < N_MAX_NODE; i_node++)
    {
        right = find_min_prob_nodes(i_node, -1);
        left = find_min_prob_nodes(i_node, right);
        node[i_node].child_0 = right;
        node[i_node].child_1 = left;
        node[i_node].prob = node[right].prob + node[left].prob;
        strcpy(node[i_node].sym, "\0");
        node[right].parent = i_node;
        node[left].parent = i_node;
        node[right].code = 0;
        node[left].code = 1;
    }
}

int main( int argc, char *argv[] )
{
    double prob;
    double ave_len = 0.0;
    char sym[ CMAX ];
    int i;
  
    FILE *fpr, *fpw;

    if( ( fpr = fopen( argv[1], "r" ) ) == NULL )
    {
        fprintf( stderr, "File open error %s\n", argv[1] );
        exit( 1 );
    }

    for( i=0; i<N_MAX_NODE; i++ )
    {
        node[i].parent = node[i].child_0 = node[i].child_1 = node[i].code = -1;
    }

    i = 0;

    while( fscanf( fpr, "%*d%s%*d%lf", sym, &prob ) != EOF )
    {

        strcpy( node[i].sym, sym );
        node[i].prob = prob;
        /*  
        printf( "%s", node[i].sym );
        printf( "%9.8f ",node[i].prob );
        printf("\n");
        */
        
        i++;
    } 

    makeHuffmanTree();

    if( ( fpw = fopen( argv[2], "w" ) ) == NULL )
    {
        fprintf( stderr, "File open error %s\n", argv[1] );
        exit( 1 );
    }

    for( i=0; i<257; i++ )
    {
        char hcode[N_MAX_CODE_LEN] = "\0";
        hcode[0] = '\0';
        int count = i;

        // get_hcode
        while(1)
        {
            if( node[count].parent == -1 )break;
                char copy[N_MAX_CODE_LEN];
                strcpy(copy, hcode);
                sprintf( hcode, "%d%s",  node[count].code, copy );
                count = node[count].parent;
        }

/* 
        printf("%d",i);
        printf( "%s:", node[i].sym );
        printf( "%f:",node[i].prob );
        printf("code=%d:",node[i].code );
        printf("parent=%d:",node[i].parent );
        printf("child0=%d:",node[i].child_0 );
        printf("child1=%d:",node[i].child_1 ); 
        printf("%s",hcode);
        printf("\n");
*/        
        int code_len = strlen(hcode);
        
        fprintf( fpw, "# %3d %s %.8f %5d %s\n", i, node[i].sym, node[i].prob, code_len, hcode );
        ave_len += code_len * node[i].prob;
    }
  
    fprintf( fpw, "average code length = %14.8f\n", ave_len);
    fprintf( fpw, "compression rate = %14.8f [%%] (ave_len/8)\n", 100 * ave_len / 8.0);
    fclose( fpr );
    fclose( fpw );

    return 0;
}