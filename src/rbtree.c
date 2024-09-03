#include "rbtree.h"

#include <stdlib.h>

void Lrot(rbtree *tree, node_t*node)
{
    //현재 노드 오른쪽주소 값 변수
    node_t *nowright = node->right;

    // 오른쪽 노드의 왼쪽 노드를 현재노드의 오른쪽과 연결
    node->right = nowright->left;

    // 이때 nowright->left가 nill노드 > 신경 x
    // nill이 아닐경우 nowright->left의 부모는 현재 노드
    if (node->right != tree->nil)
    {
        nowright->left->parent = node;
    }
    // 노드들의 부모를 변경
    nowright->parent = node->parent;
    // 현재 노드의 부모의 경우 3가지 경우존재
    
    // 현재 노드부모가 nill
    if (node->parent == tree->nil)
    {// 오른쪽 노드가 root
        tree->root = nowright;

    // 현재 노드가 왼쪽 자식
    } else if (node == node->parent->left){
        //오른쪽 노드가 부모의 왼쪽자식
        node->parent->left = nowright;

    //현재 노드가 오른쪽 자식
    } else { 
        //오른쪽 노드가 부모의 오른쪽 자식
        node->parent->right = nowright;
    }
    nowright->left = node;
    node->parent = nowright;
}

void Rrot(rbtree *tree, node_t *node)
{
    node_t *nowleft = node->left;
    node->left = nowleft->right;

    if (nowleft->right != tree->nil){
        nowleft->right->parent = node;
    }

    nowleft->parent = node->parent;

    if (node->parent == tree->nil){
        tree->root = nowleft;

    }else if (node == node->parent->left){
        node->parent->left = nowleft;

    }else{
        node->parent->right = nowleft;
    }
    nowleft->right = node;
    node->parent = nowleft;
}

void insertfix(rbtree* tree, node_t* newnode)
{
    node_t *uncle;
    // newnode의 부모가 빨간색일 경우 진행
    while(newnode->parent->color == 0)
    {
        //부모가 조부모 왼쪽자식
        if (newnode->parent == newnode->parent->parent->left)
        {// 부모가 왼쪽 이므로 삼촌은 오른쪽이다.
            uncle = newnode->parent->parent->right;
            //1. uncle의 color = 0 (red)
            if (uncle->color == 0)
            {// 부모, 삼촌 color  0 > 1, 조부모 color 1 > 0
                newnode->parent->color = 1;
                uncle->color = 1;
                newnode->parent->parent->color = 0;
                // 색 변경 후 newnode중 red로 변경된 노드가 조건을 벗어나는지 확인해야한다.
                newnode = newnode->parent->parent;
                // 최악의 경우 루트 노드까지올라가게 되며
                // 이경우 while끝난 후 루트노드의 색깔을 검정으로 바꿔주면 해결된다.
            }else {// 2. uncle의 color = 1 (black), 내가 부모의 오른쪽 자식일경우
                if (newnode == newnode->parent->right)
                {// 부모를 기준으로 왼쪽 로테이션 시켜서 3. 형태로 만들어준다.
                    newnode = newnode->parent;
                    Lrot(tree, newnode);
                }// 3. uncle의 color = 1 (black), 내가 부모의 왼쪽 자식일경우
                //먼저 부모 0 > 1, 조부모 1 > 0의 색깔변경 위치이동후 색깔변경 귀찮아짐
                newnode->parent->color = 1;
                newnode->parent->parent->color = 0;
                // 조부모 기준 오른쪽 로테이션 진행
                Rrot(tree, newnode->parent->parent);
            }
        } else{// 부모가 할아버지 오른쪽자식의 경우 왼쪽과 로직 동일, 좌우만 변경
            uncle = newnode->parent->parent->left;
            //1
            if (uncle->color == 0)
            {
                newnode->parent->color = 1;
                uncle->color = 1;
                newnode->parent->parent->color = 0;
                newnode = newnode->parent->parent;
            }else {//2
                if (newnode == newnode->parent->left)
                {
                    newnode = newnode->parent;
                    Rrot(tree, newnode);
                }
                newnode->parent->color = 1;
                newnode->parent->parent->color = 0;
                Lrot(tree, newnode->parent->parent);
            }
        }
    }
    //루트 색깔 변경 잊지말자.
    tree->root->color = 1;
}

rbtree *new_rbtree(void) {
  // 1.  rbtree구조체 생성/ root포인터, nil포인터 포함됨
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));

  // 2. sentinel 노드 생성
  node_t *nilnode = (node_t *)calloc(1, sizeof(node_t));

  // 3. sentinel 노드초기화
  nilnode->color = 1;
  nilnode->key = -1;
  nilnode->parent = tree->nil;
  nilnode->left = tree->nil;
  nilnode->right = tree->nil;

  // 4. 닐노드 포인터 지정
  tree->nil = nilnode;
  // 초기 루트노드는 닐노드임.
  tree->root = tree->nil;

