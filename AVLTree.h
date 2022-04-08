
#ifndef MIVNI1_AVLTREE_H
#define MIVNI1_AVLTREE_H

#include <iostream>


using namespace std;
typedef enum {
    SUCCESSS = 0,
    FAILUREE = -1,
    ALLOCATION_ERRORR = -2,
} AVLStatusType;
using namespace std;

template<typename Key, typename Data>
class Node {
public:
    Key key;
    Data data;
    Node<Key, Data>* parent;
    Node<Key, Data>* left;
    Node<Key, Data>* right;
    int height;
    int visited;
    explicit  Node() = default;
    // Node(Key &key,Data &data): key(key),data(data){}
    Node(Key& key, Data& data, Node* parent, Node* left, Node* right, int height = 0)
        : key(key), data(data), parent(parent), left(left), right(right), height(height), visited(0) {}

    ~Node() = default;

    void NodesetHeight();
    Node<Key, Data>* inOrder(Node<Key, Data>* node);
    void updateHeight();
    void deleteTree(Node<Key, Data>* node);
    Node(const Node<Key, Data>& node) : key(node.key), data(node.data), parent(node.parent), left(node.left),
        right(node.right), height(node.height) {}

    Node<Key, Data>& operator=(const Node<Key, Data>& node) {
        if (this == &node) {
            return *this;
        }
        data = node.data;
        key = node.key;
        parent = node.parent;
        left = node.left;
        right = node.right;
        height = node.height;
        return *this;
    }

};
static int absul(int x) {
    return x > 0 ? x : -x;
}
template<typename Key, typename Data>
Node<Key, Data>* Node<Key, Data>::inOrder(Node<Key, Data>* node) {
    Node<Key, Data>* tmp = node;
    if (tmp == nullptr) return nullptr;
    while (tmp->left != nullptr) {
        tmp = tmp->left;
    }
    return tmp;
}
template<typename Key, typename Data>
void Node<Key, Data>::NodesetHeight() {

    if (this->right == nullptr && this->left == nullptr) this->height = 0;
    else if (this->right != nullptr && this->left == nullptr) this->height = -(1 + absul(this->right->height));

    else if (this->right == nullptr && this->left != nullptr)this->height = 1 + absul(this->left->height);
    else {
        if (absul(this->left->height) > (absul(this->right->height))) this->height = 1 + absul(this->left->height);
        else { this->height = -(1 + absul(this->right->height)); }
    }
}

template<typename Key, typename Data>
void Node<Key, Data>::updateHeight() {
    Node<Key, Data>* tmp = this;
    while (tmp != nullptr) {
        tmp->NodesetHeight();
        tmp = tmp->parent;
    }

}



template<typename Key, typename Data>
class AVL {
public:
    Node<Key, Data>* root;
    int size;
    Node<Key, Data>* min_value;

    AVL() : root(nullptr), size(0), min_value(nullptr) {}

    ~AVL() = default;

    AVLStatusType insert(Key key, Data data);

    Node<Key, Data>* findNode(const Key& key);

    AVLStatusType deleteNode(Key key);

    void InOrder(Node<Key, Data>* node);

    void printTree();

    Node<Key, Data>* findLongestBath(Node<Key, Data>* node);
    Node<Key, Data>* updateTree(Node<Key, Data>* node);
    Node<Key, Data>* deleteonechild(Node<Key, Data>* node);
    Node<Key, Data>* findFirstBiggest(Node<Key, Data>* node);
    void DestroyTree();
    void swapNodes(Node<Key, Data>* upperNode, Node<Key, Data>* lowerNode);


};


template<typename Key, typename Data>
Node<Key, Data>* AVL<Key, Data>::findFirstBiggest(Node<Key, Data>* node) {
    Node<Key, Data>* tmp = node;
    while (tmp->left != nullptr) {
        tmp = tmp->left;
    }
    return tmp;
}


template<typename Key, typename Data>
Node<Key, Data>* AVL<Key, Data>::findNode(const Key& key) {
    Node<Key, Data>* tmp = this->root;

    while (tmp != nullptr) {

        if (tmp->key == key)
            return tmp;
        if (tmp->key > key) {
            tmp = tmp->left;
            continue;
        }
        tmp = tmp->right;

    }
    return tmp;
}

