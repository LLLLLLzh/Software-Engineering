#include <iostream>
#include <vector>
#include <memory>
#include<list>
using namespace std;

template <typename K>
class Hasher
{
public:
	virtual int hashfunc(K a) = 0;
};

template <typename K>
class EqualityPredicate
{
public:
	virtual bool predicate(K a, K b) = 0;
};



template <typename K, typename V>
class Hashtable
{
private:
	std::vector<std::list<std::pair<K, V>>> hashtable;
	std::shared_ptr<Hasher<K> > hasher;
	std::shared_ptr<EqualityPredicate<K> > equalpred;

public:
	Hashtable(std::shared_ptr<Hasher<K> > hasher_, std::shared_ptr<EqualityPredicate<K> > equalpred_, int size);
	~Hashtable();
	void add(std::pair<K, V> pair);
	V getvalue(K key);

};

template <typename K, typename V>
Hashtable<K, V>::Hashtable(std::shared_ptr<Hasher<K> > hasher_, std::shared_ptr<EqualityPredicate<K> > equalpred_, int size) :hashtable(size)
{
	hasher = hasher_;
	equalpred = equalpred_;
}

template <typename K, typename V>
Hashtable<K, V>::~Hashtable()
{
}


template <typename K, typename V>
void Hashtable<K, V>::add(std::pair<K, V> pair) {
	K newkey = pair.first; V newvalue = pair.second;
	int position = hasher->hashfunc(newkey);
	std::list<std::pair<K, V>> currlist = hashtable[position];
	for (auto pairintable : currlist) {
		if (equalpred->predicate(newkey, pairintable.first)) {
			return;
		}
	}
	hashtable[position].push_back(pair);

	return;
}


template <typename K, typename V>
V Hashtable<K, V>::getvalue(K newkey) {
	int position = hasher->hashfunc(newkey);
	std::list<std::pair<K, V>> currlist = hashtable[position];
	for (auto pairintable : currlist) {
		if (equalpred->predicate(key, pairintable.first)) {
			return pairintable.second;
		}
	}
	return V();
}



class Myhasher :public Hasher<string> {
public:
	int hashfunc(string a) {
		int m = (int)a[0] - 65;
		if (m >= 26) { m -= 26; }
		return m;
	}
};

class Myequal :public EqualityPredicate<string> {
public:
	bool predicate(string a, string b) {
		return a == b;
	}
};


int main() {
	shared_ptr<Hasher<string>> phasher = make_shared<Myhasher>();
	shared_ptr<EqualityPredicate<string> > ppredicate = make_shared<Myequal>();
	Hashtable<string, int> mytable(phasher, ppredicate, 26);

	std::pair<string, int> p1("AAPL", 150), p2("NIKe", 106), p3("DIS", 96), p4("AMZN", 99);

	mytable.add(p1);
	mytable.add(p2);
	mytable.add(p3);
	mytable.add(p4);
	std::cout << mytable.getvalue("AAPL") << std::endl;

	return 0;
}


