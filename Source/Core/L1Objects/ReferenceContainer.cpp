/**
 * @file ReferenceContainer.cpp
 * @brief Source file for class ReferenceContainer
 * @date 12/08/2015
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceContainerItem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainer::ReferenceContainer() :
        Object() {
    if (mux.Create()) {
        msecTimeout = TTInfiniteWait;
    }
}

/*lint -e{1551} no exception should be thrown given that ReferenceContainer is
 * the sole owner of the list (LinkedListHolder)*/
ReferenceContainer::~ReferenceContainer() {
    LinkedListable *p = list.List();
    list.Reset();
    while (p != NULL) {
        LinkedListable *q = p;
        p = p->Next();
        delete q;
    }
}

bool ReferenceContainer::Insert(Reference ref,
                                const int32 &position) {
    bool ok = true;
    ReferenceContainerItem *newItem = new ReferenceContainerItem();
    if (newItem->Load(ref)) {
        if (position == -1) {
            list.ListAdd(newItem);
        }
        else {
            list.ListInsert(newItem, static_cast<uint32>(position));
        }
    }
    else {
        delete newItem;
        ok = false;
    }

    return ok;
}


class ReferenceFilter {

public:

}

class searchMode{
    int status;
    bool withPath;
    bool recursive;

public:
    SetSingle(int position, bool withPath,bool recursive){
        status = position;
        this->withPath = withPath;
    }
    SetLast(bool recursive){
        status = -1;
        this->withPath = false;
    }
    SetMultiple(bool recursive){
        status = -2;
        this->withPath = false;
    }

    bool ShallStore(){
        if ((status == -2) || (status == -1) || (status == -0)) return true;
        return false;
    }

    void OneInstanceFound(){
        if (status <= 0) return ;

        status--;

    }

    bool ShallContinue(){
        if ((status == -2) || (status == -1)) return true;
        if (status <= 0) return false;
        return (status >= 1);
    }

    bool ShallRecurse();
};


bool ReferenceContainer::Find (ReferenceContainer &result, ReferenceFilter &rf, searchMode &sm ){

    do {

    LinkedListable *ll = list.Peek(index);
    if (ll == NULL) ok = false;

    ReferenceContainerItem *rci;

    if (ok){
        rci = dynamic_cast<Reference *> (ll);

        if (rci == NUL) ok = false;
    }

    if (ok) {
        bool found = rf.Check(rci->GetReference());


        if (found){
            sm.OneInstanceFound();
            if (!sm.ShallDelete()){  // update last
                result.Remove(all);
            }

            if (!sm.ShallStore()){
                result.Add(rci->GetReference());
            }

        } else {
            if ((isContainer(rci)) && sm.ShallRecurse()){
                if (sm.ShallStorePath()){
                    result.Add(rci->GetReference());
                 }
                 rf.StartRecurse(container(rci))
                 container(rci).Find(result,rf,sm);
                 rf.EndRecurse(container(rci))

                 if (sm.ShallStorePath()){
                    // not found - prune store path
                    if (sm.ShallContinue()){
                        result.Remove(rci->GetReference());
                    }
                 }
            }

         }



        }






    } while(ok && sm.ShallContinue());





}
































bool GCRCFind(GCReference &reference,
              uint32 index,
              bool remove,
              bool recurse) {

    LinkedListable *ll = NULL;

    // if the request is to get a specific element then just do it
    if ((name == NULL) && (selector == NULL)) {

        // unlocks automatically on exit from function
        // just lock for this code block
        MuxLock muxLock;
        if (!muxLock.Lock(gcrc.mux, gcrc.msecTimeout)) {
            gcrc.AssertErrorCondition(Timeout, "GCRCFind: timeout on resource sharing ");
            return False;
        }

        // get the LinkedListable;
        if (remove) {
            ll = gcrc.list.ListExtract(index);
        }
        else {
            ll = gcrc.list.ListPeek(index);
        }
        if (ll == NULL) {
            gcrc.AssertErrorCondition(FatalError, "GCRCFind(%i): no element in the list fits criteria", index);
            return False;
        }

        // check type
        GCRCItem *gcri = dynamic_cast<GCRCItem *>(ll);
        if (gcri == NULL) {
            gcrc.AssertErrorCondition(FatalError, "GCRCFind(): non GCRCItem derived element in the list");

            if (remove)
                delete ll;
            return False;
        }

        if (recurse && (gcri->Link() != NULL)) {
            GCRTemplate < GCNOExtender<BString> > gcrtgcnobs(GCFT_Create);
            BString *bs = gcrtgcnobs.operator->();
            (*bs) = gcri->Link();
            gc = gcrtgcnobs;
        }
        else {
            // retrieve reference
            gc = gcri->Reference();
        }
        if (remove)
            delete ll;
        return True;
    }
}

CLASS_REGISTER(ReferenceContainer, "1.0")
