// viewManager.cpp: implementation of the viewManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "viewManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static unsigned int getGranularity() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwAllocationGranularity;
}

unsigned int viewManager::GRANULARITY = getGranularity();

viewManager::viewManager(HANDLE hFileMappingObject,
		unsigned __int64 baseOffset,
		unsigned __int64 maxFileSize,
		DWORD _viewSize,
		DWORD dwDesiredAccess)
:mappingObject(hFileMappingObject)
,maxFileSize(maxFileSize)
,desiredAccess(dwDesiredAccess)
,savedOffset(baseOffset) {
	charCounter = (unsigned int)(baseOffset%GRANULARITY);
	offset = baseOffset - charCounter;
	offsetStep = _viewSize - _viewSize%GRANULARITY;
	viewSize = offsetStep?offsetStep:_viewSize;
	view = (unsigned char*) MapViewOfFile(mappingObject,
		desiredAccess,
		(DWORD)(offset>>32), (DWORD)(offset&0xFFFFFFFF),
		savedViewSize = (DWORD)((maxFileSize - offset >= viewSize)?viewSize:(maxFileSize - offset)));
}

inline viewManager::~viewManager() {UnmapViewOfFile(view);}

bool viewManager::setOffset(unsigned __int64 baseOffset) {
	if (baseOffset >= maxFileSize) return false;
	charCounter = (unsigned int)(baseOffset%GRANULARITY);
	if (!view || offset != baseOffset - charCounter) {
		offset = baseOffset - charCounter;
		return (!view || UnmapViewOfFile(view)) &&
		(view = (unsigned char*) MapViewOfFile(mappingObject,
			desiredAccess,
			(DWORD)(offset>>32), (DWORD)(offset&0xFFFFFFFF),
			savedViewSize = (DWORD)((maxFileSize - offset >= viewSize)?viewSize:(maxFileSize - offset))));
	}
	return true;
}

unsigned __int64 viewManager::getOffset() const {
	return offset + charCounter;
}

bool viewManager::nextView() {
	UnmapViewOfFile(view);
	if (!offsetStep || maxFileSize - offset <= offsetStep) {
		view = NULL;
		return false;
	}
	offset += offsetStep;
	charCounter = 0;
	view = (unsigned char*) MapViewOfFile(mappingObject,
		desiredAccess,
		(DWORD)(offset>>32), (DWORD)(offset&0xFFFFFFFF),
		savedViewSize = (DWORD)((maxFileSize - offset >= viewSize)?viewSize:(maxFileSize - offset)));
	return true;
}

bool viewManager::close() {
	return !view || (UnmapViewOfFile(view) && !(view=NULL));
}

void viewManager::mark() {
	savedOffset = offset + charCounter;
}

bool viewManager::rollback() {
	return setOffset(savedOffset);
}

