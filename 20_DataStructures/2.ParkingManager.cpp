#include<iostream>
using namespace std;
const int MAX_STACK_SIZE = 2;

struct cars
{
    int number;
    int parking_time;
};

struct node
{
    cars car;
    node* next;
    node()
    {
        next = NULL;
    }
};

class parkinglot
{
public:
    cars stack[MAX_STACK_SIZE*2-1];
    int stack_top;
    int stack2_base;
    int stack2_top;
    node* queue_start;
    node* queue_end;
    int queue_length;
    parkinglot()
    {
        stack_top = 0;
        stack2_base = MAX_STACK_SIZE * 2 - 2;
        stack2_top = MAX_STACK_SIZE * 2 - 2;
        queue_start = new node;
        queue_end = queue_start;
        queue_length = 0;
    }

    void arrive(int number, int time)
    {
        if(stack_top >= MAX_STACK_SIZE)
        {
            node* new_node_ptr = new node;
            new_node_ptr->car.number = number;
            queue_end->next = new_node_ptr;
            queue_end = new_node_ptr;
            cout << "Arrived in sideway:" << queue_length + 1 << endl;
            queue_length++;
        }
        else
        {
            stack[stack_top].number = number;
            stack[stack_top].parking_time = time;
            cout << "Arrived in parkinglot:" << stack_top + 1 << endl;
            stack_top++;
        }
    }

    void depart(int number, int time)
    {
        for(int i = stack_top - 1; i >= 0; i--)
        {
            if(stack[i].number != number)
                to_stack2();
            else
            {
                cout << "Departed,you've stayed for " << time - stack[i].parking_time << endl;
                stack_top--;
                break;
            }
        }
        for(int i = stack2_top + 1; i <= stack2_base; i++)
            to_stack();
        if(queue_length > 0)
        {
            stack[stack_top] = queue_start->next->car;
            stack[stack_top].parking_time = time;
            node* tmp = queue_start;
            queue_start = queue_start->next;
            free(tmp);
            stack_top++;
            queue_length--;
        }
    }

    void to_stack2()
    {
        if(stack_top > stack2_top || stack_top <= 0)
            return;
        stack_top--;
        stack[stack2_top] = stack[stack_top];
        stack2_top--;
    }

    void to_stack()
    {
        if(stack_top > stack2_top || stack2_top >= stack2_base)
            return;
        stack2_top++;
        stack[stack_top] = stack[stack2_top];
        stack_top++;
    }
};

int main()
{
    parkinglot p1;
    char operation;
    int number;
    int time;
    while(true)
    {
        cin >> operation >> number >> time;
        if(operation == 'E')
            break;
        if(operation == 'A')
            p1.arrive(number, time);
        if(operation == 'D')
            p1.depart(number, time);
    }
}