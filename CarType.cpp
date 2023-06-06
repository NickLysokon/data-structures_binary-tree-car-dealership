#include "CarType.h"

static const int MAX_COMPLAINT = 100;

CarType::CarType() : unsold_models_ptrs(nullptr), models_sell_count(nullptr),
					 model_grades(nullptr)
{

}


CarType::CarType(int type_id, int amount):  unsold_list(new List<GradeKey>),
											unsold_models_ptrs(new ListNode<GradeKey>*[amount]),
											models_sell_count(new int[amount]), model_grades(new int[amount])
{
	for (int i = 0; i < amount; i++)
	{
		unsold_models_ptrs[i] = unsold_list->insertLast(GradeKey(type_id, i, 0));
		models_sell_count[i] = 0;
		model_grades[i] = 0;
	}

	id = type_id;
	models_amount = amount;
	unsold_models_amount = amount;
	best_model = 0;
	best_model_count = 0;
	
}


CarType::~CarType()
{
	delete[] unsold_models_ptrs;
	delete[] models_sell_count;
	delete[] model_grades;
}


CarType::CarType(const CarType& ct): unsold_list(ct.unsold_list),
									 unsold_models_ptrs(new ListNode<GradeKey>* [ct.models_amount]),
									 models_sell_count(new int[ct.models_amount]),
									 model_grades(new int[ct.models_amount])
{
	ListNode<GradeKey>* iter = unsold_list->getHead();

	for (int i = 0; i < ct.models_amount; i++)
	{
		unsold_models_ptrs[i] = ct.unsold_models_ptrs[i];
		models_sell_count[i] = ct.models_sell_count[i];
		model_grades[i] = ct.model_grades[i];
		iter = iter->next;
	}

	id = ct.id;
	models_amount = ct.models_amount;
	unsold_models_amount = ct.unsold_models_amount;
	best_model = ct.best_model;
	best_model_count = ct.best_model_count;
}


CarType& CarType::operator=(const CarType& ct)
{
	if (this == &ct) {
		return *this;
	}

	//Delete old array data
	delete[] unsold_models_ptrs;
	delete[] models_sell_count;
	delete[] model_grades;

	unsold_models_ptrs = new ListNode<GradeKey>*[ct.models_amount];
	models_sell_count = new int[ct.models_amount];
	model_grades = new int[ct.models_amount];
	unsold_list = ct.unsold_list;  //Smart pointer operator=

	for (int i = 0; i < ct.models_amount; i++)
	{
		unsold_models_ptrs[i] = ct.unsold_models_ptrs[i];
		models_sell_count[i] = ct.models_sell_count[i];
		model_grades[i] = ct.model_grades[i];
	}

	id = ct.id;
	models_amount = ct.models_amount;
	unsold_models_amount = ct.unsold_models_amount;
	best_model = ct.best_model;
	best_model_count = ct.best_model_count;

	return *this;
}


void CarType::sellModel(int model_id)
{
	models_sell_count[model_id]++;

	//Update local best seller, if sale counts are equal, choose the lower numbered model
	if (models_sell_count[model_id] > best_model_count || 
		(models_sell_count[model_id] == best_model_count && model_id < best_model)) {
		best_model = model_id;
		best_model_count = models_sell_count[model_id];
	}

	//Remove from unsold list
	if (unsold_models_ptrs[model_id] != nullptr) {
		unsold_list->removeNode(unsold_models_ptrs[model_id]);
		unsold_models_ptrs[model_id] = nullptr;
		unsold_models_amount--;
	}

	model_grades[model_id] += 10;
}


void CarType::complain(int model_id, int time)
{
	model_grades[model_id] -= (MAX_COMPLAINT/time);
}


int CarType::getId()
{
	return id;
}

int CarType::getModelAmount()
{
	return models_amount;
}

int CarType::getUnsoldModelsAmount()
{
	return unsold_models_amount;
}

int CarType::getBestModel()
{
	return best_model;
}

int CarType::getBestModelSellCount()
{
	return best_model_count;
}

int CarType::getModelGrade(int model_id)
{
	return model_grades[model_id];
}

int CarType::getModelSellCount(int model_id)
{
	return models_sell_count[model_id];
}

List<GradeKey>* CarType::getListPtr()
{
	return unsold_list.operator->();
}
