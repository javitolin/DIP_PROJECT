#ifndef _MANUSCRIPT_EXPORT_H_
#define _MANUSCRIPT_EXPORT_H_


#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

//#ifdef MANUSCRIPT_LIB
//# define MANUSCRIPT_EXPORT DllExport
//#else
//# define MANUSCRIPT_EXPORT DllImport
//#endif

#define MANUSCRIPT_EXPORT DllExport
typedef char byte ;
#endif // _MANUSCRIPT_EXPORT_H_
