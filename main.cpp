#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <cassert>

#include "Container.h"
#include "Object.h"


using namespace std;
uint64_t Object::g_counter = 0;


void fill_task_container(Container<Task *> *container, vector<Task *> &created_task_ttl_list) {
    enum task_types {
        CALC_TASK = 0,
        PUSH_TASK = 1,
        COUNT_TASKS = 2,
        COUNT_HAS_RES_TASKS = 3,
        CLEAN_TASK = 4,
        COUNT_OBJS = 5
    } task_type;

    //Randomizing things
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> task_distrib(0,5);
    std::uniform_int_distribution<int> sign_distrib(0,3);


    queue<HasResTask *> cached_tasks;

    uint8_t i = 0;
    while (i < 17){
        task_type = task_types(task_distrib(rng));

        auto new_sign = CalcTask::op_type(sign_distrib(rng));
        HasResTask *task_with_res = nullptr;
        Task *task_no_res = nullptr;

        switch (task_type) {
            case CALC_TASK:
                if (cached_tasks.size() < 2)
                    continue;

                task_with_res = new CalcTask((string)"name", new_sign, cached_tasks.front(),
                                             cached_tasks.back());
                break;

            case PUSH_TASK:
                {
                    auto tmp = new CountObjTask();
                    created_task_ttl_list.push_back(tmp);
                    task_no_res = new PushTask(container, tmp);
                }
                break;

            case COUNT_TASKS:
                task_with_res = new CountTask(container);
                break;

            case COUNT_HAS_RES_TASKS:
                task_with_res = new CountHasResTask(container);
                break;

            case CLEAN_TASK:
                task_no_res = new CleanTask(container);
                break;

            case COUNT_OBJS:
                task_with_res = new CountObjTask();
                break;

            default:
                assert(false);
        }

        ++i;

        if (task_with_res != nullptr){
             cached_tasks.push(task_with_res);
             if (cached_tasks.size() > 2)
                cached_tasks.pop();

             container->push_back(task_with_res);
             created_task_ttl_list.push_back(task_with_res);
        } else {
            container->push_back(task_no_res);
            created_task_ttl_list.push_back(task_no_res);
        }

    }

}


void print_container(Container<Task *> * con){
    uint8_t i = 0;
    for (auto & el : *con){
        cout << (int) i++ << ":" << el->toString() << endl;
    }
}

int main(){
    vector<Task *> created_task_ttl_list{};

    auto * tasks = new Container<Task *>();
    fill_task_container(tasks, created_task_ttl_list);
    print_container(tasks);
    Container<uint64_t> results{};

    cout << "Total amount of Objects created: " << Object::getGCounter() << endl;

    try {
        while (!tasks->is_empty()) {
            Task *cur_task = tasks->pop_first();
            cur_task->run();
            if (cur_task->has_res())
                results.push_back(dynamic_cast<HasResTask *>(cur_task)->getResult());
        }
    } catch (PopEmptyException & e){
        cout << e.what() << endl;
    } catch (runtime_error & e){
        cout << e.what() << endl;
    }


    cout << "results:" << endl;
    for (auto & el : results)
        cout << el << " ";
    cout << endl;


    tasks->clear();
    for (auto  & el: created_task_ttl_list)
        delete el;
    results.clear();
    cout << "Remaining objects amount:" << Object::getGCounter() <<  endl;

    return 0;
}