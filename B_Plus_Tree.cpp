// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Entry;
class Node;
class BPlusTree;

class Entry
{
public:
    int data;
    Node *right_child;
    Entry(int data_in)
    {
        data = data_in;
        right_child = nullptr;
    }
};

bool compare(Entry *a, Entry *b)
{
    return ((a->data) < (b->data));
}

class Node
{
public:
    int d, t;
    // Data Node has d to 2d values except Root node.
    // Index Node has t to 2t + 1 values except Root node.
    vector<Entry *> keys;
    bool isDataNode;
    Node *parent;
    Node *left_child;
    Node(int d_in, int t_in, bool dataNode)
    {
        d = d_in;
        t = t_in;
        parent = nullptr;
        left_child = nullptr;
        isDataNode = dataNode;
    };
};

class BPlusTree
{
public:
    BPlusTree(int d_in, int t_in)
    {
        d = d_in;
        t = t_in;
        firstDisplay = true;
        indexNode = 0;
        dataNode = 0;
        root = nullptr;
    };

private:
    int d;
    int t;
    bool firstDisplay;
    int indexNode;
    int dataNode;
    Node *root;

public:
    void insertKey(int value)
    {
        // No Root Exists.

        if (root == nullptr)
        {
            root = new Node(d, t, true);
            root->keys.push_back(new Entry(value));
            dataNode++;
        }

        // Root Exists.

        else
        {
            // Root DataNode case.
            if (root->isDataNode == true)
            {
                // After insertion root does not become full.
                if (root->keys.size() < 2 * d)
                {
                    Entry *n = new Entry(value);
                    root->keys.push_back(n);
                    sort(root->keys.begin(), root->keys.end(), compare);
                }
                else
                {
                    // After insertion root becomes full.
                    Entry *n = new Entry(value);
                    root->keys.push_back(n);
                    sort(root->keys.begin(), root->keys.end(), compare);

                    Node *right_node = new Node(d, t, true);
                    dataNode++;
                    vector<Entry *> vec(root->keys.begin() + d, root->keys.end());
                    right_node->keys = vec;
                    vec.clear();
                    root->keys.resize(d);
                    // (root -> keys.size() - 1) / 2 == d is true

                    Node *left = root;

                    n = new Entry(right_node->keys[0]->data);
                    n->right_child = right_node;

                    root = new Node(d, t, false);
                    indexNode++;
                    root->keys.push_back(n);
                    root->left_child = left;

                    left->parent = root;
                    right_node->parent = root;
                }
            }

            // Root IndexNode case.

            else
            {
                // Finding data node
                Node *curr = root;
                while (curr->isDataNode == false)
                {
                    int k = 0;
                    for (int i = (int)(curr->keys.size()) - 1; i >= 0; i--)
                    {
                        if (curr->keys[i]->data < value)
                        {
                            k = 1;
                            curr = curr->keys[i]->right_child;
                            break;
                        }
                    }
                    if (k == 0)
                    {
                        curr = curr->left_child;
                    }
                }
                // Adding new values in data node.

                // DataNode not full.

                if (curr->keys.size() < 2 * d)
                {
                    Entry *n = new Entry(value);
                    curr->keys.push_back(n);
                    sort(curr->keys.begin(), curr->keys.end(), compare);
                }
                // DataNode full.
                else
                {
                    Entry *n = new Entry(value);
                    curr->keys.push_back(n);
                    sort(curr->keys.begin(), curr->keys.end(), compare);

                    Node *right_node = new Node(d, t, true);
                    dataNode++;
                    vector<Entry *> vec(curr->keys.begin() + d, curr->keys.end());
                    right_node->keys = vec;
                    vec.clear();
                    curr->keys.resize(d);
                    // (curr -> keys.size() - 1) / 2 == d is true

                    n = new Entry(right_node->keys[0]->data);
                    n->right_child = right_node;

                    right_node->parent = curr->parent;

                    // Sending index value to parent

                    curr->parent->keys.push_back(n);
                    Node *curr_parent = curr->parent;
                    sort(curr_parent->keys.begin(), curr_parent->keys.end(), compare);

                    // Checking if the parent IndexNode is overflowed
                    curr = curr->parent;

                    // If overflowed
                    while (curr->keys.size() > 2 * t + 1)
                    {
                        if (curr == root)
                        {
                            Node *right_node = new Node(d, t, false);
                            indexNode++;
                            vector<Entry *> vec(curr->keys.begin() + t + 1, curr->keys.end());
                            right_node->keys = vec;
                            vec.clear();
                            Entry *imp = curr->keys[t];
                            int num = curr->keys[t]->data;
                            curr->keys.resize(t);

                            Entry *n = new Entry(num);
                            right_node->left_child = imp->right_child;
                            n->right_child = right_node;

                            root = new Node(d, t, false);
                            indexNode++;
                            root->keys.push_back(n);
                            root->left_child = curr;

                            curr->parent = root;
                            right_node->parent = root;
                            curr = curr->parent;

                            right_node->left_child->parent = right_node;
                            for (int i = 0; i < (right_node->keys.size()); i++)
                            {
                                right_node->keys[i]->right_child->parent = right_node;
                            }
                        }
                        else
                        {
                            Node *right_node = new Node(d, t, false);
                            indexNode++;
                            vector<Entry *> vec(curr->keys.begin() + t + 1, curr->keys.end());
                            right_node->keys = vec;
                            vec.clear();
                            Entry *imp = curr->keys[t];
                            int num = curr->keys[t]->data;
                            curr->keys.resize(t);

                            Entry *n = new Entry(num);
                            right_node->left_child = imp->right_child;
                            n->right_child = right_node;

                            curr = curr->parent;
                            right_node->parent = curr;
                            curr->keys.push_back(n);
                            sort(curr->keys.begin(), curr->keys.end(), compare);

                            right_node->left_child->parent = right_node;
                            for (int i = 0; i < (right_node->keys.size()); i++)
                            {
                                right_node->keys[i]->right_child->parent = right_node;
                            }
                        }
                    }
                }
            }
        }
    }

