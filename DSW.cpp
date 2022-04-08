#include "DSW.h"


bool SmartKey::operator >(const SmartKey& key) const {
    if (score > key.score)
        return true;
    if (score == key.score) {
        if (typeID > key.typeID)
            return true;
        if (typeID == key.typeID) {
            if (modelID > key.modelID)
                return true;
        }
    }
    return false;
}
bool SmartKey::operator <(const SmartKey& key) const {
    if (score < key.score)
        return true;
    if (score == key.score) {
        if (typeID < key.typeID)
            return true;
        if (typeID == key.typeID) {
            if (modelID < key.modelID)
                return true;
        }
    }
    return false;
}
bool SmartKey::operator ==(const SmartKey& key) const {
    if (score == key.score && typeID == key.typeID && modelID == key.modelID)
        return true;
    return false;
}


bool SKey::operator >(const SKey& key) const {
    if (score > key.score)
        return true;
    if (score == key.score) {
        if (typeID > key.typeID)
            return true;
        if (typeID == key.typeID) {
            if (modelID > key.modelID)
                return true;
        }
    }
    return false;
}
bool SKey::operator <(const SKey& key) const {
    if (score < key.score)
        return true;
    if (score == key.score) {
        if (typeID < key.typeID)
            return true;
        if (typeID == key.typeID) {
            if (modelID < key.modelID)
                return true;
        }
    }
    return false;
}
bool SKey::operator ==(const SKey& key) const {
    if (score == key.score && typeID == key.typeID && modelID == key.modelID)
        return true;
    return false;
}


int get_max(AVL<SKey,int> *SoldCars){
    Node<SKey,int> *tmp = SoldCars->root;
    int score = 0;
    while(tmp){
        if(tmp->right)
            tmp = tmp->right;
        else
            break;
    }
    if(tmp != NULL){
        score = tmp->key.modelID;
    }
    return score;
}

StatusType DSW::DSWAddCarType(int typeID, int numOfModels) {
    if (typeID <= 0 || numOfModels <= 0) {
        return INVALID_INPUT;
    }
    if (cars->findNode(typeID) != nullptr) {
        return FAILURE;
    }
    CarData* cd = new CarData(0,numOfModels);
    cd->models = new AVL<int, ModelData*>();
    for (int i = 0; i < numOfModels; i++) {
        ModelData* md = new ModelData();
        cd->models->insert(i, md);
        SmartKey newK(0,typeID,i);
        sortCars->insert(newK,0);
        cd->models->findNode(i)->data->nodePlace = sortCars->findNode(newK);
    }

    cars->insert(typeID, cd);
    return SUCCESS;
}


StatusType DSW::DSWRemoveCarType(int typeID) {
    if (typeID <= 0) {
        return INVALID_INPUT;
    }
    Node<int, CarData*>* car = cars->findNode(typeID);
    if (car == nullptr) {
        return FAILURE;
    }

    for (int i = 0;i < car->data->numOfModels;i++) {
        if (sortCars->root != nullptr) {

            int carScore = car->data->models->findNode(i)->data->nodePlace->key.score;
            SmartKey newK(carScore,typeID,i);
            sortCars->deleteNode(newK);
            int carSales = car->data->models->findNode(i)->data->sales;
            SKey newSK(carSales,typeID,i);
            SoldCars->deleteNode(newSK);
        }

        delete car->data->models->findNode(i)->data;
        car->data->models->deleteNode(i);
    }
    maxModelId = get_max(SoldCars);
    delete car->data->models;
    delete car->data;
    cars->deleteNode(typeID);

    return SUCCESS;
}


