#ifndef _CAR_TYPE_H_
#define _CAR_TYPE_H_

#include "AvlTree.h"
#include "List.h" 
#include "GradeKey.h"
#include "SmartPtr.h"


class CarType {
	int id, models_amount, unsold_models_amount;

	SmartPtr<List<GradeKey> > unsold_list;
	ListNode<GradeKey>** unsold_models_ptrs; //Each cell holds a pointer to the model's location
											 //in the unsold_list list, cell becomes NULL if sold.

	int *models_sell_count, *model_grades;
	int best_model, best_model_count;

public:
	CarType();
	CarType(int type_id, int amount);
	~CarType();
	CarType(const CarType& ct);
	CarType& operator=(const CarType& ct);

	void sellModel(int model_id);
	void complain(int model_id, int time);

	int getId();
	int getModelAmount();
	int getUnsoldModelsAmount();
	int getBestModel();
	int getBestModelSellCount();
	int getModelGrade(int model_id);
	int getModelSellCount(int model_id);
	List<GradeKey>* getListPtr();
};

#endif /* CAR_TYPE_H */