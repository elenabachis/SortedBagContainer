#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <algorithm>

SortedBag::SortedBag(Relation r) {
    this->cap = 6;
    this->s = 0;
    this->root = -1;
    this->relation = r;
    this->firstEmpty = 0;
    this->info = new TComp[cap]();
    this->left = new int[cap]();
    this->right = new int[cap]();

    for (int i = 0; i < cap-1; i++) {
        this->info[i] = -1;
        this->left[i] = -1;
        this->right[i] = i+1;
    }

    this->right[cap-1] = -1;

}

int SortedBag::allocateNode() {
    if (firstEmpty == -1) {
        resize(cap * 2);
    }

    int newNode = firstEmpty;
    firstEmpty = right[firstEmpty];

    return newNode;
}

void SortedBag::resize(int newcap) {
    int* newInfo = new int[newcap];
    int* newLeft = new int[newcap];
    int* newRight = new int[newcap];

    for (int i = 0; i < cap; i++) {
        newInfo[i] = info[i];
        newLeft[i] = left[i];
        newRight[i] = right[i];
    }

    for (int i = cap; i < newcap; i++) {
        newInfo[i] = -1;
        newLeft[i] = -1;
        newRight[i] = i + 1;
    }
    newRight[newcap - 1] = -1;

    delete[] info;
    delete[] left;
    delete[] right;

    info = newInfo;
    left = newLeft;
    right = newRight;
    firstEmpty = cap;
    cap = newcap;
}

///Komplexitat: best case: theta(1) wenn es das root ist,worst case:theta(n), wenn ich nur am Ende des BSTs addieren konnte und average O(logn) da ich suche in diesem Baum
void SortedBag::add(TComp e) {
    if (s == cap) {
        resize(cap * 2);
    }

    int newNode = allocateNode();
    info[newNode] = e;
    left[newNode] = -1;
    right[newNode] = -1;

    //wenn das Bag leer ist
    if (s == 0) {
        root = newNode;
    }

    else {
        int current = root;
        int parent = -1;

        while (current != -1) { //ich suche wo ich das Element addieren kann
            parent = current;
            if (relation(e, info[current])) {
                current = left[current];
            } else {
                current = right[current];
            }
        }
        if (relation(e, info[parent])) {
            left[parent] = newNode;
        } else {
            right[parent] = newNode;
        }
    }

    s++;
}

///Komplexitat: best case: theta(1) wenn das Bag nur e enthalt,worst case:theta(n), wenn ich nur am Ende des BSTs losche und average O(logn) da ich suche in diesem Baum
bool SortedBag::remove(TComp e) {
    
    bool ok = false;

    //wenn man das erste Element loscht
    if(s==1 and info[root]==e){
        info[root] = -1;
        right[root] = firstEmpty;
        left[root] = -1;
        firstEmpty = root;
        root= -1;
        s--;
        return true;
    }

    int current = root;
    int parent = -1;
    while (current != -1 && !ok) {
        if (info[current] == e) {
            ok = true;
        } else { //hier sucht man das Element
            parent = current;
            if (relation(e, info[current])) {
                current = left[current];
            } else {
                current = right[current];
            }
        }
    }

    if (!ok) { //es gibt kein e in dem Bag
        return false;
    }

    //wenn das Element hat keine linken Kinder
    if(left[current]==-1){
        if(current== root){ //wenn ich das root losche
            root = current[right];
            info[current] = -1;
        }
        else if(left[parent] == current){ //ich linke das rechte Element der current zu ihrem Vater
            left[parent] = right[current];
            info[current] = -1;
        }
        else{ //wenn right[parent]=current das rechte Kind der current wird zum Vater verbindet 
            right[parent] = right[current];
            info[current] = -1;
        }

        //in dem Platz des rechten Kind wird nichts sein
        right[current] = firstEmpty; 
        firstEmpty = current;
        s--;
        return true;
    }

    int toRemove = current;
    parent = current;
    current = left[parent];
    if(right[current] == -1){ //wenn current hat nur ein linkes Kind
        left[toRemove] = left[current];
        info[toRemove] = info[current];

        info[current] = -1;
        left[current] = -1;

        right[current] = firstEmpty;
        firstEmpty = current;
        s--;
        return true;
    }

    //wenn das Element hat sowohl ein rechtes Kind, als auch ein linkes Kind
    while(right[current]!=-1){ //ich suche das minimale Wert, er ist am Ende dieses teil
        parent = current;
        current = right[parent];
    }

    info[toRemove] = info[current];
    right[parent] = left[current];

    info[current] = -1;
    right[current] = firstEmpty;
    firstEmpty = current;
    s--;
    return true;
}

///Komplexitat: average case: O(logn) , da es elem im BST sucht,worst case:theta(n), wenn ich nur am Ende des BSTs losche und best theta(1) wenn es das root ist
bool SortedBag::search(TComp elem) const {
    int current = root;
    bool ok = false;
    while (current != -1 && !ok) {
        if (info[current] == elem) {
            ok = true;
        } else if (relation(info[current], elem)) {
            current = right[current];
        } else {
            current = left[current];
        }
    }
	return ok;
}

///Komplexitat: average case=worst case=theta(n), da es das ganze BST durchfuhren muss, um alle Vorkommen zu losen und
/// best case:theta(1) wenn das Bag leer ist oder wenn es nur elem enthalt
int SortedBag::nrOccurrences(TComp elem) const {
    int count = 0;
    int current = root;
    TComp element;

    while (current != -1) {
        element = info[current];
        if (info[current] == elem) {
            count++;
        }

        if (relation(elem, info[current])) {
            current = left[current];
        } else {
            current = right[current];
        }
    }

    return count;
}



int SortedBag::size() const {
	return s;
}


bool SortedBag::isEmpty() const {
	if(s==0){
        return true;
    }
	return false;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}



SortedBag SortedBag::intersection(const SortedBag& other) const {
    SortedBag result(relation);
    int index1 = 0;
    int index2 = 0;
    int resultSize = 0;
    int resultCapacity = std::min(s, other.s);
    TComp* resultInfo = new TComp[resultCapacity]();
    int* resultLeft = new int[resultCapacity]();
    int* resultRight = new int[resultCapacity]();


    while (index1 < s && index2 < other.s) {
        TComp current1 = info[index1];
        TComp current2 = other.info[index2];

        if (current1 == current2) { //wenn die Elementen gleich sind, addiere ich diese
            resultInfo[resultSize] = current1;
            resultLeft[resultSize] = -1;
            resultRight[resultSize] = -1;
            resultSize++;
            index1++;
            index2++;
        } else if (relation(current1, current2)) {
            index1++;
        } else {
            index2++;
        }
    }

    //ich habe die neue Daten der result gestellt
    delete[] result.info;
    delete[] result.left;
    delete[] result.right;
    result.info = resultInfo;
    result.left = resultLeft;
    result.right = resultRight;
    result.s = resultSize;
    result.cap = resultCapacity;
    result.firstEmpty = -1;
    result.root = -1;

    return result;
}


SortedBag::~SortedBag() {
	delete[] info;
    delete[] right;
    delete[] left;
}
