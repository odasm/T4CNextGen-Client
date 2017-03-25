#define SetRectangle(Rect, Left, Top, Right, Bottom) \
   Rect->left   = Left; \
   Rect->top    = Top; \
   Rect->right  = Right; \
   Rect->bottom = Bottom;

#define CreateComponent(Object, Type) \
   Object = new Type; \
   ZeroMemory(Object, sizeof(Type));

#define DeleteComponent(Object) \
   if (Object) delete Object; Object = NULL;

#define Bit(B) \
   (1 << B)

#define SizeMemory(Object) \
   ZeroMemory((Object), sizeof((*Object))); \
	(Object)->dwSize = sizeof((*Object));

#define QWORD int