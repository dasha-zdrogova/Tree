// Тестирование avl_tree

#include <cstdlib>
#include <set>
#include <algorithm>
#include "RB_with_key_and_data.cpp"
using namespace std;

int main()
{
  set<int> S;
  RBTree t;
  cout<<"Вставка:"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand();
      S.insert(x);
      t.insert(Item(x, x+1));
    }
  cout<<"Поиск(есть):"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand()%S.size();
      set<int>::iterator j = S.begin();
      advance(j,x);
      int *tmp = t.search(*j);
      if(*tmp != *j+1) cout<<"плохо ищет"<<endl;
    }
  cout<<"Поиск(нет):"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand();
      if(S.count(x) == 0 && t.search(x) != nullptr) cout<<"плохо ищет"<<endl;
    }
  cout<<"Удаление:"<<endl;
  for(int i = 0; i < 1000; i++)
    {
      int x = rand()%S.size();
      set<int>::iterator j = S.begin();
      advance(j,x);
      t.del(*j);
    }
  return 0;
}