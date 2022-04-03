#include <bits/stdc++.h>
using namespace std;

class bucket
{
public:
    int *arr;
    int capacity, ldepth;

    bucket(int max_buc_cap)
    {
        arr = (int *)calloc(max_buc_cap + 1, sizeof(int));
    }
};

class extHash
{
public:
    int crowding = -1;
    int gdepth;
    int max_buc_cap;
    int max_gdepth = 20;
    bucket **dir;
    vector<int> order_of_creation;

    extHash(int given_gdepth, int given_max_buc_cap)
    {
        gdepth = given_gdepth;
        max_buc_cap = given_max_buc_cap;
        dir = (bucket **)calloc((1 << gdepth), sizeof(bucket *));
        for (int i = 0; i < (1 << gdepth); i++)
        {
            // cout << i << "\n";
            bucket *p = new bucket(max_buc_cap);
            // cout << "done\n";
            // cout << "done dona done\n";
            p->capacity = 0;
            p->ldepth = gdepth;
            dir[i] = p;
            order_of_creation.push_back(i);
        }
    }

    bucket *Insertion(int key)
    {

        int curr_buck = key % (1 << gdepth);
        dir[curr_buck]->arr[dir[curr_buck]->capacity] = key;
        dir[curr_buck]->capacity++;

        // cout << "In inseert function\n";
        if (dir[curr_buck]->capacity == max_buc_cap + 1)
        {
            for (int i = 0; i < (1 << gdepth); i++)
            {
                if (dir[i]->capacity == max_buc_cap + 1)
                {
                    crowding = i;
                    break;
                }
            }
            // crowding = key % (1 << gdepth);
            if (Accomodation_Possible(key))
            {
                // cout << "Accomodating\n";
                bucket *curr = dir[crowding];
                // While ldepth < gdepth
                while ((curr->ldepth < gdepth) && (crowding >= 0))
                {
                    bucket *p = new bucket(max_buc_cap);
                    order_of_creation.push_back(crowding + (1 << curr->ldepth));
                    for (int i = 0; i < (1 << gdepth - curr->ldepth); i++)
                    {
                        if (i % 2 == 0)
                            dir[crowding + i * (1 << curr->ldepth)] = curr;
                        else
                            dir[crowding + i * (1 << curr->ldepth)] = p;
                    }
                    curr->ldepth++;
                    p->ldepth = curr->ldepth; // can be optimised
                    curr->capacity = 0;
                    p->capacity = 0;
                    crowding = -1;

                    int duplicate[max_buc_cap + 1];
                    for (int i = 0; i < max_buc_cap + 1; i++)
                        duplicate[i] = curr->arr[i];

                    for (int i = 0; i < max_buc_cap + 1; i++) // can be optimised
                        Insertion(duplicate[i]);
                }

                if (curr->ldepth == gdepth && crowding >= 0)
                {
                    curr->capacity--;
                    Double_Directory();
                    Insertion(key);
                }
            }
            else
                cout << "Accomomodation not possible\n";
        }
        return dir[curr_buck];
    }

    bool Search(int key)
    {
        bucket *curr_buck = dir[key % (1 << gdepth)];
        bool found = false;
        for (int i = 0; i < curr_buck->capacity; i++)
        {
            if (curr_buck->arr[i] == key)
            {
                found = true;
                break;
            }
            // cout << curr_buck->arr[i] << " ";
        }
        // cout << "\n";
        return found;
    }

    void Delete_value(int key)
    {
        if (Search(key))
        {
            bucket *curr_buck = dir[key % (1 << gdepth)];
            for (int i = 0; i < curr_buck->capacity; i++)
            {
                if (curr_buck->arr[i] == key)
                {
                    curr_buck->arr[i] = curr_buck->arr[curr_buck->capacity - 1];
                    curr_buck->capacity--;
                    break;
                }
            }
        }
        else
            cout << "Cannot delete as key isn't present in hash table\n";
    }

