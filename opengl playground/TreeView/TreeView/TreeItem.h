///////////////////////////////////////////////////////////////////////////////
// TreeItem.h
// ==========
// a struct variable of TreeView item
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-10-01
// UPDATED: 2007-10-02
///////////////////////////////////////////////////////////////////////////////

#ifndef TREEITEM_H
#define TREEITEM_H

#include <windows.h>
#include <string>

struct TreeItem
{
    std::wstring text;
    HTREEITEM handle;
    HTREEITEM parentHandle;
    HTREEITEM previousHandle;
    int imageIndex;
    int selectedImageIndex;

    // init all members
    TreeItem::TreeItem() : handle(0), parentHandle(0), previousHandle(0), imageIndex(0), selectedImageIndex(0) {}
};

#endif
