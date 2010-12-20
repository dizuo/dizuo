///////////////////////////////////////////////////////////////////////////////
// View.cpp
// ========
// View component
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2007-06-07
// UPDATED: 2007-10-02
///////////////////////////////////////////////////////////////////////////////


#include "View.h"
#include "resource.h"
#include <sstream>
using std::wstringstream;
using std::ends;
using namespace Win;

const int MAX_STRING = 256;


///////////////////////////////////////////////////////////////////////////////
// ctor/dtor
///////////////////////////////////////////////////////////////////////////////
View::View() :tvItemDragged(false), tvDraggedItem(0), tvDraggedImageList(0), tvMenu(0) {}
View::~View()
{
    ::ImageList_Destroy(imageList);
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void View::init(HWND handle)
{
    treeView.set(handle, IDC_TREE);
    buttonExit.set(handle, IDC_EXIT);

    // create ImageList
    const int IMAGE_WIDTH = 16;
    const int IMAGE_HEIGHT = 16;
    const int IMAGE_COUNT = 3;
    const int GROW_SIZE = 4;                    // the number of images when ImageList needs to grow for new images

    // create ImageList control first
    imageList = ::ImageList_Create(IMAGE_WIDTH, IMAGE_HEIGHT, ILC_COLORDDB | ILC_MASK, IMAGE_COUNT, GROW_SIZE);

    // load icons and add them to ImageList
    // PARAMS: hInstance, filename, type, width, height, flag
    HICON icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDERCLOSED), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);
    icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_FOLDEROPEN), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);
    icon = (HICON)::LoadImage(::GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPDEFAULT), IMAGE_ICON, IMAGE_WIDTH, IMAGE_HEIGHT, LR_DEFAULTCOLOR);
    ::ImageList_AddIcon(imageList, icon);

    // make sure all icons are in the ImageList
    if(::ImageList_GetImageCount(imageList) == IMAGE_COUNT)
    {
        treeView.setImageList(imageList);
    }

    // add root item
    HTREEITEM parentItem = treeView.insertItem(L"Parent", TVI_ROOT, TVI_LAST, 2, 2);

    // add some children
    treeView.insertItem(L"Child 1", parentItem, TVI_LAST, 0, 1);
    treeView.insertItem(L"Child 2", parentItem, TVI_LAST, 0, 1);
    treeView.insertItem(L"Child 3", parentItem, TVI_LAST, 0, 1);
    treeView.insertItem(L"Child 4", parentItem, TVI_LAST, 0, 1);
}



///////////////////////////////////////////////////////////////////////////////
// add a child item
///////////////////////////////////////////////////////////////////////////////
void View::addChildItem()
{
    static int count = 0;
    wstringstream wss;

    wss << L"Child " << ++count << ends;

    treeView.insertItem((wchar_t*)wss.str().c_str(), treeView.getRoot(), TVI_LAST, 0, 1);
}



///////////////////////////////////////////////////////////////////////////////
// delete an item
///////////////////////////////////////////////////////////////////////////////
void View::deleteChildItem()
{
    HTREEITEM hItem;
    hItem = treeView.getChild(treeView.getRoot());
    treeView.deleteItem(hItem);
}



///////////////////////////////////////////////////////////////////////////////
// remember the handle of editbox control when an item of TreeView start editing
// NMTVDISPINFO consists of NMHDR and TVITEM.
///////////////////////////////////////////////////////////////////////////////
void View::tvBeginLabelEdit(NMTVDISPINFO* tvDispInfo)
{
    if(tvDispInfo->hdr.hwndFrom == treeView.getHandle())
    {
        // limit the length of text
        // NOTE: It does not need to handle TVN_BEGINLABELEDIT to update the edited text.
        HWND editBoxHandle = treeView.getEditControl();
        ::SendMessage(editBoxHandle, EM_SETLIMITTEXT, (WPARAM)(MAX_STRING-1), 0);
    }
}



///////////////////////////////////////////////////////////////////////////////
// update the text of editbox control
// NMTVDISPINFO consists of NMHDR and TVITEM.
///////////////////////////////////////////////////////////////////////////////
void View::tvEndLabelEdit(NMTVDISPINFO* tvDispInfo)
{
    if(tvDispInfo->hdr.hwndFrom == treeView.getHandle())
    {
        // NMTVDISPINFO contains TVITEM struct to identify which item is edited
        // and the new edited string in TVITEM.pszText.
        // We only need call TVM_SETITEM here.
        treeView.setItem(&tvDispInfo->item);
    }
}



