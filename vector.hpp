#pragma once
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <iostream>
#include <exception>
#include <string>

template <class T>
class Vector
{
public:
    //Constructors
    Vector(std::size_t capacity = 100);

    Vector(const Vector<T>& src);
    Vector(Vector<T>&& src);

    //Operators
    Vector& operator=(const Vector<T>& src);
    Vector& operator=(Vector<T>&& src);
    const T& operator[](std::size_t index);

    //DeStructor
    ~Vector();

    //add and remove element
    void push_back(const T& element);
    void pop_back();
    void insert(const T& element, std::size_t pos);
    void erase(std::size_t pos);

    bool isEmpty();

    //size
    void shrink_to_fit();
    void resize(std::size_t new_size, T elements = T());

    //iterators
    T* begin();
    T* end();

    void clear();

    template <class U>
    friend std::ostream& operator<<(std::ostream& out, const Vector<U>& vect);

private:
    T* array_;
    std::size_t size_; //colvo elements NOW
    std::size_t capacity_;
    std::size_t front_;
    std::size_t back_;
};

template <class T>
class WrongVectorCapacity: public std::exception
{
public:
    WrongVectorCapacity(std::string reason) : reason_(reason) {}
    const char* what() const noexcept override { return reason_.c_str(); }
private:
    std::string reason_;
};

template <class T>
Vector<T>:: Vector(std::size_t capacity) :
    size_(0),
    capacity_(capacity),
    front_(0),
    back_(0)
{
    try
    {
        array_ = new T[capacity_ + 1];
    }
    catch(...)
    {
        throw WrongVectorCapacity<T>("Falled allocate memory");
    }
}

template <class T>
const T& Vector<T>::operator[](std::size_t index)
{
    if (index >= back_ || index < front_) throw WrongVectorCapacity<T>("Index out of range");

    return array_[index];
}

template <class T>
Vector<T>:: ~Vector()
{
    delete[] array_;
    size_ = 0;
    capacity_ = 0;
}

template <class T>
void Vector<T>::push_back(const T& element)
{
    if (size_ == capacity_)
    {
        std::size_t new_capacity = capacity_ * 2;

        try
        {
            T* new_array = new T[new_capacity];

            for (std::size_t i = front_; i != back_; ++i) new_array[i] = array_[i];

            delete[] array_;
            array_ = new_array;
            capacity_ = new_capacity;
        }
        catch(...)
        {
            throw WrongVectorCapacity<T>("Falled allocate memory");
        }
    }

    array_[back_] = element;
    ++back_;
    ++size_;
}

template <class T>
void Vector<T>::pop_back()
{
    if (isEmpty()) throw WrongVectorCapacity<T>("Vector is empty");

    try
    {
        T* new_array = new T[capacity_];

        for (std::size_t i = front_; i < size_ - 1; ++i)
        {
            new_array[i] = array_[i];
        }

        delete[] array_;
        array_ = new_array;
        --size_;
        --back_;
    }
    catch(...)
    {
        throw WrongVectorCapacity<T>("Falled allocate memory");
    }
}

template <class T>
void Vector<T>::insert(const T& element, std::size_t pos)
{
    if (size_ + 1 == capacity_)
    {
        std::size_t new_capacity = capacity_ * 2;

        try
        {
            T* new_array = new T[new_capacity];

            for (std::size_t i = front_; i <= back_ + 1; ++i)
            {
                if (i == pos) new_array[i] = element;
                if (i < pos) new_array[i] = array_[i];
                if (i > pos) new_array[i] = array_[i - 1];
            }

            delete[] array_;
            array_ = new_array;
            capacity_ = new_capacity;
            ++back_;
            ++size_;
        }
        catch(...)
        {
            throw WrongVectorCapacity<T>("Falled allocate memory");
        }
    }
    else if (size_ + 1 < capacity_)
    {
        try
        {
            T* new_array = new T[capacity_];

            for (std::size_t i = front_; i < back_ + 1; ++i)
            {
                if (i == pos) new_array[i] = element;
                if (i < pos) new_array[i] = array_[i];
                if (i > pos) new_array[i] = array_[i - 1];
            }

            delete[] array_;
            array_ = new_array;
            ++back_;
            ++size_;
        }
        catch(...)
        {
            throw WrongVectorCapacity<T>("Falled allocate memory");
        }
    }
}

