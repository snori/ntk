#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__


#include <ntk/support/observable.h>
#include <ntk/storage/path.h>


class Document : public NObservable {
public:
	//
	// constants
	//
	enum {
		MODIFICATION_FLAG_CHANGED = 'DCMD',
		PATH_CHANGED              = 'PTHC',
	};

	//
	// methods
	//
	Document();
	Document(const NPath& path);
	virtual ~Document();

	bool is_modified() const {return m_modified;}
	void set_modification_flag(bool flag);

	const NPath& path() const {return m_path;}
	void set_path(const NPath& path);

	bool has_path() const {return m_path.init_check();}

private:
	//
	// data
	//
	bool m_modified;
	NPath m_path;

};// class Document


#endif//EOH