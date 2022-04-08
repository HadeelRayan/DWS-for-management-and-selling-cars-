#include "DSW.h"

void* Init() {
    DSW* DS = new DSW();
    return  (void*)DS;
}
StatusType AddCarType(void* DS, int typeID, int numOfModels) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWAddCarType(typeID, numOfModels);
}
StatusType RemoveCarType(void* DS, int typeID) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWRemoveCarType(typeID);
}
StatusType SellCar(void* DS, int typeID, int modelID) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWSellCar(typeID, modelID);
}
StatusType MakeComplaint(void* DS, int typeID, int modelID, int t) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWMakeComplaint(typeID, modelID, t);
}
StatusType GetBestSellerModelByType(void* DS, int typeID, int* modelID) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWGetBestSellerModelByType(typeID, modelID);
}
StatusType GetWorstModels(void* DS, int numOfModels, int* types, int* models) {
    if (DS == nullptr)return INVALID_INPUT;
    return ((DSW*)DS)->DSWGetWorstModels(numOfModels, types, models);
}
void Quit(void** DS) {
    if (DS == NULL || *DS == NULL) return;
    ((DSW*)*DS)->DSWQuit();
    delete ((DSW*)*DS);
    *DS = NULL;
    //delete DS;

}