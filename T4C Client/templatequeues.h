// Implements template queues for general uses

#pragma warning( disable : 4291 )

#ifndef __TEMPLATE_QUEUES
#define __TEMPLATE_QUEUES

//#define DEBUG_NEW	 new    (__FILE__, __LINE__)
//#define new			DEBUG_NEW

#ifndef NULL
 #define NULL 0
#endif

template <class T> class TemplateQueue;

template <class T>
class TemplateQueueObject{
      friend TemplateQueue<T>;

public:
      TemplateQueueObject();
      ~TemplateQueueObject();

      TemplateQueueObject *Next;
      T *Pointer;
};

template <class T>
class TemplateQueue{
public:
      TemplateQueue();
      ~TemplateQueue();

      T *ViewNext();
      T *ViewNextNext();

      void AddToQueue(T *NewObject);
	   DWORD NbObjects();


      T *Retreive(BOOL = TRUE);

private:
      DWORD nb_obj;
	  TemplateQueueObject<T> *Current;
      TemplateQueueObject<T> *QueueEnd;
};

////////////////////////////////////////////////////////////////////////////
// Constructor
template <class T>
TemplateQueueObject<T>::TemplateQueueObject() : Next(NULL), Pointer(NULL){
}

////////////////////////////////////////////////////////////////////////////
// Destructor
template <class T>
TemplateQueueObject<T>::~TemplateQueueObject(){
}

template <class T>
DWORD TemplateQueue<T>::NbObjects(){
	return nb_obj;
} 

////////////////////////////////////////////////////////////////////////////
// Constructor
template <class T>
TemplateQueue<T>::TemplateQueue() : Current(NULL), QueueEnd(NULL) {
	nb_obj = 0;
}
////////////////////////////////////////////////////////////////////////////
// Destructor
template <class T>
TemplateQueue<T>::~TemplateQueue(){
// Retreives all objects from the queue. Since queues are floating, it will
// not delete the object pointers
	T *Obj = NULL;
	
	Obj = Retreive();
	while(Obj) {
		delete Obj;
		Obj = Retreive();
	};
}

////////////////////////////////////////////////////////////////////////////
// Returns the current object (next to be retreived) without retreiving it
template <class T>
T *TemplateQueue<T>::ViewNext(){
  if(Current)
     return Current->Pointer;

return NULL;
}
////////////////////////////////////////////////////////////////////////////
// Returns the next object (after the current)
template <class T>
T *TemplateQueue<T>::ViewNextNext(){
  if(Current)
     if(Current->Next)
         return Current->Next->Pointer;

return NULL;
}
////////////////////////////////////////////////////////////////////////////
// Add an object to the queue
template <class T>
void TemplateQueue<T>::AddToQueue(T *object){
	nb_obj++;
	// Creates a new object pointing to the object to be queued
	TemplateQueueObject<T> *NewObj = new TemplateQueueObject<T>;
	NewObj->Pointer = object;  // point to added object
	NewObj->Next = NULL;       // will become the end of the queue

	// If there is at least one object in the queue
   if(QueueEnd){
     // Make the last object now point to the new one
     QueueEnd->Next = NewObj;
     // Then make the new object the end of the queue
     QueueEnd = NewObj;
   }else{
     // Else create a new 'queue'
     QueueEnd = NewObj;
     Current  = NewObj;
   }
}


////////////////////////////////////////////////////////////////////////////
// Retreives (removes from the queue) the next object
template <class T>
	T *TemplateQueue<T>::Retreive(BOOL Remove){
	T *ReturnObj = NULL;
	
	// If queue is empty, return NULL
	if(!Current) return NULL;

	// Else well, return the current object :)
	ReturnObj = Current->Pointer;

   if (!Remove)
   // If Object is not remove from the queue.
      return ReturnObj;

   // If there is only one object in the queue
	if(Current == QueueEnd){
		 delete Current;  // delete the current object
		Current  = NULL;
		QueueEnd = NULL;
	}else{
	// Well, if the queue has more then one object..
		 // Creates a temp object to later delete it
		 TemplateQueueObject<T> *TmpObj;
		TmpObj  = Current;
		// cannot be NULL, since we already tested for end-of-queue
		Current = Current->Next;
		delete TmpObj;
	}

   nb_obj--;
   return ReturnObj;
}

#undef new
#undef DEBUG_NEW

#endif