template<typename Key, typename Data>
Node<Key, Data>* rightRotate(Node<Key, Data>* node) {

    Node<Key, Data>* tmp1 = node->left;
    Node<Key, Data>* tmp2 = tmp1->right;
    node->left = tmp2;
    tmp1->right = node;
    if (tmp2 != nullptr) tmp2->parent = node;
    tmp1->parent = node->parent;
    if (node->parent != nullptr) {
        if (node->parent->right == node) node->parent->right = tmp1;
        else node->parent->left = tmp1;
    }
    node->parent = tmp1;
    node->NodesetHeight();
    tmp1->updateHeight();
    return tmp1;
}

template<typename Key, typename Data>
Node<Key, Data>* leftRotate(Node<Key, Data>* node) {
    Node<Key, Data>* tmp1 = node->right;
    Node<Key, Data>* tmp2 = tmp1->left;
    node->right = tmp2;
    if (tmp2 != nullptr) tmp2->parent = node;
    tmp1->left = node;
    tmp1->parent = node->parent;
    if (node->parent != nullptr) {
        if (node->parent->right == node) node->parent->right = tmp1;
        else node->parent->left = tmp1;
    }
    node->parent = tmp1;
    node->NodesetHeight();
    tmp1->updateHeight();
    return tmp1;
}

template<typename Key, typename Data>
Node<Key, Data>* AVL<Key, Data>::updateTree(Node<Key, Data>* node) {
    int x = 2;
    if (node->right == nullptr || node->left == nullptr) x = node->height;
    else x = absul(node->left->height) - absul(node->right->height);
    if (x == 0 || x == 1 || x == -1) {
        if (node->parent == nullptr) return node;
    }
    if (x > 1) {
        if (node->left->height >= 0) {//LL
            if (node->parent != nullptr) {
                if (node == node->parent->right) node->parent->right = rightRotate(node);
                else node->parent->left = rightRotate(node);
            }
            else node = rightRotate(node);
            if (node->parent == nullptr) {
                this->root = node;
                return node;
            }
        }
        else if (node->left->height < 0) {
            node->left = leftRotate(node->left); //LR
            if (node->parent != nullptr) {
                if (node == node->parent->right) node->parent->right = rightRotate(node);
                else node->parent->left = rightRotate(node);
            }
            else node = rightRotate(node);
            if (node->parent == nullptr) {
                this->root = node;
                return node;
            }
        }
    }
    if (x < -1) {
        if (node->right->height <= 0) { //RR
            if (node->parent != nullptr) {
                if (node == node->parent->right) node->parent->right = leftRotate(node);
                else node->parent->left = leftRotate(node);
            }
            else node = leftRotate(node);
            if (node->parent == nullptr) {
                this->root = node;
                return node;
            }
        }
        else if (node->right->height > 0) {
            node->right = rightRotate(node->right); //RL
            if (node->parent != nullptr) {
                if (node == node->parent->right) node->parent->right = leftRotate(node);
                else node->parent->left = leftRotate(node);
            }
            else node = leftRotate(node);
            if (node->parent == nullptr) {
                this->root = node;
                return node;
            }
        }
    }
    node = updateTree(node->parent);
    return node;
}

template<typename Key, typename Data>
AVLStatusType AVL<Key, Data>::insert(Key key, Data data) {
    Node<Key, Data>* new_node = new Node<Key, Data>(key, data, nullptr, nullptr, nullptr);
    if (new_node == nullptr) return ALLOCATION_ERRORR;
    //Node<Key, Data>* tmp = this->root;
    if (this->root == nullptr) {
        size++;
        this->root = new_node;
        this->min_value = new_node;
    }
    Node<Key, Data>* tmp = this->root;
    while (tmp != nullptr) {
        if (tmp->key == key) return FAILUREE;
        if (tmp->key > key) {
            if (tmp->left == nullptr) {
                size++;
                new_node->parent = tmp;
                tmp->left = new_node;
                if (this->min_value->key > key) this->min_value = new_node;
                break;
            }
            else tmp = tmp->left;
        }
        else {
            if (tmp->right == nullptr) {
                size++;
                new_node->parent = tmp;
                tmp->right = new_node;
                if (this->min_value->key > key) this->min_value = new_node;
                break;
            }
            else tmp = tmp->right;
        }
    }
    new_node->updateHeight();
    this->root = updateTree(new_node);
    return SUCCESSS;
}
template<typename Key, typename Data>
void AVL<Key, Data>::printTree() {
    InOrder(this->root);
}

