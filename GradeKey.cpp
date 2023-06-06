#include "GradeKey.h"

GradeKey::GradeKey(int type_id, int model_id, int grade): type(type_id), model(model_id), grade(grade) {}


bool GradeKey::operator<(const GradeKey& g)
{
	if (grade == g.grade) {

		if (type == g.type) {
			return model < g.model;
		}
		else {
			return type < g.type;
		}
	}
	else {
		return grade < g.grade;
	}
}


bool GradeKey::operator>(const GradeKey& g)
{
	if (grade == g.grade) {

		if (type == g.type) {
			return model > g.model;
		}
		else {
			return type > g.type;
		}
	}
	else {
		return grade > g.grade;
	}
}


bool GradeKey::operator==(const GradeKey& g)
{
	return (grade == g.grade) && (type == g.type) && (model == g.model);
}