/*
 *  mafDictionary.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDictionary.h"
#include "mafObjectFactory.h"

using namespace mafCore;

mafDictionary::mafDictionary(const mafString code_location): mafObjectBase(code_location), m_Entries(NULL) {
    m_Entries = new mafDictionaryEntries();
}

mafDictionary::~mafDictionary() {
    // Clean up the memory allocated for the dictionary items.
    m_Entries->clear();
    mafDEL(m_Entries);
}

mafDictionaryEntries *mafDictionary::entries() {
    return m_Entries;
}

mafDictionaryEntries *mafDictionary::entries() const {
    return m_Entries;
}

mafVariant &mafDictionary::operator[](mafString key) const{
    return (*m_Entries)[key];
}