///////////////////////////////////////////////////////////////////////////////
// handle key down notification for TreeView control
// NMTVKEYDOWN consists of NMHDR, WORD(key) and UINT(flags).
///////////////////////////////////////////////////////////////////////////////
void View::tvKeyDown(NMTVKEYDOWN* tvKeyDown)
{
    if(tvKeyDown->hdr.hwndFrom == treeView.getHandle())
    {
    }

}



///////////////////////////////////////////////////////////////////////////////
// create drag image with TVM_CREATEDRAGIMAGE
///////////////////////////////////////////////////////////////////////////////
void View::tvBeginDrag(NMTREEVIEW* tv)
{
    if(tv->hdr.hwndFrom == treeView.getHandle())
    {
        // create dragging image using TVM_CREATEDRAGIMAGE
        // you have to delete it after drop operation
        this->tvDraggedItem = tv->itemNew.hItem;
        this->tvDraggedImageList = treeView.createDragImage(tvDraggedItem);

        //@@ NOT NEEDED
        // get bounding rectangle
        // NOTE: RECT should contains HTREEITEM of the dragged item that the rectangle is being retrieved for.
        //RECT rect;
        //*(HTREEITEM*)&rect = tv->itemNew.hItem;
        //treeView.getItemRect(&rect);

        // start dragging operation
        // PARAMS: HIMAGELIST, imageIndex, xHotspot, yHotspot
        ::ImageList_BeginDrag(tvDraggedImageList, 0, 0, 0);
        ::ImageList_DragEnter(treeView.getHandle(), tv->ptDrag.x, tv->ptDrag.y);

        // redirect mouse input to the parent window
        ::SetCapture(::GetParent(treeView.getHandle()));
        ::ShowCursor(false);

        tvItemDragged = true;
    }
}



///////////////////////////////////////////////////////////////////////////////
// dragging TreeView item
///////////////////////////////////////////////////////////////////////////////
void View::tvDragItem(HWND parentHandle, int x, int y)
{
    // convert the dialog coords to control coords
    POINT point;
    point.x = (SHORT)x;
    point.y = (SHORT)y;
    ::ClientToScreen(parentHandle, &point);
    ::ScreenToClient(treeView.getHandle(), &point);

    // drag the item to the current the cursor position
    ::ImageList_DragMove(point.x, point.y);

    // hide the dragged image, so the background can be refreshed
    ::ImageList_DragShowNolock(false);

    // find out if the pointer is on a item
    // If so, highlight the item as a drop target.
    TVHITTESTINFO hitTestInfo;
    hitTestInfo.pt.x = point.x;
    hitTestInfo.pt.y = point.y;
    HTREEITEM targetItem = treeView.hitTest(&hitTestInfo);
    if(targetItem)
    {
        treeView.selectItem(targetItem, TVGN_DROPHILITE);
    }

    // show the dragged image
    ::ImageList_DragShowNolock(true);
}



///////////////////////////////////////////////////////////////////////////////
// drop the item whem mouse button is up
///////////////////////////////////////////////////////////////////////////////
void View::tvDropItem()
{
    // move the item to the target
    HTREEITEM targetItem = treeView.getDropHilight();
    this->moveTreeViewItem(&treeView, tvDraggedItem, targetItem);

    // finish drag operation
    ::ImageList_EndDrag();
    ::ImageList_Destroy(tvDraggedImageList);
    ::ReleaseCapture();
    ::ShowCursor(true);

    // needed to avoid the target item is always highlighted
    treeView.selectItem(0, TVGN_DROPHILITE);

    // clear global variables
    tvDraggedItem = 0;
    tvDraggedImageList = 0;
    tvItemDragged = false;
}



///////////////////////////////////////////////////////////////////////////////
// helper for drag and drop a TreeView item to target item
///////////////////////////////////////////////////////////////////////////////
void View::moveTreeViewItem(TreeView* tv, HTREEITEM draggedItem, HTREEITEM targetItem)
{
    if(draggedItem == 0)
        return;

    // skip if targetItem is same as draggedItem
    if(draggedItem == targetItem)
        return;

    // skip if targetItem is same as parent
    if(tv->getParent(draggedItem) == targetItem)
        return;

    // root item cannot move
    if(draggedItem == tv->getRoot())
        return;

    // store the dragged item and its children into a list
    this->buildItemList(tv, draggedItem);

    // cannot move to its own child item
    for(size_t i = 0; i < items.size(); ++i)
    {
        if(items[i].handle == targetItem)
            return;
    }

    TreeItem treeItem;
    HTREEITEM oldItem = 0;
    HTREEITEM newItem = 0;

    // set the parent of the dragged item
    items[0].parentHandle = targetItem;

    // loop
    for(size_t i = 0; i < items.size(); ++i)
    {
        oldItem = items[i].handle;
        newItem = tv->insertItem(items[i].text.c_str(), items[i].parentHandle,
                                 items[i].previousHandle, items[i].imageIndex,
                                 items[i].selectedImageIndex);

        // update parent and previous handle in the list
        for(size_t j = i+1; j < items.size(); ++j)
        {
            if(items[j].parentHandle == oldItem)
                items[j].parentHandle = newItem;
            if(items[j].previousHandle == oldItem)
                items[j].previousHandle = newItem;
        }
    }

    // we have now the copy of dragged item
    // delete the dragged item in the previous position
    tv->deleteItem(draggedItem);

}



