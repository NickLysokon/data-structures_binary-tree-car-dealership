#ifndef _GRADE_KEY_H
#define _GRADE_KEY_H


class GradeKey {
public:
	int type, model, grade;

	GradeKey() = default;
	GradeKey(int type_id, int model_id, int grade);
	~GradeKey() = default;
	GradeKey(const GradeKey& g) = default;

	bool operator<(const GradeKey& g);
	bool operator>(const GradeKey& g);
	bool operator==(const GradeKey& g);
};


#endif /* GRADE_KEY_H */