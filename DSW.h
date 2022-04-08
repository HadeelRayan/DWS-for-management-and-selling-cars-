
#ifndef MIVNI1_DSW_H
#define MIVNI1_DSW_H

#include "library.h"
#include "AVLTree.h"

class SmartKey {
public:
    int score;
    int typeID;
    int modelID;
    SmartKey(int score, int typeID, int modelID) : score(score), typeID(typeID), modelID(modelID) {}
    bool operator <(const SmartKey& key) const;
    bool  operator >(const SmartKey& key) const;
    bool  operator ==(const SmartKey& key) const;
};

class SKey {
public:
    int score;
    int typeID;
    int modelID;
    SKey(int score, int typeID, int modelID) : score(score), typeID(typeID), modelID(modelID) {}
    bool operator <(const SKey& key) const;
    bool  operator >(const SKey& key) const;
    bool  operator ==(const SKey& key) const;
};

class ModelData {
public:
    Node<SmartKey, int>* nodePlace;
    int sales;
    ModelData() : nodePlace(NULL), sales(0){}
    ~ModelData() = default;
};



class CarData {
public:
    AVL<int, ModelData*>* models;
    int maxsales;
    int numOfModels;
    CarData(int maxsales, int numOfModels) : maxsales(maxsales), numOfModels(numOfModels){
       // AVL<int, ModelData*>* models = new AVL<int, ModelData*>();
    }
    ~CarData() = default;
};


class DSW {
public:
    AVL<int, CarData*>* cars;
    AVL<SmartKey, int>* sortCars;
    AVL<SKey, int>* SoldCars;
    int maxModelId;

    DSW() {
        cars = new AVL<int, CarData*>();
        sortCars = new AVL<SmartKey, int>();
        SoldCars = new AVL<SKey, int>();
        maxModelId = 0;
    }


    ~DSW() = default;
    StatusType DSWAddCarType(int typeID, int numOfModels);
    StatusType DSWRemoveCarType(int typeID);
    StatusType DSWSellCar(int typeID, int modelID);
    StatusType DSWMakeComplaint(int typeID, int modelID, int t);
    StatusType DSWGetBestSellerModelByType(int typeID, int* modelID);
    StatusType DSWGetWorstModels(int numOfModels, int* types, int* models);
    void DSWQuit();
};



#endif //MIVNI1_DSW_H