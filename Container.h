
#pragma once

#include <cstdint>
#include <iterator>
#include <iostream>

class Task;
using namespace std;


class ContainerException: public std::exception{};
class PopEmptyException: virtual public ContainerException{};

template <typename T>
class Container {
public:
    struct elem{
         T obj;
         elem* prev;
         elem *next;
    };

private:
    elem *first;
    elem *last;
    uint64_t size;

public:

    class Container_Iterator{
    private:
        elem *cur_elem;
    public:
        explicit Container_Iterator(Container & container){
                cur_elem = container.first;
        }
        explicit Container_Iterator(elem * node){
            cur_elem = node;
        }
        T& operator*() const {
            return cur_elem->obj;
        }
        T& operator*() {
            return cur_elem->obj;
        }
        Container_Iterator& operator++(){
            if (cur_elem != nullptr)
                cur_elem = cur_elem->next;
            return *this;
        }
        const Container_Iterator operator++(int){
            Container_Iterator tmp = *this;
            if (tmp.cur_elem != nullptr)
                tmp.cur_elem = tmp.cur_elem->next;
            return tmp;
        }
        Container_Iterator& operator--(){
            if (cur_elem != nullptr)
                cur_elem = cur_elem->prev;
            return *this;
        }
        const Container_Iterator operator--(int){
            Container_Iterator tmp = *this;
            if (tmp.cur_elem != nullptr)
                tmp.cur_elem = tmp.cur_elem->prev;
            return tmp;
        }
        bool operator==(const Container_Iterator & other){
            return cur_elem == other.cur_elem;
        }
        bool operator!=(const Container_Iterator & other){
            return !(*this == other);
        }
    };

    Container(){
        size = 0;
        first = last = new elem;
    }

    void push_front(T const & obj){
        elem * new_obj = new elem;
        new_obj->obj = obj;
        new_obj->next = nullptr;
        new_obj->prev = nullptr;

        if (size == 0) {
            first = new_obj;
            first->next = last;
            last->prev = first;
        }else {
            elem * prev_first = first;
            first = new_obj;
            first->next = prev_first;
            prev_first->prev = first;
        }

        ++size;

    }
    void push_back(T const & obj){
        auto * new_obj = new elem;
        new_obj->obj = obj;
        new_obj->next = nullptr;
        new_obj->prev = nullptr;

       if (size == 0){
            first = new_obj;
            first->next = last;
            last->prev = first;
       } else {
            elem * prev_end = last->prev;
            prev_end->next = new_obj;
            new_obj->next = last;
            new_obj->prev = prev_end;
            last->prev = new_obj;
       }
        ++size;

    }

    T & pop_first(){

        if (size == 0)
            throw PopEmptyException();

        T * s = new T(first->obj);

        if (size == 1){
            delete first;
            first = nullptr;
        } else {
            elem * new_first = first->next;
            delete first;
            first = new_first;
            first->prev = nullptr;
        }
        --size;
        return *s;
    }

    T & pop_last(){
        if (size == 0)
            throw PopEmptyException();


        T * s = new T(last->prev->obj);

        if (size == 1){
            delete last->prev;
            first = nullptr;
        } else {
            elem * prev_prev_end = last->prev->prev;
            delete last->prev;
            prev_prev_end->next = last;
            last->prev = prev_prev_end;
        }
        --size;
        return s;
    }

    Container_Iterator begin(){
        return Container_Iterator(this->first);
    }

    Container_Iterator end(){
       return Container_Iterator(this->last);
    }

    uint64_t get_size(){
        return size;
    }
    bool is_empty() {return size == 0;}

    void swap(Container<T> & other){
        elem * tmp_first = this->first;
        elem * tmp_last = this->last;
        uint64_t tmp_size = this->size;

        first = other.first;
        last = other.last;
        size = other.size;

        other.first = tmp_first;
        other.last = tmp_last;
        other.size = tmp_size;
    }

    void reverse(){
        elem *tmp = last;
        last = first;
        first = tmp;
    }

    void clear(){
        if (size == 0)
            return;
        elem * tmp = first;
        while (tmp != last){
            elem * new_tmp = tmp->next;
            delete tmp;
            tmp = new_tmp;
        }
        first = nullptr;
        size = 0;
    }

    ~Container(){
        clear();
    }

    //todo: почитать про exception'ы
    //todo: стиль
    //todo: написать письмо с кодом, предусмотреть месяц на обсуждение всех моментов. 25-го апреля, получается, дедлайн. Но чем раньше - тем лучше.

};
