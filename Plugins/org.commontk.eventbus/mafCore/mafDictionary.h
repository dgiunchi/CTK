/*
 *  mafDictionary.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */
#include "mafCore_global.h"
#include "mafObjectBase.h"

#ifndef MAFDICTIONARY_H
#define MAFDICTIONARY_H

namespace mafCore {

// Class forwarding list

/**
 Class name: mafDictionary
 This class defines the MAF3 dictionary.
 @sa mafHierarchy
 */
class MAFCORESHARED_EXPORT mafDictionary : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafDictionary(const mafString code_location = "");

    /// Object destructor.
    virtual ~mafDictionary();

    /// Redefined operator to have access to the entries owned.
    mafDictionaryEntries *entries();

    /// Redefined operator to have access to the entries owned.
    mafDictionaryEntries *entries() const;

    /// Overload operator for rapid access to mafDictionaryEntries
    mafVariant &operator[](mafString key) const;


private:
    mafDictionaryEntries *m_Entries;
};

    typedef mafDictionary* mafDictionaryPointer;

} // namespace mafCore

Q_DECLARE_METATYPE(mafCore::mafDictionaryPointer);

#endif // MAFDICTIONARY_H
