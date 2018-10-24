#include <Vector.h>

#include <initializer_list>
#include <complex>
#include <cstdint>
#include <cstddef>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <boost/mpl/list.hpp>

namespace
{

class OperationCountingObject
{
public:
  OperationCountingObject(int value_ = 0)
    : value(value_)
  {
    ++constructedObjects;
  }

  OperationCountingObject(const OperationCountingObject& other)
    : value(std::move(other.value))
  {
    ++constructedObjects;
    ++copiedObjects;
  }

  OperationCountingObject(OperationCountingObject&& other)
    : value(other.value)
  {
    ++constructedObjects;
    ++movedObjects;
  }

  ~OperationCountingObject()
  {
    ++destroyedObjects;
  }

  OperationCountingObject& operator=(const OperationCountingObject& other)
  {
    ++assignedObjects;
    value = other.value;
    return *this;
  }

  OperationCountingObject& operator=(OperationCountingObject&& other)
  {
    ++assignedObjects;
    ++movedObjects;
    value = std::move(other.value);
    return *this;
  }

  operator int() const
  {
    return value;
  }

  static void resetCounters()
  {
    constructedObjects = 0;
    destroyedObjects = 0;
    copiedObjects = 0;
    movedObjects = 0;
    assignedObjects = 0;
  }

  static std::size_t constructedObjectsCount()
  {
    return constructedObjects;
  }

  static std::size_t destroyedObjectsCount()
  {
    return destroyedObjects;
  }

  static std::size_t copiedObjectsCount()
  {
    return copiedObjects;
  }

  static std::size_t movedObjectsCount()
  {
    return movedObjects;
  }

  static std::size_t assignedObjectsCount()
  {
    return assignedObjects;
  }

private:
  int value;

  static std::size_t constructedObjects;
  static std::size_t destroyedObjects;
  static std::size_t copiedObjects;
  static std::size_t movedObjects;
  static std::size_t assignedObjects;
};

std::size_t OperationCountingObject::constructedObjects = 0;
std::size_t OperationCountingObject::destroyedObjects = 0;
std::size_t OperationCountingObject::copiedObjects = 0;
std::size_t OperationCountingObject::movedObjects = 0;
std::size_t OperationCountingObject::assignedObjects = 0 ;

std::ostream& operator<<(std::ostream& out, const OperationCountingObject& obj)
{
  return out << '<' << static_cast<int>(obj) << '>';
}

struct Fixture
{
  Fixture()
  {
    OperationCountingObject::resetCounters();
  }
};

} // namespace

template <typename T>
using LinearCollection = aisdi::Vector<T>;

using TestedTypes = boost::mpl::list<std::int32_t,
                                     std::uint64_t,
                                     std::complex<std::int32_t>,
                                     OperationCountingObject>;

using std::begin;
using std::end;

BOOST_FIXTURE_TEST_SUITE(VectorTests, Fixture)

template <typename T>
void thenCollectionContainsValues(const LinearCollection<T>& collection,
                                  std::initializer_list<int> expected)
{
  BOOST_CHECK_EQUAL_COLLECTIONS(begin(collection), end(collection),
                                begin(expected), end(expected));
}

template <typename T>
void thenConstructedObjectsCountWas(std::size_t count)
{
  (void) count;
  // unable to check it (in a simple way) for all objects, hence template specialization.
}

template <typename T>
void thenDestroyedObjectsCountWas(std::size_t count)
{
  (void) count;
  // unable to check it (in a simple way) for all objects, hence template specialization.
}

template <typename T>
void thenCopiedObjectsCountWas(std::size_t count)
{
  (void) count;
  // unable to check it (in a simple way) for all objects, hence template specialization.
}

template <typename T>
void thenMovedObjectsCountWas(std::size_t count)
{
  (void) count;
  // unable to check it (in a simple way) for all objects, hence template specialization.
}

template <typename T>
void thenAssignedObjectsCountWas(std::size_t count)
{
  (void) count;
  // unable to check it (in a simple way) for all objects, hence template specialization.
}

template <>
void thenConstructedObjectsCountWas<OperationCountingObject>(std::size_t count)
{
  BOOST_CHECK_EQUAL(OperationCountingObject::constructedObjectsCount(), count);
}

template <>
void thenDestroyedObjectsCountWas<OperationCountingObject>(std::size_t count)
{
  BOOST_CHECK_EQUAL(OperationCountingObject::destroyedObjectsCount(), count);
}