template<typename Key, typename Data>
void AVL<Key, Data>::InOrder(Node<Key, Data>* node) {
    if (node == nullptr) {
        return;
    }
    if (node->left) {
        inorder(node->left);
    }
    if (node->right) {
        inorder(node->right);
    }
}
template<typename Key, typename Data>
Node<Key, Data>* AVL<Key, Data>::deleteonechild(Node<Key, Data>* node) {
    Node<Key, Data>* temp = node;
    if (temp->right == nullptr && temp->left == nullptr) {//no child case
        if (temp->parent != nullptr) {
            Node<Key, Data>* parent = temp->parent;
            if (temp->parent->right == temp) temp->parent->right = nullptr;
            else temp->parent->left = nullptr;
            delete temp;
            temp = nullptr;
            parent->updateHeight();
        }
        this->size--;
    }
    else if (temp->right != nullptr && temp->left == nullptr) {//one right child
        if (temp->parent != nullptr) {
            if (temp->parent->right == temp) {
                temp->parent->right = temp->right;
                temp->right->parent = temp->parent;
            }
            else {
                temp->parent->left = temp->right;
                temp->right->parent = temp->parent;
            }
            temp->parent->updateHeight();
        }
        else {
            this->root = temp->right;
            temp->right->parent = nullptr;
        }
        delete temp;
        temp = nullptr;
        this->size--;
    }
    else if (temp->right == nullptr && temp->left != nullptr) {//one left child
        if (temp->parent != nullptr) {
            if (temp->parent->right == temp) {
                temp->parent->right = temp->left;
                temp->left->parent = temp->parent;
            }
            else {
                temp->parent->left = temp->left;
                temp->left->parent = temp->parent;
            }
            temp->parent->updateHeight();
        }
        else {
            this->root = temp->left;
            temp->left->parent = nullptr;
        }
        delete temp;
        temp = nullptr;
        this->size--;
    }
    return this->root;
}

template<typename Key, typename Data>
AVLStatusType AVL<Key, Data>::deleteNode(Key key) {
    if (root == nullptr) return FAILUREE;
    Node<Key, Data>* temp = findNode(key);
    if (temp == nullptr) return FAILUREE;//if key is not found
    if (temp == root && temp->left == nullptr && temp->right == nullptr) {//size==1
        size--;
        delete temp;
        root = nullptr;
        return SUCCESSS;
    }
    if (temp->right == nullptr && temp->left == nullptr) {//no child case
        if (temp->parent != nullptr) {
            Node<Key, Data>* parent = temp->parent;
            if (temp->parent->right == temp) temp->parent->right = nullptr;
            else temp->parent->left = nullptr;
            delete temp;
            temp = nullptr;
            parent->updateHeight();
        }
        this->size--;
    }
    else if (temp->right != nullptr && temp->left == nullptr) {//one right child
        if (temp->parent != nullptr) {
            if (temp->parent->key < temp->key) {
                temp->parent->right = temp->right;
                temp->right->parent = temp->parent;
            }
            else {
                temp->parent->left = temp->right;
                temp->right->parent = temp->parent;
            }
            temp->parent->updateHeight();
        }
        else {
            this->root = temp->right;
            this->root->parent = nullptr;
        }
        delete temp;
        temp = nullptr;
        this->size--;
    }
    else if (temp->right == nullptr && temp->left != nullptr) {//one left child
        if (temp->parent != nullptr) {
            if (temp->parent->key < temp->key) {
                temp->parent->right = temp->left;
                temp->left->parent = temp->parent;
            }
            else {
                temp->parent->left = temp->left;
                temp->left->parent = temp->parent;
            }
            temp->parent->updateHeight();
        }
        else {
            this->root = temp->left;
            this->root->parent = nullptr;
        }
        delete temp;
        temp = nullptr;
        this->size--;
    }
    else {//more than one child
        Node<Key, Data>* toSwap = this->findFirstBiggest(temp->right);
        this->swapNodes(temp, toSwap);
        deleteonechild(temp);
        if (this->root == temp) {
            this->root = toSwap;
            this->root->parent = nullptr;
        }

    }
    if (root == nullptr) return SUCCESSS;
    Node<Key, Data>* NotBalanced;
    if (key < this->root->key) {
        if (this->root->left != nullptr)
            NotBalanced = findLongestBath(this->root->left);
        else
            NotBalanced = root;
    }
    else {
        if (this->root->right != nullptr)
            NotBalanced = findLongestBath(this->root->right);
        else NotBalanced = root;
    }
    this->min_value = this->findFirstBiggest(this->root);
    updateTree(NotBalanced);
    return SUCCESSS;
}

