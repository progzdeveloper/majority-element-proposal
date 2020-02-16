// MIT License
// 
// Copyright (c) 2018, Michael Polukarov (Russia).
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <functional> // for equal_to
#include <iterator>   // for iterator_traits


/*!
 * \brief majority_element() find a majority element in a [begin, end) range
 *
 * Given a range of values [begin, end) where strictly more than half the
 * elements have the same value according to the binary comparator comp,
 * returns the value of this most-common element.  If no element is a majority
 * element, end is returned as a sentinel.
 * 
 * \tparam _FwdIt type of iterator that meet requipments of forward-only iterator
 * \tparam _Comp type of user-supplied equality comparator functor
 * \param begin iterator pointing to begin of range
 * \param end iterator pointing to one-past-end element of range
 * \param comp user-supplied comparator functor
 */
template <class _FwdIt, class _Comp>
_FwdIt majority_element(_FwdIt begin, _FwdIt end, _Comp comp) 
{
	/* Initially, we have no guess and our count is zero.  However, to avoid
	 * edge cases with the empty range, we initialize the candidate to end.
	 */
	_FwdIt candidate = end;
	size_t confidence = 0;

	/* Scan over the input using the Boyer-Moore update rules. */
	for (_FwdIt it = begin; it != end; ++it) {
		/* If we have no confidence in our previous guess, update it to this new
		 * element.
		 */
		if (confidence == 0) {
			candidate = it;
			++confidence;
		}
		/* Otherwise, increment or decrement the confidence based on whether the
		 * next element matches.
		 */
		else if (comp(*candidate, *it))
			++confidence;
		else
			--confidence;
	}

	/* Finally, do one more pass to confirm that we have a majority element. */
	size_t nmatches = 0, ntotal = 0;
	for (_FwdIt it = begin; it != end; ++it) {
		/* Check whether this is a match and update appropriately. */
		if (comp(*candidate, *it))
			++nmatches;

		/* Either way, increment the total number of elements. */
		++ntotal;
	}

	/* This is a majority element if it accounts for at least half the number
	 * of elements.
	 */
	return (((ntotal / 2) < nmatches) ? candidate : end);
}

/*!
 * \brief majority_element() find a majority element in a [begin, end) range
 *
 * Given a range of values [begin, end) where strictly more than half the
 * elements have the same value according to the binary comparator std::equal_to<>,
 * returns the value of this most-common element.  If no element is a majority
 * element, end is returned as a sentinel.
 *
 * \tparam _FwdIt type of iterator that meet requipments of forward-only iterator
 * \param begin iterator pointing to begin of range
 * \param end iterator pointing to one-past-end element of range
 * \note std::equal_to<> is used to compare values.
 * \note Non-comparator version implemented in terms of comparator version.
 */
template <class _FwdIt>
inline _FwdIt majority_element(_FwdIt begin, _FwdIt end) {
	typedef typename std::iterator_traits<_FwdIt>::value_type val_t;
	return majority_element(begin, end, std::equal_to<val_t>());
}

/*!
 * \brief is_majority_element() check if a given value is actually a majority element.
 *
 * Given a sorted range of values [begin, end) check whatever supplied element x 
 * is actually a majority element.
 *
 * \tparam _FwdIt type of iterator that meet requipments of forward-only iterator
 * \tparam _Comp type of user-supplied comparator functor
 * \param begin iterator pointing to begin of range
 * \param end iterator pointing to one-past-end element of range
 * \param x value of majority candidate 
 * \param comp comp user-supplied comparator functor
 * \return true if x is a majority element, otherwise return false
 * \note range [begin, end) must be sorted
 */
template <class _FwdIt, class _Comp>
inline bool is_majority_element(_FwdIt begin, _FwdIt end, 
								const typename std::iterator_traits<_FwdIt>::value_type& x, _Comp comp)
{
	/* Find a range of first and last occurences of the element x */
	auto range = std::equal_range(begin, end, x, comp);

	/* This is a majority element if it accounts for at 
	 * least half the number of elements.
	 */
	return (std::distance(range.first, range.second) > (std::distance(begin, end) >> 1));
}

/*!
* \brief is_majority_element() check if a given value is actually a majority element.
*
* Given a sorted range of values [begin, end) check whatever supplied element x
* is actually a majority element.
*
* \tparam _FwdIt type of iterator that meet requipments of forward-only iterator
* \param begin iterator pointing to begin of range
* \param end iterator pointing to one-past-end element of range
* \param x value of majority candidate
* \return true if x is a majority element, otherwise return false
* \note range [begin, end) must be sorted
* \note std::less<> is used to compare values.
* \note Non-comparator version implemented in terms of comparator version.
*/
template <class _FwdIt>
inline bool is_majority_element(_FwdIt begin, _FwdIt end, 
								const typename std::iterator_traits<_FwdIt>::value_type& x)
{
	typedef typename std::iterator_traits<_FwdIt>::value_type val_t;
	return is_majority_element(begin, end, x, std::less<val_t>());
}



