#include "carDealershipManager.h"

static const int UNINITIALIZED = -1;

CarDealershipManager::CarDealershipManager(): worst_model(nullptr), best_seller(0),
								  			  best_seller_count(0), best_seller_type(UNINITIALIZED),
										      total_model_amount(0){}


StatusType CarDealershipManager::addCarType(int type_id, int models_amount)
{
	try {
		//Insert the new type to the types tree - O(log(n))
		car_types.insert(type_id, CarType(type_id, models_amount));

		//Insert the new models to the unsold_models lists tree - O(log(n))
		TreeNode<CarType, int>* node = car_types.findNode(type_id);
		unsold_models.insert(type_id, node->data.getListPtr());
		left_most_unsold_list = unsold_models.getMinNode();

		//Insert a model to the best selling models tree - O(log(n))
		car_types_best_sellers.insert(SaleKey(type_id, 0), 0);
		total_model_amount += models_amount;
	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}
	catch (AvlTree<CarType, int>::REPEATED_NODE& ex) {
		return FAILURE;
	}

	return SUCCESS;
}


StatusType CarDealershipManager::removeCarType(int type_id)
{
	try {
		//Check if the type exists in the system - O(log(n))
		TreeNode<CarType, int>* node = car_types.findNode(type_id);
		if (node == nullptr) {
			return FAILURE;
		}

		int len = node->data.getModelAmount();
		//Remove from unsold_models pointer list - O(log(n))
		unsold_models.remove(type_id);
		left_most_unsold_list = unsold_models.getMinNode();

		//Remove each sold model from model_grades tree - O(m * log(M))
		for (int i = 0; i < len; i++) {
			model_grades.remove(GradeKey(node->data.getId(), i, node->data.getModelGrade(i)));
		}

		//Remove best seller from best seller tree - O(log(n))
		car_types_best_sellers.remove(SaleKey(type_id, node->data.getBestModelSellCount()));
		total_model_amount -= node->data.getModelAmount();

		//Update best selling models tree and the worst model pointer - O(log(n) + log(m))
		updateBestSeller();
		updateWorstModel();

		//Remove the type from the types tree - O(log(n))
		car_types.remove(type_id);
	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}


	return SUCCESS;
}


StatusType CarDealershipManager::sellCar(int type_id, int model_id)
{
	try {
		//Check if the type and model exist in the system - O(log(n))
		TreeNode<CarType, int>* node = car_types.findNode(type_id);
		if (node == nullptr || model_id >= node->data.getModelAmount()) {
			return FAILURE;
		}

		int old_best_sales_count = node->data.getBestModelSellCount();

		//Chceks if the model has been sold before, if so, removes the old grade from models_grades - O(log(m))
		if (node->data.getModelSellCount(model_id) > 0) {
			model_grades.remove(GradeKey(type_id, model_id, node->data.getModelGrade(model_id)));
		}
		//Increases model's grade, sale count, local best seller and removes it from unsold_list - O(log(n))
		node->data.sellModel(model_id);

		//Remove list from unsold_models tree - O(log(n))
		if (node->data.getUnsoldModelsAmount() == 0) {
			unsold_models.remove(type_id);
			left_most_unsold_list = unsold_models.getMinNode();
		}

		//Adds the new model grade to model_grades tree - O(log(m))
		model_grades.insert(GradeKey(type_id, model_id, node->data.getModelGrade(model_id)), model_id);

		//Check and update best seller tree and worst_model - O(log(n) + log(m))
		if (node->data.getBestModel() == model_id) {
			car_types_best_sellers.remove(SaleKey(type_id, old_best_sales_count));
			car_types_best_sellers.insert(SaleKey(type_id, node->data.getBestModelSellCount()), model_id);
			updateBestSeller();
		}
		updateWorstModel();
	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}

	return SUCCESS;
}


StatusType CarDealershipManager::makeComplaint(int type_id, int model_id, int time)
{
	try {
		//Check if the type and model exist in the system - O(log(n))
		TreeNode<CarType, int>* node = car_types.findNode(type_id);
		if (node == nullptr || model_id >= node->data.getModelAmount()) {
			return FAILURE;
		}

		//Change the model's grade in the model_grades tree, and update worst model - O(log(m))
		model_grades.remove(GradeKey(type_id, model_id, node->data.getModelGrade(model_id)));
		node->data.complain(model_id, time);
		model_grades.insert(GradeKey(type_id, model_id, node->data.getModelGrade(model_id)), model_id);
		updateWorstModel();

	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}

	return SUCCESS;
}