    void Display_Status()
    {
        cout << gdepth << "\n";
        cout << order_of_creation.size() << "\n";
        for (auto x : order_of_creation)
        {
            // cout << "Address number " << x << " ";
            cout << dir[x]->capacity << " " << dir[x]->ldepth << "\n";
            // for (int i = 0; i < dir[x]->capacity; i++)
            //     cout << dir[x]->arr[i] << " ";
            // cout << "\n";
        }
    }

private:
    bucket **Double_Directory()
    {
        int k = (1 << gdepth); // can be optimised
        bucket **p;
        p = (bucket **)calloc(2 * k, sizeof(bucket *));
        for (int i = 0; i < 2 * k; i++)
            p[i] = dir[i % k];
        bucket **q = dir;
        dir = p;
        free(q);
        gdepth++;
        // for (int i = 0; i < 2 * k; i++)
        //     cout << dir[i] << " ";
        // cout << "\n";
        return dir;
    }

    bool Accomodation_Possible(int key)
    {
        map<int, int> freq;
        for (int i = 0; i <= max_buc_cap; i++)
            freq[dir[crowding]->arr[i] % (1 << max_gdepth)]++;
        for (auto x : freq)
            if (x.second > max_buc_cap)
                return false;
        return true;
    }
};

int main()
{
    int given_gdepth, given_max_buc_cap;
    cin >> given_gdepth >> given_max_buc_cap;
    extHash object1(given_gdepth, given_max_buc_cap);
    // cout << object1.gdepth << " " << object1.max_buc_cap << "\n";
    int n;
    bool quit = false;
    while (cin >> n)
    {
        int key;
        switch (n)
        {
        case 2:
            // cout << "Insertion\n";
            cin >> key;
            // cout << "Inserting " << key << "\n";
            object1.Insertion(key);
            // cout << "Insertion finished\n";
            break;
        case 3:
            cin >> key;
            // cout << "Searching " << key << "\n";
            if (object1.Search(key))
                cout << "Found the key " << key << "\n";
            else
                cout << "Not found " << key << "\n";
            break;
        case 4:
            // cout << "Delete key " << key << "\n";
            cin >> key;
            object1.Delete_value(key);
            break;
        case 5:
            // cout << "Display Status\n";
            object1.Display_Status();
            break;
        case 6:
        {
            // cout << "Quit\n";
            quit = true;
            break;
        }
        default:
            cout << "Select a valid option\n";
        }
        if (quit)
            break;
    }

    return 0;
}

// int crowding = -1;
// int gdepth;
// int max_buc_cap;
// int max_gdepth = 20;

// bool Initialise_Dir(int depth)
// {
//     int k = (1 << depth);
//     dir = new bucket[k];
//     int *p = new int[max_buc_cap + 1];
//     int *q = new int[max_buc_cap + 1];
//     for (int i = 0; i < k; i++)
//     {
//         if (i % 2 == 0)
//             dir[i].arr = p;
//         else
//             dir[i].arr = q;
//         dir[i].capacity = 0;
//         dir[i].ldepth = 1;
//     }
//     cout << k << "\n";
//     return true;
// }

// bool Double_Directory()
// {
//     int k = (1 << gdepth);
//     bucket *p = new bucket[2 * k];
//     for (int i = 0; i < 2 * k; i++)
//         p[i] = dir[i % k];
//     bucket *q = dir;
//     dir = p;
//     delete q;
//     gdepth++;
//     // for (int i = 0; i < 2 * k; i++)
//     //     cout << dir[i] << " ";
//     // cout << "\n";
//     return true;
// }

// bool Accomodation_Possible(int key)
// {
//     map<int, int> freq;
//     for (int i = 0; i <= max_buc_cap; i++)
//         freq[dir[crowding].arr[i] % (1 << max_gdepth)]++;
//     for (auto x : freq)
//         if (x.second > max_buc_cap)
//             return false;
//     return true;
// }

// bool Insertion(int key)
// {
//     int curr_buck = key % (1 << gdepth);
//     dir[curr_buck].arr[dir[curr_buck].capacity] = key;
//     dir[curr_buck].capacity++;

//     if (dir[curr_buck].capacity == max_buc_cap + 1)
//     {
//         crowding = curr_buck;
//         if (Accomodation_Possible(key))
//         {
//             bucket* curr=&dir[crowding];
//             while(dir[crowding].ldepth<gdepth && crowding>=0)
//             {
//                 bucket p;
//                 dir[crowding+(1<<curr->ldepth)]=p;
//                 dir[crowding].ldepth++;
//                 p->ldepth=dir[crowding].ldepth;

//             }
//         }
//         else
//             cout << "Accomomodation not possible\n";
//     }
//     return true;
// }