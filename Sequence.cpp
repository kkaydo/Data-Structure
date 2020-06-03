#include <iostream>
#include <cmath>
#include <algorithm> 
#include "Sequence.h"

using namespace std;


//default constructor
//creates an default array
Sequence::Sequence()
{
	current_size = 0;
	max_size = 2;
	arr = new double[max_size];
}

//copy constructor
//creates a deep copy of Sequence reference parameter
Sequence::Sequence(const Sequence& sq)
{
	current_size = sq.current_size;
	max_size = sq.max_size;

	arr = new double[max_size];
	for (int i = 0; i < current_size; ++i) {
		arr[i] = sq.arr[i];
	}
}

//destructor 
//deallocates dynamic memory associated with the array pointer
Sequence::~Sequence()
{
	delete[] arr;
}

//overloaded assignment operator 
//copies its constant Sequence reference parameter to the calling object
Sequence& Sequence::operator=(const Sequence& sq)
{

	current_size = sq.current_size;
	max_size = sq.max_size;
	arr = new double[max_size];
	for (int i = 0; i < current_size; ++i) {
		arr[i] = sq.arr[i];
	}
	return *this;
}


//inserts a new value at the next available index
void Sequence::insert(double value)
{

	if (current_size < max_size) {
		arr[current_size] = value;
		current_size += 1;
	}
	//double the maximum size when the array is full
	else if (current_size == max_size) {
		max_size = 2 * max_size;

		//initialize a new array, copy the elements from original array
		double* new_arr = new double[max_size];
		for (int i=0; i < current_size; ++i) {
			new_arr[i] = arr[i];
		}

		//Delete the old array and assign the new array to original array
		delete[] arr;
		arr = new_arr;
		new_arr = nullptr;

		arr[current_size] = value;
		current_size += 1;
		
	}
}

//inserts the values which stored in another array
void Sequence::insert(double* add_arr, int add_size)
{
	int new_size = current_size + add_size;
	if (new_size <= max_size) {

		int start = current_size;      //mark down the index where new elements start to add on
		for (int i = current_size;i < new_size;++i) {
			arr[i] = add_arr[i - start];
			current_size += 1;
		}

	}
	//set the new size equal to the combined size of the parameter and the current contents
	//when the calling object's underlying array is not large enough
	else if (new_size > max_size) {
		max_size = new_size;
		int start = current_size;
		//initialize a new array
		double* new_arr = new double[max_size];
		//copy the elements of original array to the new array
		for (int i = 0;i < start;++i) {
			new_arr[i] = arr[i];
		}
		//pass new elements to the new array
		for (int i = start;i < new_size; ++i) {
			new_arr[i] = add_arr[i - start];
			current_size += 1;
		}

		//Delete the old array then assign the new array to original array
		delete[] arr;
		arr = new_arr;
		new_arr = nullptr;
	}
}

//find the number of sequence elements whose value is equal to the search value
int Sequence::find(double value)
{
	int count = 0;
	for (int i = 0;i < current_size;++i) {
		if (arr[i] == value) {
			count += 1;
		}
	}
	return count;
}

//check the size of the current size of the calling object
int Sequence::size()
{
	return current_size;
}

//sum of the values in the array
double Sequence::sum()
{
	double sum = 0;
	for (int i = 0;i < current_size;++i) {
		sum += arr[i];
	}

	return sum;
}


//average of the values in the array
double Sequence::mean()
{
	double mean = 0;
	double sum = 0;
	for (int i = 0;i < current_size;++i) {
		sum += arr[i];
	}
	mean = sum / current_size;

	return mean;
}

//median of the values of the array
double Sequence::median()
{
	double median;
	//make a new array which can be changed the order of elements
	double* new_arr = new double[current_size];
	for (int i = 0; i < current_size; ++i) {
		new_arr[i] = arr[i];
	}

	//sort by order
	sort(new_arr, new_arr + current_size);

	int r = current_size % 2;
	//when the size of array is even:
	if (r == 0) {
		int m1 = current_size / 2;
		int m2 = m1 - 1;
		//median equals average of the central two elements 
		median = (new_arr[m1] + new_arr[m2]) / 2;
	}
	//when the size of array is odd:
	else if (r != 0) {
		int m = current_size / 2;
		median = new_arr[m];
	}

	return median;
}

//standard deviation of the values in the sequence
double Sequence::stddev()
{
	double mean = 0;
	double sum = 0;
	double sd = 0;
	for (int i = 0;i < current_size;++i) {
		sum += arr[i];
	}
	mean = sum / current_size;

	sum = 0;
	for (int i = 0;i < current_size;++i) {
		sum += pow(arr[i]-mean,2);
	}
	sd = sqrt(sum / current_size);

	return sd;
}

//create a new Sequence object that contains the contents of the calling object 
//followed by the contents of its constant Sequence reference parameter
Sequence& Sequence::concatenate(const Sequence& sq)
{
	
	int new_size = current_size + sq.current_size;
	double* new_arr = arr;
	arr = new double[new_size];
	int mark_down = current_size;   //mark down the current size of the calling object

	for (int i = 0;i < mark_down;++i) {
		arr[i] = new_arr[i];
	}

	for (int i = mark_down;i < new_size;++i) {
		arr[i] = sq.arr[i - mark_down];
	}

	this->current_size = new_size;
	this->max_size = new_size;


	return *this;
}