#ifndef _IMAGELIST_H_
#define _IMAGELIST_H_

#include "Image.h"
#include <list>

// Wraps a list of images which is used to play animations.
class CImageList
{
public:
	// Default constructor: construct an empty list.
	CImageList();
	// Copy constructor: copies the content of the
	// list passed in argument.
	CImageList(const CImageList& other);
	// Default destructor.
	~CImageList();

	// Assignement operator: empty the current content
	// and copies the content of the list passed in argument.
	CImageList& operator= (const CImageList& other);

	// Empty the content of the list
	void Clear();
	// Append a new image to the list
	void AppendImage(TImagePtr pImage);
	// Return the number of images in this list
	unsigned GetImagesCount() const;

	// Make the first image active
	void GoToFirstImage();
	// Make the next image active. If the last image
	// was active, we go back to the first image. In
	// that case, the function returns true.
	bool GoToNextImage();
	// Get the current image
	TImagePtr GetCurrentImage() const;

private:
	// Typedef for a std::list containing TImagePtr objects
	typedef std::list<TImagePtr> TImageList;
	// The list of images
	TImageList m_lstImages;

	// Iterator pointing to the current image
	TImageList::iterator m_iterCurrentImg;
};

#endif  // _IMAGELIST_H_