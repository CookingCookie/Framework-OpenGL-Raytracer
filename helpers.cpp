#include "helpers.h"

helpers::helpers() {

}

helpers::~helpers() {

}

float helpers::eukl(Vec3f vec) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}