//   tree->root = nilnode; 큰 차이는 없지만 때에 따라서 차이발생 할 수 있음.
  return tree;
}

void deltree(rbtree *tree, node_t *node)
{
    if (node != tree->nil)
    {
        deltree(tree, node->right);
        deltree(tree, node->left);
        free(node);
    }
    
}

void delete_rbtree(rbtree *t) 
{
  // 만약 루트 노드가 닐노드가 아니라면 트리가 존재 
  if (t->root != t->nil)
  {
      deltree(t, t->root);
  }
  
  free(t->nil);
  free(t);

}

node_t *rbtree_insert(rbtree *tree, const key_t newkey) 
{
    node_t *newnode = (node_t *)calloc(1, sizeof(node_t));
    newnode->color = 0;
    newnode->key = newkey;
    newnode->right = tree->nil;
    newnode->left = tree->nil;

    node_t *nownode = tree->root;
    // 만약 rbtree가 비어있다면 새로 만든 노드는 루트노드가 된다. 
    if (nownode == tree->nil){
        tree->root = newnode;
        newnode->parent = tree->nil;
        // 루트 노드는 항상 검은색이여야 한다!
        newnode->color = 1;
        return newnode;
    }
    // rbtree에 노드 존재
    // nil 노드에 도달할때까지
    node_t *nowparent;
    while (nownode != tree->nil)
    {// 다음노드의 부모노드가 될 현재 노드 저장
        nowparent = nownode;

        // 만약 현재 노드의 key값이 새로운 노드의 key값보다 크거나 작다면
        if ( nownode->key >= newkey)
        {// 현재노드의 왼쪽노드로 넘어가라
            nownode = nownode->left;
        } else{// 현재노드의 오른쪽노드로 넘어가라
            nownode = nownode->right;
        }
    }

    // while 문이 깨지면 현재노드 nil노드에 도달 따라서 새로운 노드의 부모노드 갱신
    newnode->parent = nowparent;
    if (nowparent->key >= newkey){
        nowparent->left = newnode;
    }else{
        nowparent->right = newnode;
    }
    insertfix(tree, newnode);
    return newnode;
}

node_t *rbtree_find(const rbtree *tree, const key_t findkey) 
{
    // 트리의 루트부터 시작
    node_t *nownode = tree->root;
    //1 찾는 키와 현재 노드 키값이 다를 경우 진행
    while (nownode->key != findkey)
    {
        if (nownode == tree->nil){//현재노드 닐 노드라면
            return NULL;
        }

        // 만약 키 값 크다면 현재노드 오른쪽 노드가 다음노드
        if (nownode->key < findkey){
            nownode = nownode->right;
        }else{ // 작다면 왼쪽 노드가 다음노드
            nownode = nownode->left;
        } // 현재노드 키값과 찾는 키값이 같을 때까지 무한반복
    }
    return nownode;
}

node_t *rbmin(const rbtree *t, node_t* nodett) {
//   node_t* nownode = t-> root;
  node_t* nownode = nodett;
  node_t* temp;
  while(nownode != t -> nil){
    temp = nownode;
    nownode = nownode->left;
  }
  return temp;
}
/////////////////////////////////////////////////////////
node_t *rbtree_min(const rbtree *tree) {
    node_t* minnode = tree->root;
    while(minnode->left != tree->nil){
        minnode = minnode->left;
    }
    return minnode;
}

node_t *rbtree_max(const rbtree *tree) {
    node_t* maxnode = tree->root;
    while(maxnode->right != tree->nil){
        maxnode = maxnode->right;
    }
    return maxnode;
}
/////////////////////////////////////////////////////////////
node_t *rbmax(const rbtree *tree, node_t* nodett) {
    node_t* maxnode = nodett;
    // while(maxnode != tree->nil) 이런식으로 진행하면 임시 변수를 하나 더 설정해야한다.(rbmin)
    // 아래와 같이 진행하는게 더 좋아보인다. // 확인 후 이동
    while(maxnode->right != tree->nil){
        maxnode = maxnode->right;
    }
    return maxnode;
}

void changep(rbtree* tree, node_t* oldnode, node_t* newnode){
    // 바꿀 노드가 루트노드 라면
    if (oldnode->parent == tree->nil){
        tree->root = newnode;

    }else if (oldnode == oldnode->parent->left){
        oldnode->parent->left = newnode;
    
    }else{
        oldnode->parent->right = newnode;
    
    }
    newnode->parent = oldnode ->parent;
}

