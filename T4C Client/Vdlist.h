// VDList.h: interface for the VDList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VDLIST_H__B5F32863_E0F7_11D0_A876_00E029058623__INCLUDED_)
#define AFX_VDLIST_H__B5F32863_E0F7_11D0_A876_00E029058623__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ************************************************************************
// ***                                                                  ***
//      File Name: VDList.h
//      Project:   Virtual Dreams Library
//      Creation:  May 30th 1997
//      Author:    Benoit Thomas    (TH)
//                 Francois Leblanc (FL)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         05-30-1997      1.0     TH          Initial development
//         06-??-1997      1.1     FL          Encapsulated list in DLL
//         06-??-1997      1.1     FL          Added thread controls
//         06-26-1997      1.2     FL          Added position saving
//         06-26-1997      1.2     FL          Added floating list possibilities
//         06-26-1997      1.2     FL          Restructured TemplateList 
//         06-27-1997      1.3     FL          Adjusted QueryNext() and QueryPrevious() 
//         07-04-1997      1.4     FL          Fixed CreatePrevious() and CreateNext() 
//
//      Description
//				Provide Template List.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vritual Dreams (r). All rights reserved. ***

static const HardCopiedList = 1;
static const FloatingList   = 2;

#include "Lock.h"

namespace ServerList{

enum ListStatus { NO_QUERY, QUERY_NEXT, QUERY_PREVIOUS};

enum ListError { NO_MORE_NEXT, NO_MORE_PREVIOUS, 
                 NO_CURRENT_PREVIOUS, NO_CURRENT_NEXT, INVALID_LIST_TYPE, NO_CURRENT, BEFORE_HEAD, AFTER_TAIL, MIDDLE};

template <class ObjectType> class FrameList;
template <class ObjectType> class TemplateList;
template <class ObjectType> class ListIterator;

class ListException {
	public:
		ListException(ListError);
};

template <class ObjectType>
class TemplateObject {
	friend TemplateList<ObjectType>;

	private:
		TemplateObject *Greater;
		TemplateObject *Lesser;

	public:
		TemplateObject(void);		
		ObjectType     *Pointer;
};

template <class ObjectType>
class TemplateList : public CLock{

	private:
		TemplateObject<ObjectType> *Current;
		TemplateObject<ObjectType> *Head;
		TemplateObject<ObjectType> *Tail;
		TemplateObject<ObjectType> *TempObject;		
		TemplateObject<ObjectType> *SavedPos;
		char OutBound;
		int nb_objects;
		ListStatus SavedQuery;
		ListStatus Querying;
		//CRITICAL_SECTION csThreadLock;

	protected:
		void CreateNext(ObjectType *source);
		void CreatePrevious(ObjectType *source);
		void CreateHead(ObjectType *source);
		void CreateTail(ObjectType *source);

	public:
	
		TemplateList(void);
		~TemplateList(void);
		
		void ToHead(void);					// Position list at begining
		void ToTail(void);					// Position list at end 	

		void AddToNext    (ObjectType &);	// Adds an object after the current
		void AddToNext    (ObjectType *);
		void AddToPrevious(ObjectType &);	// Adds an object before the current
		void AddToPrevious(ObjectType *);
		void AddToHead    (ObjectType &);	// Adds an object at the head of the list
		void AddToHead    (ObjectType *);
		void AddToTail    (ObjectType &);	// Adds an object at the end (tail) of the list
		void AddToTail    (ObjectType *);

		BOOL MoveToTail();					// Places the current object at the end of the list
		BOOL MoveToHead();					// Places the current object at the beginning of the list

		void Remove(void);					// Removes the current object from the list
		void DeleteAbsolute(void);          // Deletes the current object, even in floating list

		ObjectType *Object();               // Returns the current pointed object

		BOOL QueryNext    (void);			// Checks and updates the position of the list, forward
		BOOL QueryPrevious(void);			// Checks and updates the position of the list, backwards
		
		//void Lock();						// Waits until a list is released, and sets it busy
		//void Unlock();						// Sets a controlled list to non-busy

