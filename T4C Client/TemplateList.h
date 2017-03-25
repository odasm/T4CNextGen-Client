// ********************************************************************************************
// ***                                                                                      ***
//      File Name: VDList.h
//      Project:   TFC Client
//      Creation:  May 30th 1997
//      Author:    Benoit Thomas    (TH)
//                 Francois Leblanc (FL)
// ***                                                                                      ***
// ********************************************************************************************
// ***                                                                                      ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         05-30-1997      1.0     TH          Initial development
//         06-01-1997      1.1     FL          Encapsulated list in DLL
//         06-01-1997      1.1     FL          Added thread controls
//         06-26-1997      1.2     FL          Added position saving
//         06-26-1997      1.2     FL          Added floating list possibilities
//         06-26-1997      1.2     FL          Restructured TemplateList 
//         06-27-1997      1.3     FL          Adjusted QueryNext() and QueryPrevious() 
//         07-04-1997      1.4     FL          Fixed CreatePrevious() and CreateNext() 
//			  11-18-1997		1.5     TH          A Little Clean Up.
//
//      Description
//				Provide Template List.
// ***                                                                                      ***
//	********************************************************************************************
// *** Copyright (c) 1997-1998 Vircom (r). All rights reserved.                             ***

#pragma warning( disable : 4291 )

#ifndef __Virtual_Dreams_TemplateList_075200111897
#define __Virtual_Dreams_TemplateList_075200111897           

//#define DEBUG_NEW	 new    (__FILE__, __LINE__)
//#define new			DEBUG_NEW

enum ListStatus { NO_QUERY, QUERY_NEXT, QUERY_PREVIOUS, STOP_QUERY};

enum ListError { NO_MORE_NEXT, NO_MORE_PREVIOUS, 
                 NO_CURRENT_PREVIOUS, NO_CURRENT_NEXT, INVALID_LIST_TYPE, NO_CURRENT, BEFORE_HEAD, AFTER_TAIL, MIDDLE};

template <class ObjectType> class __declspec(dllexport) TemplateList;

class __declspec(dllexport) ListException {
	public:
		ListException(ListError);
};

template <class ObjectType>
class TemplateObject {
	public:
		TemplateObject *Greater;
		TemplateObject *Lesser;

		TemplateObject(void);
		ObjectType     *Pointer;
};

template <class ObjectType>
class __declspec(dllexport) TemplateList {

	private:
		int nb_objects;
		char OutBound;
		ListStatus SavedQuery;

	protected:
		TemplateObject<ObjectType> *Current;
		TemplateObject<ObjectType> *Head;
		TemplateObject<ObjectType> *Tail;
		TemplateObject<ObjectType> *TempObject;
		TemplateObject<ObjectType> *SavedPos;
		ListStatus Querying;

		CRITICAL_SECTION csThreadLock;

	public:
		TemplateList(void);
		~TemplateList(void);
		
		void ToHead(void);					// Position list at begining
		void ToTail(void);					// Position list at end 	

		void AddToNext    (ObjectType *);
		void AddToPrevious(ObjectType *);
		void AddToHead    (ObjectType *);
		void AddToTail    (ObjectType *);

		BOOL MoveToTail();					// Places the current object at the end of the list
		BOOL MoveToHead();					// Places the current object at the beginning of the list

		void RemoveObject(void);			// Removes the Current Object from the list
		void DeleteObject(void);			// Removes and Delete the Current Objects

		ObjectType *GetObject();          // Returns the current pointed object

		BOOL QueryNext    (void);			// Checks and updates the position of the list, forward
		BOOL QueryPrevious(void);			// Checks and updates the position of the list, backwards

		void PushList( void );				// Save current list position.
		void PopList( void );				// Restore list position.		

		void Lock(char *);							// Waits until a list is released, and sets it busy
		void Unlock(char *);							// Sets a controlled list to non-busy

		int  GetNbObjects();
		void SetQueryState(ListStatus);

		void DestroyList();
};

template <class ObjectType>
TemplateObject<ObjectType>::TemplateObject(void) : Greater(0), Lesser(0) {
// TemplateObject Defautl Constructor.
// Initialize Greater and Lesser pointer to null.
}

