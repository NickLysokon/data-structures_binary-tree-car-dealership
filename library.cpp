#include "library.h"
#include "carDealershipManager.h"



void* Init()
{
	CarDealershipManager* DS = new CarDealershipManager();
	return (void*)DS;
}


StatusType AddCarType(void* DS, int typeID, int numOfModels)
{
	if (DS == nullptr|| typeID <= 0 || numOfModels <= 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->addCarType(typeID, numOfModels);
}


StatusType RemoveCarType(void* DS, int typeID)
{
	if (DS == nullptr || typeID <= 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->removeCarType(typeID);
}


StatusType SellCar(void* DS, int typeID, int modelID)
{
	if (DS == nullptr || typeID <= 0 || modelID < 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->sellCar(typeID, modelID);
}


StatusType MakeComplaint(void* DS, int typeID, int modelID, int t)
{
	if (DS == nullptr || typeID <= 0 || modelID < 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->makeComplaint(typeID, modelID, t);
}


StatusType GetBestSellerModelByType(void* DS, int typeID, int* modelID)
{
	if (DS == nullptr || typeID < 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->getBestSellerModelByType(typeID, modelID);
}


StatusType GetWorstModels(void* DS, int numOfModels, int* types, int* models)
{
	if (DS == nullptr || numOfModels <= 0) {
		return INVALID_INPUT;
	}
	return ((CarDealershipManager*)DS)->getWorstModels(numOfModels, types, models);
}


void Quit(void** DS)
{
	if (DS == nullptr || *DS == nullptr) {
		return;
	}

	delete ((CarDealershipManager*)(*DS));
	*DS = nullptr;
}