/*
 * functional.h
 *
 *  Created on: Mar 27, 2014
 *      Author: vlad
 */

#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

#include <stdexcept>
#include <cmath>
#include "types.h"
#include "internal.h"

// Matrix functional calculus

namespace qpp
{

// Computes f(A), where (*f) is the function pointer
template<typename Derived>
Eigen::MatrixXcd funm(const Eigen::MatrixBase<Derived> &A,
		types::cplx (*f)(const types::cplx &))
{
	// check square matrix
	if (!internal::_check_square_mat(A))
		throw std::runtime_error("funm: Matrix must be square!");

	Eigen::ComplexEigenSolver<Eigen::MatrixXcd> es(
			A.template cast<types::cplx>());
	Eigen::MatrixXcd evects = es.eigenvectors();
	Eigen::MatrixXcd evals = es.eigenvalues();
	for (int i = 0; i < evals.rows(); i++)
		evals(i) = (*f)(evals(i)); // apply f(x) to each eigenvalue

	Eigen::MatrixXcd evalsdiag = evals.asDiagonal();

	return evects * evalsdiag * evects.inverse();
}

// Apply f(A) component-wise, where (*f) is the function pointer
template<typename FunctionInputType, typename FunctionOutputType,
		typename MatrixInputType>
Eigen::Matrix<FunctionOutputType, Eigen::Dynamic, Eigen::Dynamic> fun(
		const Eigen::MatrixBase<MatrixInputType> &A,
		FunctionOutputType (*f)(const FunctionInputType &))
// The type of A is MatrixInputType
// The function is of the form FunctionOutputType f(const FunctionInputType &)
// The output is an Eigen::Matrix of the type FunctionOutputType

// The MatrixInputType is in general automatically deduced
// If (*f) is not overloaded, then FunctionInputType and FunctionOutputType are also
// automatically deduced

// Somehow cannot deduce FunctionInputType and FunctionOutputType if using a lambda
{
	Eigen::Matrix<FunctionOutputType, Eigen::Dynamic, Eigen::Dynamic> result(
			A.rows(), A.cols());

	for (size_t i = 0; i < A.rows(); i++)
		for (size_t j = 0; j < A.cols(); j++)
			result(i, j) = (*f)(A(i, j));

	return result;
}

// Matrix absolute value, note the syntax of lambda invocation
template<typename Derived>
Eigen::MatrixXcd absm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(adjoint(A) * A, [](const types::cplx & x)->types::cplx
	{	return std::sqrt(x);});
}

// Matrix exponential
template<typename Derived>
Eigen::MatrixXcd expm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(A, std::exp);
}

// Matrix logarithm
template<typename Derived>
Eigen::MatrixXcd logm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(A, std::log);
}

// Matrix square root
template<typename Derived>
Eigen::MatrixXcd sqrtm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(A, std::sqrt);
}

// Matrix sin
template<typename Derived>
Eigen::MatrixXcd sinm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(A, std::sin);
}

// Matrix cos
template<typename Derived>
Eigen::MatrixXcd cosm(const Eigen::MatrixBase<Derived> &A)
{
	return funm(A, std::cos);
}

}

#endif /* FUNCTIONAL_H_ */
