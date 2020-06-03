#pragma once

class Sequence
{
public:
	Sequence();

	Sequence(const Sequence& sq);
	~Sequence();
	Sequence& operator=(const Sequence& sq);
	Sequence& concatenate(const Sequence& sq);

	void insert(double value);
	void insert(double* add_arr, int add_size);
	int find(double value);
	int size();
	double sum();
	double mean();
	double median();
	double stddev();
	

private:
	double* arr;
	int current_size;
	int max_size;
};
