#include <mxml.h>


int main(){

    FILE* filePointer;
    mxml_node_t *tree;
    filePointer=fopen("users.xml","r");
    tree = mxmlLoadFile(NULL, filePointer, MXML_OPAQUE_CALLBACK);
    fclose(filePointer);
    mxml_node_t *node;
    FILE* fp = fopen("codebeautify.xml","w");
    mxmlSaveFile(tree, fp, MXML_NO_CALLBACK);
    fclose(fp);

    

}