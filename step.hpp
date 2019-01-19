#ifndef step_hpp
#define step_hpp

#include"bucket.hpp"

#include<ostream>

enum class action { fill, empty, transfer };

class step;

step make_fill_step(unsigned& wastage, bucket*);
step make_transfer_step(unsigned& wastage, bucket* from, bucket* to);
step make_empty_step(unsigned& wastage, bucket*);

std::ostream& operator<<(std::ostream&, const step&);

class step
{
public:
	bool can_apply() const;
	int apply(); // returns difference in measurement
	// only call undo() on same step instance that apply() was last called
	int undo();  // returns difference in measurement

private:
	step(const action, unsigned*, bucket*, bucket* = nullptr);

	action step_action;
	bucket* bucket1;
	bucket* bucket2;
	unsigned* wastage;
	int bucket1_used_before = 0;
	int bucket2_used_before = 0;
	int bucket1_used_after = 0;
	int bucket2_used_after = 0;
	int difference = 0;

	friend step make_fill_step(unsigned&, bucket*);
	friend step make_transfer_step(unsigned&, bucket*, bucket*);
	friend step make_empty_step(unsigned&, bucket*);
	friend std::ostream& operator<<(std::ostream&, const step&);
};

#endif