void delfixed(rbtree* tree, node_t *delchil)
{
    node_t* fixnode = delchil;
    node_t* othernode;

    // delchil 노드가 루트, 색깔이 검정> while문 끝
    while(fixnode != tree->root && fixnode->color == 1)
    {   // fixnode이 왼쪽일때
        if(fixnode == delchil->parent->left){
            othernode = fixnode->parent->right;
            // 1. othernode 가 빨간색 > 2,3,4 가능
            if (othernode->color == 0){
                // 색깔 변경
                othernode -> parent -> color = 0;
                othernode -> color = 1;
                // othernode-p 왼쪽 회전
                Lrot(tree, othernode->parent);
                // othernode는 변경후 fixnode->p의 오른쪽 자식 갱신
                othernode = fixnode->parent->right;
            }
            // 1에서 othernode의 색 변경 따라서 바로 진행
            // 2 othernode 모든 자식이 검은색  > 1, 3, 4 가능
            if (othernode->left->color == 1 && othernode->right->color == 1){
                // othernode의 색변경
                othernode->color = 0;
                // fixnode 갱신
                fixnode = othernode->parent;

            }else{ // othernode 가 검은색
            // 3 othernode 왼쪽 자식이 빨간색 > 4 가능
                if(othernode->left->color == 0)
                {// 색깔 변경
                    othernode->left->color = 1;
                    othernode->color = 0;
                    // 오른쪽으로 회전
                    Rrot(tree, othernode);
                    // othernode 갱신
                    othernode = fixnode->parent->right;
                }else{
                // 4 othernode 오른쪽 자식이 빨간색 > 종료 가능//
                othernode->color = othernode->parent->color;
                othernode->parent->color = 1;
                othernode->right->color = 1;
                Lrot(tree, othernode->parent);

                // 여기 까지 오게 된다면 모든 문제점들이 해결된거나 마찬가지
                // while 문을 끝내주기 위한 조건일뿐 자료구조 자체는 변경점이 없다. 
                fixnode = tree->root;
                }
            }
        } else{// 좌우 반전
            othernode = fixnode->parent->left;
            if (othernode->color == 0){
                othernode->parent->color = 0;
                othernode->color = 1;
                Rrot(tree, fixnode->parent);
                othernode = fixnode->parent->left;
            }

            if (othernode->left->color == 1 && othernode->right->color == 1){
                othernode->color = 0;
                fixnode = fixnode->parent;

            }else{
                if (othernode->right->color == 0){
                    othernode->parent->color = 0;
                    othernode->right->color = 1;
                    Lrot(tree, othernode);
                    othernode = fixnode->parent->left;
                }else{
                othernode->color = othernode->parent->color;
                othernode->parent->color = 1;
                othernode->left->color = 1;
                Rrot(tree,othernode->parent);
                fixnode = tree->root;
                }
            }
        }
            
    }
    fixnode->color = 1;
}





int rbtree_erase(rbtree *tree, node_t *delnode) {
    if (delnode == NULL){
        return 0;
    }
    // 삭제할 노드, 색깔 저장
    node_t* delchil;
    node_t* deling = delnode; 
    color_t delcolor = delnode->color;
    // 삭제 노드가 빨간색일때
    // 자식이 왼쪽 하나 or 자식이 없을때
    if (delnode->right == tree->nil){
        // 삭제할 노드의 왼쪽자식의 주소 저장
        // 노드삭제후 트리구조변경시 사용 
        delchil = deling->left;
        // 삭제할 노드와 왼쪽 자식노드를 바꿔준다.
        changep(tree, deling, deling->left);
    
    }else if (delnode->left == tree->nil){
        // 자식이 오른쪽 하나(좌우 반대)
        delchil = deling->right;
        changep(tree, deling, deling->right);
    
    }else{ // 두개의 자식 존재
        // 삭제 하고자 하는 노드의 왼쪽을 기준으로> 왼쪽 노드를 시작으로 가장 작은 값을 사용
        //                 노드의 오른쪽을 기준 > 오른쪽 노드를 시작으로 가장 큰 값을 사용
        // 두 방식 모두 적용가능

        // 삭제할 노드, 색깔 저장
        deling  = rbmax(tree, delnode->left);
        delcolor = deling->color;
        delchil = deling->left;

        // 만약 삭제 노드의 오른쪽 값이 아닐경우, 즉 더 아래에 있을 경우.
        if(deling != delnode->left){
            // deling의 자식을 deling의 부모노드와 연결
            changep(tree, deling, deling->left);
            deling->left = delnode->left;
            deling->left->parent = deling;
        }else{
            delchil->parent = deling;
        }
        // 삭제할 노드의 부모를 deling의 부모로 변경
        changep(tree, delnode, deling);
        // delnode의 오른쪽 자식을 deling과 연결
        deling->right = delnode->right;
        deling->right->parent = deling;
        // 삭제 노드의 색 정보를 deling으로 전달
        deling->color = delnode->color;
    }
    // delnode 삭제
    free(delnode);

    if (delcolor == 1){
        delfixed(tree, delchil);
    }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  
  return 0;
}