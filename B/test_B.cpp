#include <cstdlib>
#include <set>
#include <algorithm>
#include <cassert>
#include "B_with_keys_and_data.cpp"

using namespace std;

const int N = 10;

void test1(const BTree &t, BTreeNode *r, int *&mn, int *&mx)
{
  if(r==0) { return; }
  if (t.root!=r)
    if (r->n < r->t-1 || r->n > 2 * r->t - 1)
      cout << "неправильное число сыновей" << endl;
  if (mn != nullptr)
    if (r->elems[0].key <= *mn)
      cout << "неправильный порядок1" << endl;
  for (int i = 1; i < r->n; ++i)
    if (r->elems[i].key <= r->elems[i - 1].key)
      {
        cout << "неправильный порядок2" << endl;
      }
  if (mx != nullptr)
    if (r->elems[r->n - 1].key >= *mx)
      cout << "неправильный порядок3" << endl;
  for (int i = 0; i <= r->n; ++i)
    if ((r->leaf) != (r->C[i] == nullptr))
      cout << "неправильная балансировка" << endl;
  if (!r->leaf)
  {
    int hl, hr;
    int *mn1, *mx1;
    mn1 = mn;
    mx1 = new int(r->elems[0].key);
    test1(t, r->C[0], mn1, mx1);
    mn1 = new int;
    for (int i = 1; i < r->n; ++i)
    {
      *mn1 = r->elems[i - 1].key;
      *mx1 = r->elems[i].key;
      test1(t, r->C[i], mn1, mx1);
    }
    delete mx1;
    mx1 = mx;
    *mn1 = r->elems[r->n - 1].key;
    test1(t, r->C[r->n], mn1, mx1);
    delete mn1;
  }
}

int main()
{
  set<int> S;
  BTree t(N);
  int *m1 = nullptr, *m2 = nullptr;
  cout << "Вставка:" << endl;
  for (int i = 0; i < 1000; i++)
  {
    int x = rand();
    S.insert(x);
    t.insert(x, x + 1);
    test1(t, t.root, m1, m2);
  }
  cout << "Поиск(есть):" << endl;
  for (int i = 0; i < 1000; i++)
  {
    int x = rand() % S.size();
    set<int>::iterator j = S.begin();
    advance(j, x);
    if (*t.search(*j) != *j + 1)
      cout << "плохо ищет t[" << *j << "] = " << t.search(*j) << " должно " << *j + 1 << "   x = " << x << " S.size() == " << S.size() << endl;
  }
  cout << "Поиск(нет):" << endl;
  for (int i = 0; i < 1000; i++)
  {
    int x = rand();
    if (S.count(x) == 0 && t.search(x) != 0)
      cout << "плохо ищет" << endl;
  }
  cout << "Удаление:" << endl;
  for (int i = 0; i < 1000; i++)
  if(!S.empty()) {
    int x = rand() % S.size();
    set<int>::iterator j = S.begin();
    advance(j, x);
    t.remove(*j);
    test1(t, t.root, m1, m2);
  }
  return 0;
}