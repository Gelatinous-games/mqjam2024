#pragma once

#include "raylib.h"

#include <stdio.h>
#include "string.h"

typedef struct {
    Texture2D *texture;
    char *texturePath;
    int texturePathLength;
} _TEXTUREMANAGER_LIBRARYTREE_NodeData_;


typedef struct {
    _TEXTUREMANAGER_LIBRARYTREE_NodeData_ *data;
    _TEXTUREMANAGER_LIBRARYTREE_Node_ *left;
    _TEXTUREMANAGER_LIBRARYTREE_Node_ *right;
} _TEXTUREMANAGER_LIBRARYTREE_Node_;

_TEXTUREMANAGER_LIBRARYTREE_Node_ *_TEXTUREMANAGER_rootNode;



_TEXTUREMANAGER_LIBRARYTREE_Node_ *_TEXTUREMANAGER_LIBRARYTREE_NewNode(char *pathOfTexture);
void _TEXTUREMANAGER_LIBRARYTREE_FreeNode(_TEXTUREMANAGER_LIBRARYTREE_Node_ **nodeToFree);

int _TEXTUREMANAGER_LIBRARYTREE_CompareNode(_TEXTUREMANAGER_LIBRARYTREE_NodeData_ *self, _TEXTUREMANAGER_LIBRARYTREE_NodeData_ *other);
_TEXTUREMANAGER_LIBRARYTREE_Node_ *_TEXTUREMANAGER_LIBRARYTREE_GetNodeFromPath(char *pathOfTexture);
int _TEXTUREMANAGER_LIBRARYTREE_PlaceNodeInTree(_TEXTUREMANAGER_LIBRARYTREE_Node_ *nodeToAdd);

void _TEXTUREMANAGER_Initialise();
void _TEXTUREMANAGER_Destroy();