StatusType CarDealershipManager::getBestSellerModelByType(int type_id, int* model_id)
{
	try {
		if (type_id == 0) {
			if (car_types.getRoot() == nullptr) { //Empty tree
				return FAILURE;
			}
			*model_id = best_seller;
		}
		else {
			TreeNode<CarType, int>* node = car_types.findNode(type_id);
			if (node == nullptr) {
				return FAILURE;
			}
			*model_id = node->data.getBestModel();
		}

	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}

	return SUCCESS;
}


StatusType CarDealershipManager::getWorstModels(int amount, int* types, int* models)
{
	try {
		if (amount > total_model_amount) {
			return FAILURE;
		}

		TreeNode<int, GradeKey>* curr = worst_model;

		//First we take all models with a negative grade
		while (curr != nullptr && amount > 0 && curr->key.grade < 0) {
			*(types++) = curr->key.type;
			*(models++) = curr->key.model;
			amount--;
			curr = model_grades.getInOrderNext(curr);
		}

		if (amount == 0) {
			return SUCCESS;
		}

		//Now all model grades are >= 0
		getGradeZeroModels(&amount, &types, &models, &curr);

		while (curr != nullptr && amount > 0) {
			*(types++) = curr->key.type;
			*(models++) = curr->key.model;
			amount--;
			curr = model_grades.getInOrderNext(curr);
		}
	}
	catch (std::bad_alloc& ex) {
		return ALLOCATION_ERROR;
	}

	return (amount == 0) ? SUCCESS : FAILURE;
}




/* -----------------------------------------------------------------------------
						
						Helper Functions Implementation

   ----------------------------------------------------------------------------- */



void CarDealershipManager::updateBestSeller()
{
	TreeNode<int, SaleKey>* ptr = car_types_best_sellers.getMaxNode();

	if (ptr == nullptr) { //Empty tree
		best_seller = 0;
		best_seller_count = 0;
		best_seller_type = UNINITIALIZED;
		return;
	}

	best_seller = ptr->data;
	best_seller_count = ptr->key.sale_count;
	best_seller_type = ptr->key.type;
}


void CarDealershipManager::updateWorstModel()
{
	worst_model = model_grades.getMinNode();
}


ListNode<GradeKey>* CarDealershipManager::getNextUnsoldModel(TreeNode<List<GradeKey>*, int>** ptr, ListNode<GradeKey>* iter)
{
	if (iter == (*ptr)->data->getEnd()) {
		*ptr = unsold_models.getInOrderNext(*ptr); //Updates tree list ptr in the main function
		if ((*ptr) == nullptr) { //We reached the end
			return nullptr;
		}
		return (*ptr)->data->getHead();
	}
	else {
		return iter->next;
	}
}


/*
getGradeZeroModels: Inserts all of the unsold models to the given arrays, while maintaining 
					amount >= 0 and updating the given model_grades tree iterator (curr) 

@param amount - A pointer the amount of empty spaces on the arrays.
@param types - The user given types array.
@param models - The user given models array.
@param curr - A model_grades tree iterator.

@return SUCCESS - The unsold models were all successfully inserted (amount >= 0).
*/
StatusType CarDealershipManager::getGradeZeroModels(int* amount, int** types, int** models, TreeNode<int, GradeKey>** curr)
{
	TreeNode<List<GradeKey>*, int>* ptr = left_most_unsold_list; //Pointer the first unsold models list

	if (ptr != nullptr) { //The unsold_models tree isn't empty
		ListNode<GradeKey>* iter = ptr->data->getHead();

		while ((*amount) > 0 && *curr != nullptr && (*curr)->key.grade == 0 && iter != nullptr) {
			if ((*curr)->key < iter->data) {
				*((*types)++) = (*curr)->key.type;
				*((*models)++) = (*curr)->key.model;
				(*curr) = model_grades.getInOrderNext((*curr));
			}
			else {
				*((*types)++) = iter->data.type;
				*((*models)++) = iter->data.model;
				iter = getNextUnsoldModel(&ptr, iter);
			}
			(*amount)--;
		}

		//Now either Amount = 0 OR curr = null OR curr.grade > 0 OR iter = null
		while ((*amount) > 0 && iter != nullptr) {
			*((*types)++) = iter->data.type;
			*((*models)++) = iter->data.model;
			iter = getNextUnsoldModel(&ptr, iter);
			(*amount)--;
		}
		return SUCCESS;

	}
	else {
		return SUCCESS;
	}
}