StatusType DSW::DSWSellCar(int typeID, int modelID) {
    if (typeID <= 0 || modelID < 0) {
        return INVALID_INPUT;
    }
    Node<int, CarData*>* car = cars->findNode(typeID);
    if (car == nullptr || car->data->numOfModels <= modelID) {
        return FAILURE;
    }
    /*update SoledCars tree*/
    int oldSales = cars->findNode(typeID)->data->models->findNode(modelID)->data->sales;
    SKey oldK(oldSales,typeID,modelID);
    SoldCars->deleteNode(oldK);
    oldK.score++;
    SoldCars->insert(oldK,0);
    /*update sortCars rating with + 10*/
    int temp = car->data->models->findNode(modelID)->data->nodePlace->key.score;
    sortCars->deleteNode(car->data->models->findNode(modelID)->data->nodePlace->key);
    SmartKey newKey(temp + 10, typeID, modelID);
    sortCars->insert(newKey, 0);
    car->data->models->findNode(modelID)->data->nodePlace = sortCars->findNode(newKey);
    /*update the sales*/
    car->data->models->findNode(modelID)->data->sales++;
    /*update max for this car*/
    if (car->data->models->findNode(modelID)->data->sales >= car->data->models->findNode(car->data->maxsales)->data->sales) {
        if (car->data->models->findNode(modelID)->data->sales == car->data->models->findNode(car->data->maxsales)->data->sales) {
            if (modelID < car->data->maxsales)
                car->data->maxsales = modelID;
        }
        else {
            car->data->maxsales = modelID;
        }
    }
    /*update the total max for this car*/
    maxModelId = get_max(SoldCars);
    return SUCCESS;
}

StatusType DSW::DSWMakeComplaint(int typeID, int modelID, int t) {
    if (typeID <= 0 || modelID < 0) {
        return INVALID_INPUT;
    }
    Node<int, CarData*>* car = cars->findNode(typeID);
    if (car == nullptr || car->data->numOfModels <= modelID) {
        return FAILURE;
    }

    int temp = car->data->models->findNode(modelID)->data->nodePlace->key.score;
    sortCars->deleteNode(car->data->models->findNode(modelID)->data->nodePlace->key);
    SmartKey newKey(temp - (int)(100/t), typeID, modelID);
    sortCars->insert(newKey, 0);
    car->data->models->findNode(modelID)->data->nodePlace = sortCars->findNode(newKey);
 
    return SUCCESS;
}

StatusType DSW::DSWGetBestSellerModelByType(int typeID, int* modelID) {
    if (typeID < 0 ) {
        return INVALID_INPUT;
    }
    if (typeID == 0 && cars->size == 0) {
        return FAILURE;
    }
    if(typeID == 0){
        *modelID = maxModelId;
        return SUCCESS;
    }
    Node<int, CarData*>* car = cars->findNode(typeID);
    if (car == nullptr ) {
        return FAILURE;
    }
    *modelID = car->data->maxsales;
    return SUCCESS;
}


void DSWInOrder(Node<SmartKey, int>* node, int* types, int* models, int* counter, int numOfModels) {
    if (node == nullptr || *counter == numOfModels) {
        return;
    }
    DSWInOrder(node->left, types, models, counter,numOfModels);
    models[*counter] = node->key.modelID;
    types[*counter] = node->key.typeID;
    (*counter)++;
    DSWInOrder(node->right, types, models, counter,numOfModels);
}

StatusType DSW::DSWGetWorstModels(int numOfModels, int* types, int* models) {
    if (numOfModels <= 0) {
        return INVALID_INPUT;
    }
    if (sortCars->size < numOfModels) {
        return FAILURE;
    }
    int ctr = 0;
    int* counter = &ctr;
    DSWInOrder(sortCars->root, types, models, counter, numOfModels);
    return SUCCESS;
}
void DSW::DSWQuit() {
    while (sortCars->root != nullptr) {
        sortCars->deleteNode(sortCars->root->key);
    }
    while (SoldCars->root != nullptr) {
        SoldCars->deleteNode(SoldCars->root->key);
    }
//    SoledCars->DestroyTree();
//    sortCars->DestroyTree();
    while (cars->root != nullptr) {
        while (cars->root->data->models->root != nullptr) {
            delete cars->root->data->models->root->data;
            cars->root->data->models->deleteNode(cars->root->data->models->root->key);
        }
      //  cars->root->data->models->DestroyTree();
        delete cars->root->data->models;
        delete cars->root->data;
        cars->deleteNode(cars->root->key);
    }
    //cars->DestroyTree();
    delete SoldCars;
    delete cars;
    delete sortCars;
}