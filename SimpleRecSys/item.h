/*
 * =====================================================================================
 *
 *       Filename:  item.h
 *
 *    Description:  Modeling object representation for users and ads
 *                  Currently modeling both users and ads as 'items'
 *
 *        Version:  1.0
 *       Compiler:  g++
 *
 *         Author:  Vishal Surana (), surana.vishal@gmail.com
 *
 * =====================================================================================
 */

#include <vector>
#include <map>

using namespace std;

class Item {
	public:

		int id;
		std::map<float, std::vector<int> > similaritems;
		std::vector<int> likes;
		std::vector<int> topN;
		std::vector<int> topNRecs;
		Item();
		Item(int);
		~Item();
};

Item::Item(int _id) {
	id = _id;
}

Item::~Item() {

 	likes.clear();
	similaritems.clear();
}