template <class T>
void Vector<T>::erase(std::size_t pos)
{
    if (isEmpty()) throw WrongVectorCapacity<T>("Vector is empty");
    if (pos > back_ || pos < front_) throw WrongVectorCapacity<T>("Index out of range");

    T tmp = array_[pos];

    try
    {
        T* new_array = new T[capacity_];

        for (std::size_t i = front_; i <= back_; ++i)
        {
            if (i >= pos) new_array[i] = array_[i+1];
            else new_array[i] = array_[i];
        }

        delete[] array_;
        array_ = new_array;
        --size_;
        --back_;
    }
    catch(...)
    {
        throw WrongVectorCapacity<T>("Falled allocate memory");
    }
}

template <class T>
bool Vector<T>::isEmpty()
{
    return size_ == 0;
}

template <class T>
void Vector<T>::shrink_to_fit()
{
    capacity_ = size_;

    try
    {
        T* new_array = new T[size_];

        for (std::size_t i = front_; i < size_; ++i) new_array[i] = array_[i];

        delete[] array_;
        array_ = new_array;
    }
    catch(...)
    {
        throw WrongVectorCapacity<T>("Falled allocate memory");
    }
}

template <class T>
void Vector<T>::resize(std::size_t new_size, T elements)
{
    if (new_size > size_)
    {
        if (new_size >= capacity_)
        {
            try
            {
                T* new_array = new T[new_size + 1];

                for (std::size_t i = front_; i < new_size; ++i)
                {
                    if (i < size_) new_array[i] = array_[i];
                    if (i >= size_) new_array[i] = elements;
                }

                delete[] array_;
                array_ = new_array;
                capacity_ = new_size + 1;
                size_ = new_size;
                back_ = size_;
            }
            catch(...)
            {
                throw WrongVectorCapacity<T>("Falled allocate memory");
            }
        }
        else if (new_size < capacity_)
        {
            for (std::size_t i = size_; i < new_size; ++i)
            {
                array_[i] = elements;
            }

            size_ = new_size;
            back_ = size_;
        }
    }
    else if (new_size < size_)
    {
        try
        {
            T* new_array = new T[capacity_ + 1];

            for (std::size_t i = front_; i < size_; ++i)
            {
                if (i < new_size) new_array[i] = array_[i];
            }

            delete[] array_;
            array_ = new_array;
            size_ = new_size;
        }
        catch(...)
        {
            throw WrongVectorCapacity<T>("Falled allocate memory");
        }
    }
}

template <class T>
T* Vector<T>::begin()
{
    return array_;
}

template <class T>
T* Vector<T>::end()
{
    return array_ + size_ - 1;
}

template <class T>
void Vector<T>::clear()
{
    if (isEmpty()) throw WrongVectorCapacity<T>("Vector is empty");

    size_ = 0;

    try
    {
        T* new_array = new T[capacity_ + 1];

        delete[] array_;
        array_ = new_array;
    }
    catch(...)
    {
        throw WrongVectorCapacity<T>("Falled allocate memory");
    }
}

template<class U>
std::ostream& operator<<(std::ostream& out, const Vector<U>& vect)
{
    out << "\nElements of vector" << '\n';

    for (std::size_t i = vect.front_; i < vect.back_; ++i)
    {
        out << vect.array_[i] << '\n';
    }

    out << "End of vector's elements" << '\n';

    return out;
};

#endif // !__VECTOR_HPP