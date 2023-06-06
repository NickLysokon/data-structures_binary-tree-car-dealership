#ifndef _SALE_KEY_H
#define _SALE_KEY_H


class SaleKey {
public:
	int type, sale_count;

	SaleKey() = default;
	SaleKey(int type_id, int sales);
	~SaleKey() = default;
	SaleKey(const SaleKey& key) = default;

	bool operator<(const SaleKey& key);
	bool operator>(const SaleKey& key);
	bool operator==(const SaleKey& key);
};


#endif /* SALE_KEY_H */