		int NbObjects();

		void DestroyList();					// Removes all items from a list.
		void AnnihilateList();				// DeleteAbsolutes all items from a list.

		void PushList( void );				// Save current list position.
		void PopList( void );				// Restore list position.

		BOOL IsPointerInList( ObjectType *lpPointer );	// Returns TRUE if pointer was found in list.
        BOOL RemovePointer( ObjectType *lpPointer, BOOL boDestroy = FALSE );
};

template <class ObjectType>
TemplateObject<ObjectType>::TemplateObject(void) : Greater(0), Lesser(0) {
// TemplateObject Defautl Constructor.
// Initialize Greater and Lesser pointer to null.
}

template <class ObjectType>
TemplateList<ObjectType>::TemplateList() : Head(0), Tail(0), TempObject(0), SavedPos(0) {
// Frame List Default Constructor.
// Initialise Head, Tail and Current pointer to NULL.
	Querying = NO_QUERY;
	OutBound = MIDDLE;
	nb_objects = 0;	
}

template <class ObjectType>
TemplateList<ObjectType>::~TemplateList(void) {
// Frame List Default Destructor.
// Delete the whole List.
	ToHead();
	
	while (QueryNext()) 
		Remove();
}

// Removes the item pointed by lpPointer from the list.
template <class ObjectType>
BOOL TemplateList<ObjectType>::RemovePointer( ObjectType *lpPointer, BOOL boDestroy ){
    BOOL boRemoved = FALSE;

    Lock();
    ToHead();
    while( QueryNext() && !boRemoved ){
        if( Object() == lpPointer ){
            if( boDestroy ){
                DeleteAbsolute();
            }else{
                Remove();
            }
            boRemoved = TRUE;
        }
    }
    Unlock();

    return boRemoved;
}

template <class ObjectType>
void TemplateList<ObjectType>::PushList( void )
{
	SavedPos = Current;
	SavedQuery = Querying;
}

template <class ObjectType>
void TemplateList<ObjectType>::PopList( void )
{	
	if( SavedPos ){
		Current = SavedPos;
		Querying = SavedQuery;
	}
}

template <class ObjectType>
BOOL TemplateList<ObjectType>>::IsPointerInList( ObjectType *lpPnt ){
	BOOL boInList = FALSE;
	PushList();
	ToHead();
	while( QueryNext() && !boInList ){
		if( Object() == lpPnt ){
			boInList = TRUE;
		}
	}
	PopList();
	return boInList;
}

template <class ObjectType>
void TemplateList<ObjectType>::DestroyList()
{
	ToHead();
	while(QueryNext())
		Remove();
}

template <class ObjectType>
void TemplateList<ObjectType>::AnnihilateList()
{
	ToHead();
	while( QueryNext() ){
		DeleteAbsolute();
	}
}

template <class ObjectType>
int TemplateList<ObjectType>::NbObjects()
{return nb_objects;}


template <class ObjectType>
BOOL TemplateList <ObjectType>::MoveToTail()
{
#ifdef _DEBUG
/*	if(!Current)
	{
		throw ListException(NO_CURRENT);
	}	*/
#endif
	if(Current == Tail || !Current) return FALSE;

	AddToTail(Current->Pointer);
	
	Remove();

	return TRUE;
}

template <class ObjectType>
BOOL TemplateList <ObjectType>::MoveToHead()
{	
#ifdef _DEBUG
/*	if(!Current)
	{
		throw ListException(NO_CURRENT);
	}*/	
#endif
	if(Current == Head || !Current) return FALSE;

	AddToHead(Current->Pointer);
	
	Remove();

	return TRUE;
}


template <class ObjectType>
ObjectType *TemplateList <ObjectType>::Object()
{
#ifdef _DEBUG
/*	if(!Current)
	{
		throw ListException(NO_CURRENT);			
	}	*/
#endif
	return Current->Pointer;
}
/*
template <class ObjectType>
void TemplateList <ObjectType>::Lock()
{
	EnterCriticalSection(&csThreadLock);
}

template <class ObjectType>
void TemplateList <ObjectType>::Unlock()
{
	LeaveCriticalSection(&csThreadLock);
}
*/

template <class ObjectType>
void TemplateList<ObjectType>::ToHead(void) {
// Set Current ObjectType Pointer to Head of List.
	Current = Head;
	Querying = NO_QUERY;
	OutBound = MIDDLE;
}

template <class ObjectType>
void TemplateList<ObjectType>::ToTail(void) {
// Set Current ObjectType Pointer to Tail of List.
	Current = Tail;
	Querying = NO_QUERY;
	OutBound = MIDDLE;
}

template <class ObjectType>
void TemplateList<ObjectType>::CreateNext(ObjectType *Source) {
// Create an ObjectType Next to the Current ObjectType.

	TempObject = Current;
	
	if (!Current)		
	{
		if(OutBound == AFTER_TAIL) AddToTail(Source);	// No Current ObjectType, so create one..!
		else if(OutBound == BEFORE_HEAD) AddToTail(Source);	// No Current ObjectType, so create one..!
	}
	else 	
	{
		if(!Current->Greater) 
			CreateTail(Source);
		else{		
			nb_objects++;
			TempObject = new TemplateObject<ObjectType>;
			TempObject->Lesser = Current;
			TempObject->Greater = Current->Greater;
			TempObject->Greater->Lesser = Current->Greater = TempObject;		 

			if(Querying == QUERY_NEXT) Current = TempObject;

			TempObject->Pointer = Source;
		}
	}
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToNext(ObjectType &SourceObject) {
// Add an ObjectType Next to the Current ObjectType	
	CreateNext(&SourceObject);
	
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToNext(ObjectType *SourceObject) {
// Add an ObjectType Next to the Current ObjectType.		
	CreateNext(SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::CreatePrevious(ObjectType *Source) {
// Create an ObjectType Previous to the Current ObjectType.

	TempObject = Current;

	if (!Current)		
	{
		if(OutBound == AFTER_TAIL) AddToTail(Source);	// No Current ObjectType, so create one..!
		else if(OutBound == BEFORE_HEAD) AddToTail(Source);	// No Current ObjectType, so create one..!
	}
	else 
	{
		if(!Current->Lesser)
			CreateHead(Source);
		else{
			nb_objects++;
			TempObject = new TemplateObject<ObjectType>;
			TempObject->Lesser = Current->Lesser;
			TempObject->Greater = Current;
			TempObject->Lesser->Greater = Current->Lesser = TempObject;				
			
			if(Querying == QUERY_PREVIOUS) Current = TempObject;		

			TempObject->Pointer = Source;
		}
	}
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToPrevious(ObjectType &SourceObject) {
// Add an ObjectType Previous to the Current ObjectType.
	CreatePrevious(&SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToPrevious(ObjectType *SourceObject) {
// Add an ObjectType Previous to the Current ObjectType.
	CreatePrevious(SourceObject);	
}

template <class ObjectType>
void TemplateList<ObjectType>::CreateHead(ObjectType *Source) {
// Create an ObjectType at the Head of the List.
	ToHead();

	TempObject = Current;
	
	nb_objects++;

	if (!Current)
	{
		// No ObjectType in the list;
		Head = Current = Tail = new TemplateObject<ObjectType>;
		Current->Greater = 0;
		Current->Lesser = 0;
	}
	else {
		Current = Head = Current->Lesser = new TemplateObject<ObjectType>;
		Current->Greater = TempObject;
		Current->Lesser = 0;
	}


	Current->Pointer = Source;
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToHead(ObjectType &SourceObject) {
// Add an ObjectType to the Head of the List.
	CreateHead(&SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToHead(ObjectType *SourceObject) {
// Add an ObjectType to the Head of the List.
	CreateHead(SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::CreateTail(ObjectType *Source) {
// Create an ObjectType at the Tail of the List.
	ToTail();

	TempObject = Current;
	
	nb_objects++;

	if (!Current)
	// No ObjectType in the list;
	{	
		Head = Current = Tail = new TemplateObject<ObjectType>;
		Current->Greater = 0;
		Current->Lesser = 0;
	}
	else 
	{

		Current = Tail = Current->Greater = new TemplateObject<ObjectType>;
		Current->Lesser = TempObject;
		Current->Greater = 0;
	}

	Current->Pointer = Source;
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToTail(ObjectType &SourceObject) {
// Add an ObjectType to the Tail of the List.	
	CreateTail(&SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToTail(ObjectType *SourceObject) {
// Add an ObjectType to the Tail of the List.	
	CreateTail(SourceObject);
}

template <class ObjectType>
void TemplateList<ObjectType>::Remove(void) {
// Delete the Current ObjectType.
	TempObject = Current;
	nb_objects--;

	if (Current != Tail && Current != Head) {
	// Middle of the List.
		Current->Greater->Lesser = Current->Lesser;
		Current->Lesser->Greater = Current->Greater;
		if   (Querying == QUERY_NEXT)
			Current = Current->Lesser;
		else if (Querying == QUERY_PREVIOUS) 
			Current = Current->Greater;
	} else if (Current == Tail && Current != Head) {
	// At the Tail of the List
		Current->Lesser->Greater = 0;
		Tail = Current->Lesser;
		Current = Current->Lesser;
		if (Querying == QUERY_PREVIOUS)
			Querying = NO_QUERY;
	} else if (Current == Head && Current != Tail) {
	// At the Head of the List.
		Current->Greater->Lesser = 0;
		Head = Current = Current->Greater;
		if (Querying == QUERY_NEXT)
			Querying = NO_QUERY;
	} else {
	// At both Head and Tail of the List.
		Tail = Head = Current = 0;
		Querying = NO_QUERY;
	}		

	delete TempObject;
}

template <class ObjectType>
void TemplateList<ObjectType>::DeleteAbsolute(void) {
	TempObject = Current;
	nb_objects--;

	if (Current != Tail && Current != Head) {
	// Middle of the List.
		Current->Greater->Lesser = Current->Lesser;
		Current->Lesser->Greater = Current->Greater;
		if   (Querying == QUERY_NEXT)
			Current = Current->Lesser;
		else if (Querying == QUERY_PREVIOUS) 
			Current = Current->Greater;
	} else if (Current == Tail && Current != Head) {
	// At the Tail of the List
		Current->Lesser->Greater = 0;//Head
		Tail = Current->Lesser;
		Current = Current->Lesser;
		if (Querying == QUERY_PREVIOUS)
			Querying = NO_QUERY;
	} else if (Current == Head && Current != Tail) {
	// At the Head of the List.
		Current->Greater->Lesser = 0;//Tail;
		Head = Current = Current->Greater;
		if (Querying == QUERY_NEXT)
			Querying = NO_QUERY;
	} else {
	// At both Head and Tail of the List.
		Tail = Head = Current = 0;
		Querying = NO_QUERY;
	}		

	delete TempObject->Pointer;
	delete TempObject;
}


template <class ObjectType>
BOOL TemplateList<ObjectType>::QueryNext(void) {
// Query the next element in the list.
	if (Querying)
		Current = Current->Greater;
	else
		Querying = QUERY_NEXT;

	if (!Current)
	{						
		if(Querying) OutBound = AFTER_TAIL;
		Querying = NO_QUERY;
		return FALSE;
	}
return TRUE;
}

template <class ObjectType>
BOOL TemplateList<ObjectType>::QueryPrevious(void) {
// Query the next element in the list.
	if (Querying)
		Current = Current->Lesser;	
	else
		Querying = QUERY_PREVIOUS;

	if (!Current)
	{		
		if(Querying) OutBound = BEFORE_HEAD;		
		Querying = NO_QUERY;
		return FALSE;
	}
return TRUE;
}

}

#endif // !defined(AFX_VDLIST_H__B5F32863_E0F7_11D0_A876_00E029058623__INCLUDED_)