template <class ObjectType>
TemplateList<ObjectType>::TemplateList() : Head(0), Tail(0), TempObject(0) {
// TemplateList Default Constructor.
// Initialise Head, Tail and Current pointer to NULL.
	InitializeCriticalSection(&csThreadLock);
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
		RemoveObject();

	DeleteCriticalSection(&csThreadLock);
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
void TemplateList<ObjectType>::DestroyList()
{
	ToHead();
	while(QueryNext())
		DeleteObject();
}

template <class ObjectType>
int TemplateList<ObjectType>::GetNbObjects()
{
	return nb_objects;
}

template <class ObjectType>
BOOL TemplateList <ObjectType>::MoveToTail()
{
	return FALSE;
}

template <class ObjectType>
BOOL TemplateList <ObjectType>::MoveToHead()
{	
	return FALSE;
}

template <class ObjectType>
ObjectType *TemplateList <ObjectType>::GetObject()
{
 return Current->Pointer;//a voir ici  ptet un bug ?
}

template <class ObjectType>
void TemplateList <ObjectType>::Lock(char *s)
{
/*   if (s) {
      OutputDebugString("Locking [");
      OutputDebugString(s);
      OutputDebugString("]");
   }*/
	EnterCriticalSection(&csThreadLock);
/*   if (s) {
	   OutputDebugString (" Success!\r\n");
   }*/
}

template <class ObjectType>
void TemplateList <ObjectType>::Unlock(char *s)
{
/*   if (s) {
      OutputDebugString("Unlocking [");
      OutputDebugString(s);
      OutputDebugString("]");
   }*/
	LeaveCriticalSection(&csThreadLock);
/*   if (s) {
      OutputDebugString (" Success!\r\n");
   }*/
}

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
void TemplateList<ObjectType>::SetQueryState(ListStatus ls) {
// Set the QueryStatus.
	Querying = ls;
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToNext(ObjectType *SourceObject) {
// Add an ObjectType Next to the Current ObjectType.		
	TempObject = Current;
	
	if (!Current)		
	{
		if      (OutBound == AFTER_TAIL) AddToTail(Source);	// No Current ObjectType, so create one..!
		else if (OutBound == BEFORE_HEAD) AddToHead(Source);	// No Current ObjectType, so create one..!
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

			TempObject->Pointer = SourceObject;
		}
	}
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToPrevious(ObjectType *SourceObject) {
// Add an ObjectType Previous to the Current ObjectType.
	TempObject = Current;

	if (!Current)		
	{
		if		  (OutBound == AFTER_TAIL)  AddToTail(SourceObject);	// No Current ObjectType, so create one..!
		else if (OutBound == BEFORE_HEAD) AddToHead(SourceObject);	// No Current ObjectType, so create one..!
	}
	else 
	{
		if(!Current->Lesser)
			CreateHead(SourceObject);
		else{
			nb_objects++;
			TempObject = new TemplateObject<ObjectType>;
			TempObject->Lesser = Current->Lesser;
			TempObject->Greater = Current;
			TempObject->Lesser->Greater = Current->Lesser = TempObject;				
			
			if(Querying == QUERY_PREVIOUS) Current = TempObject;		

			TempObject->Pointer = SourceObject;
		}
	}
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToHead(ObjectType *SourceObject) {
// Add an ObjectType to the Head of the List.
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

	Current->Pointer = SourceObject;
}

template <class ObjectType>
void TemplateList<ObjectType>::AddToTail(ObjectType *SourceObject) {
// Add an ObjectType to the Tail of the List.	
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

	Current->Pointer = SourceObject;
}

template <class ObjectType>
void TemplateList<ObjectType>::RemoveObject(void) {
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
void TemplateList<ObjectType>::DeleteObject(void) {
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
   {
		if (Querying != STOP_QUERY) 
      {
         if(Current)
			   Current = Current->Greater;
         else
            Current = NULL;

		} 
      else 
      {
			return FALSE;
		}
	} 
   else 
   {
		Querying = QUERY_NEXT;
	}

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
	if (Querying) {
		if (Querying != STOP_QUERY) {
			Current = Current->Lesser;	
		} else {
			return FALSE;
		}
	} else {
		Querying = QUERY_PREVIOUS;
	}

	if (!Current)
	{		
		if(Querying) OutBound = BEFORE_HEAD;		
		Querying = NO_QUERY;
		return FALSE;
	}

	return TRUE;
}

#undef new
#undef DEBUG_NEW

#endif
