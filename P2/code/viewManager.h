// viewManager.h: interface for the viewManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWMANAGER_H__3A163BCA_BA13_45EF_8056_34A91DC44FFD__INCLUDED_)
#define AFX_VIEWMANAGER_H__3A163BCA_BA13_45EF_8056_34A91DC44FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class viewManager  
{
protected:
	HANDLE mappingObject;  //the file-mapping object
	
	unsigned __int64 offset;  //the offset of mapping
	unsigned int charCounter;  //the offset of the current view
	unsigned char* view;  //the current map-view
	DWORD viewSize;  //the size of view
	unsigned __int64 maxFileSize;  //the file size
	DWORD offsetStep;  //the growth rate of the offset; offset grows only when we need to move to the next view

	DWORD desiredAccess;  //the desired access to map-view; see more at MSDN, MapViewOfFile

	DWORD savedViewSize;  //the actual view size of the current view
	unsigned __int64 savedOffset;  //a backup of file offset

	/* write the current view "lazily" to disk and move to the next view,
	   reset charCounter and update offset

	   return true only if it is logically able to move to the next view, not necessarily truly moved successfully

	   Note: If the moving actually failed, for simplicity we just wait for a crash to detect it since the function is protected.
	*/
	bool nextView();
public:
	/* hFileMappingObject: the file-mapping object
	   baseOffset: the offset in the file
	   maxFileSize: the file size
	   viewSize: the size of a map-view
	   dwDesiredAccess: the desired access to map-view; see more at MSDN, MapViewOfFile

	   Note: If you want to split one file into pieces and work on them separately, use the baseOffset and maxFileSize
	   to set the lower bound and higher bound and ensure that viewSize is not less than viewManager::GRANULARITY to guarantee
	   that the baseOffset can be any value less than maxFileSize.
	   If you want to split one file or one piece of file into several views and work on them sequentially, please
	   ensure that viewSize is not less than viewManager::GRANULARITY to support view change by nextView().
	   Don't worry about too large viewSize that cause an overflow of maxFileSize. It'll be cut down to appropriate size.
	   But ensure that viewSize is acceptable for the memory of current process.
	*/
	viewManager(HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD viewSize = GRANULARITY,
		DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);

	virtual ~viewManager();

	/* close the view, after which the viewManager shouldn't be used again before calling setOffset(unsigned __int64) successfully
	*/
	virtual bool close();

	/* baseOffset: the new desired offset in the file
	
	   return: true only if the operation was successful

	   Note: Offset should be less than maxFileSize previously set.
	   It reset the base offset thus can change view even if nextView() cannot.
	   If it failed not because of too large baseOffset, no more operations should be done until it succeeds. Same to rollback().
	   Call GetLastError() to get details when it failed with other reason.
	*/
	virtual bool setOffset(unsigned __int64 baseOffset);

	//get the current offset
	virtual unsigned __int64 getOffset() const;

	//save the current file offset in savedOffset
	virtual void mark();

	//reset the file offset to savedOffset
	virtual bool rollback();

	//return the actual view size
	virtual DWORD getViewSize() const { return viewSize;}

	//allocation granularity of the system
	static unsigned int GRANULARITY;
};

#endif // !defined(AFX_VIEWMANAGER_H__3A163BCA_BA13_45EF_8056_34A91DC44FFD__INCLUDED_)
