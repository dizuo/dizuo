///////////////////////////////////////////////////////////////////////////////
// View.h
// ======
// View component
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-06-07
// UPDATED: 2007-06-08
///////////////////////////////////////////////////////////////////////////////

#ifndef WIN_VIEW_H
#define WIN_VIEW_H

#include <vector>
#include "controls.h"
#include "TreeItem.h"

namespace Win
{
    class View
    {
    public:
        View();
        ~View();

        void init(HWND handle);
        void addChildItem();
        void deleteChildItem();
        void showPopupMenu(HWND handle, int x, int y);

        // handle TreeView Notifications
        void tvBeginLabelEdit(NMTVDISPINFO* tvDispInfo);
        void tvEndLabelEdit(NMTVDISPINFO* tvDispInfo);
        void tvKeyDown(NMTVKEYDOWN* tvKeyDown);
        void tvBeginDrag(NMTREEVIEW* tv);
        void tvDragItem(HWND parent, int x, int y);
        void tvDropItem();
        bool isTreeViewItemDragged() const { return tvItemDragged; };

    private:
        // member functions
        void moveTreeViewItem(TreeView* tv, HTREEITEM dragged, HTREEITEM target);
        void buildItemList(TreeView* tv, HTREEITEM from);
        void getItemInfo(TreeView* tv, HTREEITEM item, TreeItem* treeItem);
        void addChildren(TreeView* tv, HTREEITEM first);

        TreeView treeView;
        Button   buttonExit;
        HMENU tvMenu;                           // handle to popup menu
        HIMAGELIST imageList;                   // handle to imagelist for items

        // for drag-and-drop operation in TreeView
        HTREEITEM tvDraggedItem;                // handle to item being dragged
        HIMAGELIST tvDraggedImageList;          // handle to LmageList created for dragging
        bool tvItemDragged;
        std::vector <TreeItem> items;           // container for items to be moved
    };
}

#endif