    void display()
    {
        if (!firstDisplay)
        {
            cout << endl;
        }
        firstDisplay = false;
        cout << indexNode << " " << dataNode;
        if (root != nullptr)
        {
            for (int i = 0; i < root->keys.size(); i++)
            {
                cout << " " << root->keys[i]->data;
            }
        }
        cout << "\n";
        Print_Keys(root);
    }

    void Print_Keys(Node *curr)
    {
        if (curr != nullptr)
        {
            if (curr->isDataNode)
            {
                cout << "Printing a Data Node\n";
                for (int i = 0; i < curr->keys.size(); i++)
                    cout << curr->keys[i]->data << " ";
                cout << "\n";
            }
            else
            {
                cout << "\nPrinting an Index Node\n";
                for (int i = 0; i < curr->keys.size(); i++)
                    cout << curr->keys[i]->data << " ";
                cout << "\n";
                Print_Keys(curr->left_child);
                for (int i = 0; i < curr->keys.size(); i++)
                    Print_Keys(curr->keys[i]->right_child);
            }
        }
    }
};

int main()
{
    int d, t, operation;

    // 2d is the capacity of Data Node.
    // t to 2t+1 keys in a Index Node.

    cin >> d;
    cin >> t;

    BPlusTree instance(d, t);

    while (cin >> operation)
    {
        switch (operation)
        {
        case 1:
        {
            int data;
            cin >> data;
            instance.insertKey(data);
            break;
        }

        case 2:
        {
            instance.display();
            break;
        }

        case 3:
        {
            return 0;
        }
        default:
            cout << "Please enter a valid operation\n";
        }
    }

    return 0;
}
