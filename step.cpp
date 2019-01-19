#include"step.hpp"

#include<algorithm>
#include<cassert>

step make_fill_step(unsigned& wastage, bucket* b)
{
	return step{action::fill, &wastage, b};
}

step make_empty_step(unsigned& wastage, bucket* b)
{
	return step{action::empty, &wastage, b};
}

step make_transfer_step(unsigned& wastage, bucket* from, bucket* to)
{
	return step{action::transfer, &wastage, from, to};
}

step::step(const action act, unsigned* wastage, bucket* a, bucket* b)
	: step_action(act)
	, bucket1(a)
	, bucket2(b)
	, wastage(wastage)
{
}

bool step::can_apply() const
{
	switch (step_action) {
	case action::fill :
		return !bucket1->full();
	case action::empty :
		return !bucket1->empty();
	case action::transfer :
		return !bucket1->empty() && !bucket2->full();
	default:
		assert(false);
	}
	return false;
}

int step::apply()
{
	switch (step_action) {
	case action::fill :
		bucket1_used_before = bucket1->used;
		bucket1->used = bucket1->capacity;
		bucket1_used_after = bucket1->used;
		difference = bucket1_used_after - bucket1_used_before;
		break;
	case action::empty :
		bucket1_used_before = bucket1->used;
		bucket1->used = 0;
		bucket1_used_after = bucket1->used;
		difference = bucket1_used_after - bucket1_used_before;
		*wastage += -difference;
		break;
	case action::transfer :
		bucket1_used_before = bucket1->used;
		bucket2_used_before = bucket2->used;
		bucket2->used = std::min(bucket1->used + bucket2->used, bucket2->capacity);
		bucket1->used -= bucket2->used - bucket2_used_before;
		bucket1_used_after = bucket1->used;
		bucket2_used_after = bucket2->used;
		difference = 0;
		break;
	default :
		assert(false);
	}
	return difference;
}

int step::undo()
{
	bucket1->used = bucket1_used_before;
	if (bucket2)
		bucket2->used = bucket2_used_before;
	if (step_action == action::empty)
		*wastage += difference;
	return -difference;
}

std::ostream& operator<<(std::ostream& os, const step& s)
{
	switch (s.step_action) {
	case action::fill :
		os << "Fill bucket " << s.bucket1->name << " from ";
		os << s.bucket1_used_before << " to " << s.bucket1_used_after;
		break;
	case action::empty :
		os << "Empty bucket " << s.bucket1->name << " from ";
		os << s.bucket1_used_before << " to " << s.bucket1_used_after;
		break;
	case action::transfer :
		os << "Transfer bucket " << s.bucket1->name;
		os << " into " << s.bucket2->name << " reducing " << s.bucket1->name;
		os << " from " << s.bucket1_used_before << " to " << s.bucket1_used_after;
		os << " and increasing " << s.bucket2->name;
		os << " from " << s.bucket2_used_before << " to " << s.bucket2_used_after;
		break;
	default:
		assert(false);
	}
	return os;
}
