#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    this->travers = new int[bag.s];
    pos=0;
    inorder(bag.root);
    pos =0;
    first();

}
void SortedBagIterator::inorder(int root){
    if(root != -1){
        inorder(bag.left[root]);
        travers[pos] = bag.info[root];
        pos++;
        inorder(bag.right[root]);
    }
}

TComp SortedBagIterator::getCurrent() {
    if(!valid()){
        throw exception();
    }
    return travers[currentPosition];
}

bool SortedBagIterator::valid() {
	if(currentPosition < bag.s and currentPosition > -1){
        return true;
    }
	return false;
}

void SortedBagIterator::next() {
	if(!valid()){
        throw exception();
    }
    currentPosition++;
}

void SortedBagIterator::first() {
	this->currentPosition = 0;
}

