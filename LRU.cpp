/*
运用你所掌握的数据结构，设计实现一个LRU（最近最少使用）缓存机制。它应该支持以下操作：获取数据get和写入数据put。

获取数据get(key)- 如果关键词(key)存在于缓存中，则获取关键字的值（总是正数），否则返回 -1。

写入数据put(key,value)- 如果关键字已经存在，则变更其数据值；如果关键字不存在，则插入该组[关键字/值]。当缓存容量达到上限时，它应该在写入数据之前删除最久未使用的数据值，从而为新的数据值留出空间。

进阶：你是否可以在O(1)时间复杂度内完成这两种操作？
*/
#include <iostream>
#include <unordered_map>

using namespace std;

// 双向链表
typedef struct Node{
    int key;
    int value;
    Node* next = nullptr;
    Node* before = nullptr;
}NODE;

class LRUCache{
private:
    int Capacity;
    int tmp;
    Node* root;
    NODE* end;
    unordered_map<int, NODE*> hashmap;
public:
    LRUCache(int capacity):Capacity(capacity){
        if(Capacity<=0){
            cout<<"Error!"<<endl;
        }
        tmp = 0;
        root = new NODE;
        end = root;
    }

    // 更新链表，输入变量为变动的节点地址
    void updateKey(NODE *tmpNODE){
        NODE *tmpNode = tmpNODE;
        // 输入为表头且不为表位时，将表头往后挪一位
        if(tmpNode==root&&tmpNode!=end){
            root = tmpNode->next;
            root->before = nullptr;
            end->next = tmpNode;
            tmpNode->before = end;
            tmpNode->next = nullptr;
            end = tmpNode;
        }
            // 输入为表尾时，无需更改
        else if(tmpNode==end){

        }
            // 输入为链表中间时，将该节点挪至尾部
        else{
            tmpNode->before->next = tmpNode->next;
            tmpNode->next->before = tmpNode->before;
            end->next = tmpNode;
            tmpNode->before = end;
            tmpNode->next = nullptr;
            end = tmpNode;
        }
    }

    //为了显示方便，使用void
    void get(int key){
        if(tmp==0){
            cout<<"-1"<<endl;
        }else{
            auto ptr = hashmap.find(key);
            if(ptr==hashmap.end()){
                cout<<"-1"<<endl;
            }else{
                NODE *tmpNode = ptr->second;
                updateKey(tmpNode);
                cout<<tmpNode->value<<endl;
            }
        }
    }

    void put(int key,int value){
        // 如果tmp==0&&Capacity>0，直接添加
        if(tmp==0&&Capacity>0){
            root->value = value;
            root->key = key;
            pair<int, NODE*> buff(key, root);
            hashmap.insert(buff);
            tmp++;
        }
            // 否则 搜索是否有key,
        else{
            // 没有key时，判断是否 tmp < Capacity
            if(hashmap.find(key)==hashmap.end()){
                // 如果 tmp = Capacity， 使用root节点作为新节点，并更新链表和哈希表
                if(tmp==Capacity){
                    hashmap.erase(root->key);
                    pair<int, NODE*> buff(key, root);
                    hashmap.insert(buff);
                    root->value = value;
                    root->key = key;
                    updateKey(root);
                }
                    // 如果tmp < Capacity，申请新的节点，添加至链表末尾，更新哈希表、tmp
                else{
                    NODE *newnode = new NODE;
                    newnode->value = value;
                    newnode->key = key;
                    newnode->before = end;
                    end->next = newnode;
                    end = newnode;

                    //update hashmap、tmp
                    pair<int, NODE *> buff(key, newnode);
                    hashmap.insert(buff);
                    tmp++;
                }
            }
            // 如果有的话，更改相应的value，更新链表
            else{
                updateKey(hashmap.at(key));
                hashmap.at(key)->value = value;
            }
        }
    }
};


int main(){
    LRUCache cache(2);
    cache.put(1,1);
    cache.put(2,2);
    cache.get(1);
    cache.put(3,3);
    cache.get(2);
    cache.put(4,4);
    cache.get(1);
    cache.get(3);
    cache.get(4);

    return 0;
}

