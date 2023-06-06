#ifndef _CAR_DEALERSHIP_MANAGER_H
#define _CAR_DEALERSHIP_MANAGER_H

#include "library.h"
#include "AvlTree.h"
#include "List.h"
#include "CarType.h"
#include "GradeKey.h"
#include "SaleKey.h"


class CarDealershipManager {
	AvlTree<CarType, int> car_types;
	AvlTree<int, GradeKey> model_grades;
	AvlTree<List<GradeKey>*, int> unsold_models;

	TreeNode<List<GradeKey>*, int>* left_most_unsold_list;
	TreeNode<int, GradeKey>* worst_model;
	int best_seller, best_seller_count, best_seller_type;
	AvlTree<int, SaleKey> car_types_best_sellers;
	int total_model_amount;

	//Helper Functions:
	void updateBestSeller();
	void updateWorstModel();
	StatusType getGradeZeroModels(int* amount, int** types, int** models, 
								  TreeNode<int, GradeKey>** curr);
	ListNode<GradeKey>* getNextUnsoldModel(TreeNode<List<GradeKey>*, int>** ptr,
										   ListNode<GradeKey>* iter);
	void fixUnsoldList(TreeNode<CarType, int>* type_node);

public:
	CarDealershipManager();
	~CarDealershipManager() = default;
	CarDealershipManager(const CarDealershipManager& cdm) = default;

	StatusType addCarType(int type_id, int models_amount);
	StatusType removeCarType(int type_id);
	StatusType sellCar(int type_id, int model_id);
	StatusType makeComplaint(int type_id, int model_id, int time);
	StatusType getBestSellerModelByType(int type_id, int* model_id);
	StatusType getWorstModels(int amount, int* types, int* models);
};


#endif /* CAR_DEALERSHIP_MANAGER_H */