template<typename Key, typename Data>
Node<Key, Data>* AVL<Key, Data>::findLongestBath(Node<Key, Data>* node) {
    Node<Key, Data>* temp = node;
    while (temp != nullptr) {
        if (temp->right == nullptr && temp->left != nullptr)temp = temp->left;
        else if (temp->right != nullptr && temp->left == nullptr)temp = temp->right;
        else if (temp->left == nullptr && temp->right == nullptr) return temp;
        else {
            if (absul(temp->left->height) > absul(temp->right->height)) temp = temp->left;
            else temp = temp->right;
        }

    }
    return temp;
}
template<typename Key, typename Data>
void Node<Key, Data>::deleteTree(Node<Key, Data>* node) {
    if (node->right) deleteTree(node->right);
    if (node->left) deleteTree(node->left);
    delete node;
    node = nullptr;
}
template<typename Key, typename Data>
void AVL<Key, Data>::DestroyTree() {
    if (this->root == nullptr) return;
    this->root->deleteTree(this->root);
    this->root = nullptr;
    this->min_value = nullptr;
    size = 0;
}

template<class Key, class Data>
void AVL<Key, Data>::swapNodes(Node<Key, Data>* upperNode, Node<Key, Data>* lowerNode) {
    Node<Key, Data>* upperParent = upperNode->parent;
    Node<Key, Data>* lowerParent = lowerNode->parent;
    Node<Key, Data>* upperNodeLeft = upperNode->left, * upperNodeRight = upperNode->right;
    bool adjacent = upperNode->right == lowerNode;
    upperNode->left = (lowerNode->left != upperNode ? lowerNode->left : lowerNode);
    upperNode->right = (lowerNode->right != upperNode ? lowerNode->right : lowerNode);
    lowerNode->left = (upperNodeLeft != lowerNode ? upperNodeLeft : upperNode);
    lowerNode->right = (upperNodeRight != lowerNode ? upperNodeRight : upperNode);

    if (upperNode->left != nullptr) upperNode->left->parent = upperNode;
    if (upperNode->right != nullptr) upperNode->right->parent = upperNode;
    if (lowerNode->left != nullptr) lowerNode->left->parent = lowerNode;
    if (lowerNode->right != nullptr) lowerNode->right->parent = lowerNode;

    if (upperParent != nullptr) {
        if (upperParent->left == upperNode) {
            upperParent->left = lowerNode;
        }
        else if (upperParent->right == upperNode) {
            upperParent->right = lowerNode;
        }
        lowerNode->parent = upperParent;
    }
    else {
        lowerNode->parent = nullptr;
    }
    if (lowerParent != nullptr) {
        if (lowerParent->left == lowerNode) {
            lowerParent->left = upperNode;
        }
        else if (lowerParent->right == lowerNode) {
            lowerParent->right = upperNode;
        }
        if (adjacent)
            upperNode->parent = lowerNode;
        else
            upperNode->parent = lowerParent;
    }
    else {
        upperNode->parent = nullptr;
    }
    upperNode->NodesetHeight();
    lowerNode->NodesetHeight();

}

#endif