template <>
void thenCopiedObjectsCountWas<OperationCountingObject>(std::size_t count)
{
  BOOST_CHECK_EQUAL(OperationCountingObject::copiedObjectsCount(), count);
}

template <>
void thenMovedObjectsCountWas<OperationCountingObject>(std::size_t count)
{
  BOOST_CHECK_EQUAL(OperationCountingObject::movedObjectsCount(), count);
}

template <>
void thenAssignedObjectsCountWas<OperationCountingObject>(std::size_t count)
{
  BOOST_CHECK_EQUAL(OperationCountingObject::assignedObjectsCount(), count);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollection_WhenCreatedWithDefaultConstructor_ThenItIsEmpty,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection;

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenAddingItem_ThenItIsNoLongerEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  collection.append(T{});

  BOOST_CHECK(!collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenGettingIterators_ThenBeginEqualsEnd,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK(begin(collection) == end(collection));
  BOOST_CHECK(const_cast<const LinearCollection<T>&>(collection).begin() == collection.end());
  BOOST_CHECK(collection.cbegin() == collection.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenGettingIterator_ThenBeginIsNotEnd,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(T{});

  BOOST_CHECK(collection.begin() != collection.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollectionWithOneElement_WhenIterating_ThenElementIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(753);

  auto it = collection.begin();

  BOOST_CHECK_EQUAL(*it, 753);
  BOOST_CHECK(++it == collection.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPostIncrementing_ThenPreviousPositionIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(T{});

  auto it = collection.begin();
  auto postIncrementedIt = it++;

  BOOST_CHECK(postIncrementedIt == collection.begin());
  BOOST_CHECK(it == collection.end());
  BOOST_CHECK(postIncrementedIt == collection.cbegin());
  BOOST_CHECK(it == collection.cend());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPreIncrementing_ThenNewPositionIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(T{});

  auto it = collection.begin();
  auto preIncrementedIt = ++it;

  BOOST_CHECK(preIncrementedIt == it);
  BOOST_CHECK(it == collection.end());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenIncrementing_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(collection.end()++, std::out_of_range);
  BOOST_CHECK_THROW(++(collection.end()), std::out_of_range);
  BOOST_CHECK_THROW(collection.cend()++, std::out_of_range);
  BOOST_CHECK_THROW(++(collection.cend()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenDecrementing_ThenIteratorPointsToLastItem,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(1);
  collection.append(2);

  auto it = collection.end();
  --it;

  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPreDecrementing_ThenNewIteratorValueIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(1);

  auto it = collection.end();
  auto preDecremented = --it;

  BOOST_CHECK(it == preDecremented);
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenPostDecrementing_ThenOldIteratorValueIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  collection.append(1);

  auto it = collection.end();
  auto postDecremented = it--;

  BOOST_CHECK(postDecremented == collection.end());
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenBeginIterator_WhenDecrementing_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(collection.begin()--, std::out_of_range);
  BOOST_CHECK_THROW(--(collection.begin()), std::out_of_range);
  BOOST_CHECK_THROW(collection.cbegin()--, std::out_of_range);
  BOOST_CHECK_THROW(--(collection.cbegin()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEndIterator_WhenDereferencing_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(*collection.end(), std::out_of_range);
  BOOST_CHECK_THROW(*collection.cend(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenConstIterator_WhenDereferencing_ThenItemIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 10, 20, 30 };

  auto it = ++collection.cbegin();

  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenDereferencing_ThenItemCanBeChanged,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 10, 20, 30 };

  auto it = ++begin(collection);
  *it = 500;

  thenCollectionContainsValues(collection, { 10, 500, 30 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenAddingInteger_ThenAdvancedIteratorIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 2001, 2010, 2051 };

  auto it = begin(collection);

  BOOST_CHECK(it + 3 == end(collection));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenIterator_WhenSubstractingInteger_ThenChangedIteratorIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 2001, 2010, 2051 };

  auto it = end(collection);

  BOOST_CHECK(it - 2 == ++begin(collection));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenAddingItem_ThenItemIsInCollection,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  collection.append(42);

  thenCollectionContainsValues(collection, { 42 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollection_WhenInitializingFromList_ThenAllItemsAreInCollection,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection = { 1410, 753, 1789 };

  thenCollectionContainsValues(collection, { 1410, 753, 1789 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenCreatingCopy_ThenAllItemsAreCopied,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1410, 753, 1789 };
  LinearCollection<T> other{collection};

  collection.append(1024);

  thenCollectionContainsValues(collection, { 1410, 753, 1789, 1024 });
  thenCollectionContainsValues(other, { 1410, 753, 1789 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenCreatingCopy_ThenBothCollectionsAreEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  LinearCollection<T> other{collection};

  BOOST_CHECK(other.isEmpty());
  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenMovingToOther_ThenAllItemsAreMoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1410, 753, 1789 };

  OperationCountingObject::resetCounters();
  LinearCollection<T> other{std::move(collection)};

  thenCollectionContainsValues(other, { 1410, 753, 1789 });
  thenConstructedObjectsCountWas<T>(0);
  thenCopiedObjectsCountWas<T>(0);
  thenAssignedObjectsCountWas<T>(0);
  thenMovedObjectsCountWas<T>(0);
  thenDestroyedObjectsCountWas<T>(0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenMovingToOther_ThenSecondCollectionsIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  LinearCollection<T> other{std::move(collection)};

  BOOST_CHECK(other.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenAssigningToOther_ThenAllElementsAreCopied,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection = { 1, 2, 3, 4 };
  LinearCollection<T> other = { 100, 200, 300, 400 };

  other = collection;

  thenCollectionContainsValues(other, { 1, 2, 3, 4 });
  thenCollectionContainsValues(collection, { 1, 2, 3, 4 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenAssigningToOther_ThenOtherCollectionIsEmpty,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection;
  LinearCollection<T> other = { 100, 200, 300, 400 };

  other = collection;

  BOOST_CHECK(other.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenSelfAssigning_ThenNothingHappens,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  collection = collection;

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNotEmptyCollection_WhenSelfAssigning_ThenNothingHappens,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 100, 200, 300, 400 };

  collection = collection;

  thenCollectionContainsValues(collection, { 100, 200, 300, 400 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenMoveAssigning_ThenAllElementsAreMoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1, 2, 3, 4 };
  LinearCollection<T> other = { 100, 200, 300, 400 };

  OperationCountingObject::resetCounters();
  other = std::move(collection);

  thenCollectionContainsValues(other, { 1, 2, 3, 4 });
  thenConstructedObjectsCountWas<T>(0);
  thenCopiedObjectsCountWas<T>(0);
  thenAssignedObjectsCountWas<T>(0);
  thenMovedObjectsCountWas<T>(0);
  thenDestroyedObjectsCountWas<T>(4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenMoveAssigning_ThenNewCollectionIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;
  LinearCollection<T> other = { 100, 200, 300, 400 };

  other = std::move(collection);

  BOOST_CHECK(other.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollection_WhenAppendingItem_ThenItemIsLast,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1, 2, 3 };

  collection.append(42);

  thenCollectionContainsValues(collection, { 1, 2, 3, 42 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenPrependingItem_ThenItemIsAdded,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  collection.prepend(300);

  thenCollectionContainsValues(collection, { 300 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPrependingItem_ThenItemIsFirst,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1, 2 };

  collection.prepend(300);

  thenCollectionContainsValues(collection, { 300, 1, 2 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenGettingSize_ThenZeroIsReturned,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection;

  BOOST_CHECK_EQUAL(collection.getSize(), 0);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenGettingSize_ThenElementCountIsReturned,
                              T,
                              TestedTypes)
{
  const LinearCollection<T> collection = { 12, 100, 500 };

  BOOST_CHECK_EQUAL(collection.getSize(), 3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollection_WhenChangingIt_ThenItsSizeAlsoChanges,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 72, 27, 77 };
  collection.append(99);

  BOOST_CHECK_EQUAL(collection.getSize(), 4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPrependingItem_ThenSizeIsUpdated,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 72, 27, 77 };
  collection.prepend(99);

  BOOST_CHECK_EQUAL(collection.getSize(), 4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenInsertingItem_ThenItemIsAdded,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  collection.insert(begin(collection), 42);

  thenCollectionContainsValues(collection, { 42 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenInsertingAtBegin_ThenItemIsPrepended,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 11, 12, 13 };

  collection.insert(begin(collection), 42);

  thenCollectionContainsValues(collection, { 42, 11, 12, 13 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenInsertingAtEnd_ThenItemIsAppended,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 11, 12, 13 };

  collection.insert(end(collection), 42);

  thenCollectionContainsValues(collection, { 11, 12, 13, 42 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenInsertingInMiddle_ThenItemInserted,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 11, 12, 13 };

  collection.insert(++begin(collection), 42);

  thenCollectionContainsValues(collection, { 11, 42, 12, 13 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenInserting_ThenSizeIsUpdated,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 101, 102, 103 };

  collection.insert(begin(collection), 27);

  BOOST_CHECK_EQUAL(collection.getSize(), 4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenPoppingFirst_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(collection.popFirst(), std::logic_error);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenPoppingLast_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(collection.popLast(), std::logic_error);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollectionWithSingleItem_WhenPoppingFirst_ThenCollectionIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 420 };

  collection.popFirst();

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollectionWithSingleItem_WhenPoppingLast_ThenCollectionIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 420 };

  collection.popLast();

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingFirst_ThenCollectionSizeIsReduced,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 14, 10 };

  collection.popFirst();

  BOOST_CHECK_EQUAL(collection.getSize(), 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingLast_ThenCollectionSizeIsReduced,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 14, 10 };

  collection.popLast();

  BOOST_CHECK_EQUAL(collection.getSize(), 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingFirst_ThenItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 300, 8, 480 };

  collection.popFirst();

  thenCollectionContainsValues(collection, { 8, 480 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingLast_ThenItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 300, 8, 480 };

  collection.popLast();

  thenCollectionContainsValues(collection, { 300, 8 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingFirst_ThenItemsIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 101, 202, 303 };

  BOOST_CHECK_EQUAL(collection.popFirst(), 101);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenPoppingLast_ThenItemsIsReturned,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 101, 202, 303 };

  BOOST_CHECK_EQUAL(collection.popLast(), 303);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenEmptyCollection_WhenErasing_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection;

  BOOST_CHECK_THROW(collection.erase(collection.begin()), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingEnd_ThenOperationThrows,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 20, 16 };

  BOOST_CHECK_THROW(collection.erase(end(collection)), std::out_of_range);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingBegin_ThenItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 22, 41, 31 };

  collection.erase(begin(collection));

  thenCollectionContainsValues(collection, { 41, 31 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingLastItem_ThemItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 22, 45, 33 };

  collection.erase(--end(collection));

  thenCollectionContainsValues(collection, { 22, 45 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingMiddleItem_ThenItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 22, 51, 48 };

  collection.erase(++begin(collection));

  thenCollectionContainsValues(collection, { 22, 48 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasing_ThenSizeIsReduced,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1000, 500, 2, 900 };

  collection.erase(begin(collection) + 2);

  BOOST_CHECK_EQUAL(collection.getSize(), 3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenCollectionWithSingleItem_WhenErasing_ThenCollectionIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 1529 };

  collection.erase(begin(collection));

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingEmptyRange_ThenNothingHappens,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 19, 42, 11 };

  collection.erase(begin(collection), begin(collection));

  thenCollectionContainsValues(collection, { 19, 42, 11 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingRangeFromBegin_ThenItemsAreRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 19, 42, 11 };

  collection.erase(begin(collection), begin(collection) + 2);

  thenCollectionContainsValues(collection, { 11 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_whenErasingRangeToEnd_ThenItemsAreRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 20, 1, 45 };

  collection.erase(begin(collection) + 1, end(collection));

  thenCollectionContainsValues(collection, { 20 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingSingleItemRange_ThenItemIsRemoved,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 2001, 2010, 2051, 3001 };

  collection.erase(begin(collection) + 1, begin(collection) + 2);

  thenCollectionContainsValues(collection, { 2001, 2051, 3001 });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingWholeRange_ThenCollectinIsEmpty,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 400, 403, 404 };

  collection.erase(begin(collection), end(collection));

  BOOST_CHECK(collection.isEmpty());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(GivenNonEmptyCollection_WhenErasingRange_ThenSizeIsUpdated,
                              T,
                              TestedTypes)
{
  LinearCollection<T> collection = { 23, 10, 20, 16 };

  collection.erase(begin(collection) + 1, end(collection) - 1);

  BOOST_CHECK_EQUAL(collection.getSize(), 2);
}

// ConstIterator is tested via Iterator methods.
// If Iterator methods are to be changed, then new ConstIterator tests are required.

BOOST_AUTO_TEST_SUITE_END()