///////////////////////////////////////////////////////////////////////////////
// helper to get TVITEM data into TreeItem
///////////////////////////////////////////////////////////////////////////////
void View::getItemInfo(TreeView* tv, HTREEITEM item, TreeItem* treeItem)
{
    if(!tv || !treeItem)
        return;

    // string storage
    wchar_t str[MAX_STRING];

    // get TVITEM struct of item
    TVITEM tvItem;
    tvItem.hItem = item;
    tvItem.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT | TVIF_PARAM; // all attributes
    tvItem.pszText = str;
    tvItem.cchTextMax = MAX_STRING - 1;
    tv->getItem(&tvItem);

    // store info to TreeItem
    treeItem->text = str;
    treeItem->handle = item;
    treeItem->parentHandle = tv->getParent(item);
    treeItem->previousHandle = tv->getPrevious(item);
    treeItem->imageIndex = tvItem.iImage;
    treeItem->selectedImageIndex = tvItem.iSelectedImage;
}



///////////////////////////////////////////////////////////////////////////////
// store the item to be moved and its children to a container
///////////////////////////////////////////////////////////////////////////////
void View::buildItemList(TreeView* tv, HTREEITEM fromItem)
{
    // clear the container
    items.clear();

    if(!tv)
        return;

    TreeItem treeItem;

    // first, add the dragged item
    this->getItemInfo(tv, fromItem, &treeItem);
    items.push_back(treeItem);

    // add children if it has
    HTREEITEM firstChildItem = tv->getChild(fromItem);
    this->addChildren(tv, firstChildItem);
}



///////////////////////////////////////////////////////////////////////////////
// recursive method to add children items
///////////////////////////////////////////////////////////////////////////////
void View::addChildren(TreeView* tv, HTREEITEM firstChild)
{
    if(!tv)
        return;

    // check if no children
    if(!firstChild)
        return;

    TreeItem treeItem;
    HTREEITEM item = firstChild;    // start from the first child item

    do
    {
        this->getItemInfo(tv, item, &treeItem);
        items.push_back(treeItem);

        // add sub-children first before sibling
        this->addChildren(tv, tv->getChild(item));

        // next sibling
        item = tv->getNext(item);
    }
    while(item != 0);
}



///////////////////////////////////////////////////////////////////////////////
// show pop-up menu
// The cursor position is in screen coordinates. It needs to convert to client
// coords for TreeView hittest.
///////////////////////////////////////////////////////////////////////////////
void View::showPopupMenu(HWND handle, int x, int y)
{
    if(handle == treeView.getHandle())
    {
        // convert the cursor position to client coords
        POINT point;
        point.x = (SHORT)x;
        point.y = (SHORT)y;
        ::ScreenToClient(treeView.getHandle(), &point);

        // find out the item under the cursor
        TVHITTESTINFO hitTestInfo;
        hitTestInfo.pt.x = point.x;
        hitTestInfo.pt.y = point.y;
        HTREEITEM item = treeView.hitTest(&hitTestInfo);

        // load popup menu
        HMENU menu = ::LoadMenu(::GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU_TREEVIEW));
        HMENU subMenu = ::GetSubMenu(menu, 0);
        int command = ::TrackPopupMenu(subMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, x, y, 0, treeView.getParentHandle(), 0);
        if(command == ID_TREEVIEW_ADDITEM)
        {
            static int count = 5;
            wstringstream wss;
            wss << L"Child " << count++ << ends;

            treeView.insertItem((wchar_t*)wss.str().c_str(), item, TVI_LAST, 0, 1);
        }
        else if(command == ID_TREEVIEW_REMOVEITEM)
        {
            // delete if it is not root
            if(item != treeView.getRoot())
                treeView.deleteItem(item);
        }
        else if(command == ID_TREEVIEW_EXPAND)
        {
            treeView.expand(item, TVE_TOGGLE);
        }

        ::DestroyMenu(menu);
    }
}
