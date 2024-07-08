#include "texturemanager.h"


_TEXTUREMANAGER_LIBRARYTREE_Node_ *_TEXTUREMANAGER_LIBRARYTREE_NewNode(char *pathOfTexture){
    // ... allocates the node
    _TEXTUREMANAGER_LIBRARYTREE_Node_ *newNode = (_TEXTUREMANAGER_LIBRARYTREE_Node_ *)malloc(sizeof(_TEXTUREMANAGER_LIBRARYTREE_Node_));
    // and the data elem
    newNode->data = (_TEXTUREMANAGER_LIBRARYTREE_NodeData_ *)malloc(sizeof(_TEXTUREMANAGER_LIBRARYTREE_NodeData_));
    if(pathOfTexture){
        // ... had path
        newNode->data->texture = (Texture2D *)malloc(sizeof(Texture2D));
        *(newNode->data->texture) = LoadTexture(pathOfTexture);
        newNode->data->texturePathLength = sizeof(pathOfTexture);
        newNode->data->texturePath = (char *)malloc(sizeof(newNode->data->texturePathLength));
        strcpy((newNode->data->texturePath), pathOfTexture);
    }
    else {
        // ... none path
        newNode->data->texture = 0;
        newNode->data->texturePath = 0;
        newNode->data->texturePathLength = 0;
    }
    newNode->left = 0;
    newNode->right = 0;
}

void _TEXTUREMANAGER_LIBRARYTREE_FreeNode(_TEXTUREMANAGER_LIBRARYTREE_Node_ **nodeToFree){
    // real and exists
    if(nodeToFree && *nodeToFree){
        /// data exists
        if((*nodeToFree)->data){
            /// bin the path
            free( (*nodeToFree)->data->texturePath );
            (*nodeToFree)->data->texturePath = 0;
            // bin texture
            UnloadTexture( *((*nodeToFree)->data->texture) );
            // bin texture pointer
            free( (*nodeToFree)->data->texture );
            (*nodeToFree)->data->texture = 0;
            // bin node data pointer
            free( (*nodeToFree)->data );
            (*nodeToFree)->data = 0;
        }

        // this is assumed to already be done tho? dangerous if it isnt
        // // detach the left and rights
        // (*nodeToFree)->left = 0;
        // (*nodeToFree)->right = 0;

        // bin node pointer
        free( (*nodeToFree) );
        (*nodeToFree) = 0;
    }
    // done
}

int _TEXTUREMANAGER_LIBRARYTREE_CompareNode(_TEXTUREMANAGER_LIBRARYTREE_NodeData_ *self, _TEXTUREMANAGER_LIBRARYTREE_NodeData_ *other){
    // ...
    // TODO: aaa
}

_TEXTUREMANAGER_LIBRARYTREE_Node_ *_TEXTUREMANAGER_LIBRARYTREE_GetNodeFromPath(char *pathOfTexture){
    // ...
    // TODO: aaa 
    // TODO: return the node pointer when found
    // TODO: return null otherwise
}

int _TEXTUREMANAGER_LIBRARYTREE_PlaceNodeInTree(_TEXTUREMANAGER_LIBRARYTREE_Node_ *nodeToAdd){
    // ...
}

void _TEXTUREMANAGER_Initialise(){
    // ...
    // TODO : make the root node
    
}

void _TEXTUREMANAGER_Destroy(){
    // ...
    // TODO : unallocate all nodes
}