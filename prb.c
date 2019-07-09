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

void getChild( int node_number )
{

    strcmp( node[node_number].sym, "\0" );
    node[node_number].parent = -1;
    node[node_number].code = -1;

    int i,count = 0;

    unsigned int first = 999999;
    unsigned int second = 999999;

    double prob0, prob1, prob2 = 0.0;

    prob0 = prob1 = 9.9999;
    
    for( i=0; i<node_number; i++ )
    {
        if( node[i].parent == -1 )
        {
            if( first > node[i].prob && second > node[i].prob )
            {
                first = node[i].prob;
                node[i].parent = node_number;
                node[i].code = 0;
                node[node_number].child_0 = i;
                prob0 = node[i].prob;
            //    printf("%f\n",node[i].prob);
            }
            else if( second > node[i].prob )
            {
                second = node[i].prob;
                node[i].parent = node_number;
                node[i].code = 1;
                node[node_number].child_1 = i;
                prob1 = node[i].prob;
            //    printf("%f\n",node[i].prob);
            }
        }
    }

    prob2 = prob0 + prob1;
    printf("%f\n", prob2);
    node[node_number].prob =  prob2; 
  //  printf("%f\n",node[node_number].prob);

} 

void makeHuffmanTree()
{
    int i, counter = 0;
    int node_counter = 257;

    while( 1 )
    {
        int counter = 0;


        for( i=0; i<N_MAX_NODE+1; i++ )if(node[i].parent == -1)counter++;
        if( counter == 1 )break;
        printf("ok%d\n",counter);
        printf("oknode%d\n",node_counter);

    //    getChild( node_counter );

        getChild( node_counter );
        node_counter++;

        //if(node_counter == 512)break;
    }

}

int main( int argc, char *argv[] )
{
    char filename[256];
    double prob;
    char sym[ CMAX ];
    int i, j;

    
    FILE *fpr, *fpw;

    if( ( fpr = fopen( argv[1], "r" ) ) == NULL )
    {
        fprintf( stderr, "File open error %s\n", argv[1] );
        exit( 1 );
    }


    for( i=0; i<CMAX; i++ )
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
 //   printf("ok\n");

    if( ( fpw = fopen( argv[2], "w" ) ) == NULL )
    {
        fprintf( stderr, "File open error %s\n", argv[1] );
        exit( 1 );
    }

    for( i=0; i<CMAX+1; i++ )
    {
        char hcode[N_MAX_CODE_LEN];
        hcode[0] = '\0';
        int count;
        char tmp[N_MAX_CODE_LEN];
        count = i;


        while( 1 )
        {
            if( node[count].code == -1 )break;
//            printf("%d",node[count].code);
            strcpy( tmp, hcode );
            sprintf( hcode, "%d%s",  node[count].code, tmp );
            count = node[count].parent;
        //    printf( "%s", node[count].sym );
        //    printf( "%9.8f ",node[count].prob );
        //    printf("\n");
        }
        for( i=0;i<512;i++)
        {
        printf("%d",i);
        printf( "%s:", node[i].sym );
        printf( "%f:",node[i].prob );
        printf("code=%d:",node[i].code );
        printf("parent=%d:",node[i].parent );
        printf("child0=%d:",node[i].child_0 );
        printf("child1=%d:",node[i].child_1 ); 
        printf("%s",hcode);
        printf("\n");
        }
        fprintf( fpw, "# %3d %s %9.8f %s\n", i, node[i].sym, node[i].prob, hcode );
    }

    fclose( fpr );
    fclose( fpw );

    return 0;
}