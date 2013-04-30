/*
 * =====================================================================================
 *
 *       Filename:  user.h
 *
 *    Description:
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Vishal Surana (), surana.vishal@gmail.com
 *   Organization:
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

/// Due to time constraints, I'm modeling both Ad and User objects as Items
/**
class User: public item {

}

class Ad: public item {


//	public:
//		std::string name;
}

*/
