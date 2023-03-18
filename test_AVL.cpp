// Тестирование avl_tree

#include <cstdlib>
#include <set>
#include <algorithm>
#include "new_alv_tree.cpp"
using namespace std;

void test1(Node *p, int &h, int *&mn, int *&mx)
{
  int hl, hr;
  int *mnl, *mnr, *mxl, *mxr;
  if(p == 0)
    {
      h = 0;
      mn = 0;
      mx = 0;
    }
  else
    {
      test1(p->left, hl, mnl, mxl);
      test1(p->right, hr, mnr, mxr);
      if(p->height != max(hl, hr) + 1) cout<<"неверная h"<<endl;
      if(hl-hr < -1 || hl-hr > 1) cout<<"плохая балансировка"<<endl;
      if(mxl != 0 && *mxl > p->key) cout<<"левое больше"<<endl;
      if(mnr != 0 && *mnr < p->key) cout<<"правое меньше"<<endl;
      h = p->height;
      mn = new int(p->key);
      mx = new int(p->key);
      if(mnr != 0) *mn = min(*mn, *mnr);
      if(mnl != 0) *mn = min(*mn, *mnl);
      if(mxr != 0) *mx = max(*mx, *mxr);
      if(mxl != 0) *mx = max(*mx, *mxl);
      delete mnr;
      delete mxr;
      delete mnl;
      delete mxl;
      return;
    }
}


int main()
{
  set<int> S;
  Node *t = nullptr;
  int h, *m1 = nullptr, *m2 = nullptr;
  cout<<"Вставка:"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand();
      S.insert(x);
      t = insert(t, x, x+1);
      test1(t, h, m1, m2);
    }
  cout<<"Поиск(есть):"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand()%S.size();
      set<int>::iterator j = S.begin();
      advance(j,x);
      Node *tmp = search(t, *j);
      if(tmp->data != *j+1) cout<<"плохо ищет"<<endl;
    }
  cout<<"Поиск(нет):"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand();
      if(S.count(x) == 0 && search(t, x) != nullptr) cout<<"плохо ищет"<<endl;
    }
  cout<<"Удаление:"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand()%S.size();
      set<int>::iterator j = S.begin();
      advance(j,x);
      t = del(t, *j);
      test1(t, h, m1, m2);
    }
  return 0;
}
