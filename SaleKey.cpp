#include "SaleKey.h"


SaleKey::SaleKey(int type_id, int sales): type(type_id), sale_count(sales) {}


bool SaleKey::operator<(const SaleKey& key)
{
	if (sale_count == key.sale_count) {
		return type > key.type;			//A bigger type has a smaller priority
	}
	else {
		return sale_count < key.sale_count;
	}
}


bool SaleKey::operator>(const SaleKey& key)
{
	if (sale_count == key.sale_count) {
		return  type < key.type;
	}
	else {
		return sale_count > key.sale_count;
	}
}


bool SaleKey::operator==(const SaleKey& key)
{
	return (sale_count == key.sale_count) && (type == key.type);
}