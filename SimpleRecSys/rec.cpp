/*
 * =====================================================================================
 *
 *       Filename:  rec.cpp
 *
 *    Description:  A recommendation system that suggests products/ads using
 *                  techniques from collaborative filtering
 *
 *        Version:  1.0
 *       Compiler:  g++
 *
 *         Author:  Vishal Surana (), surana.vishal@gmail.com
 *
 * =====================================================================================
 */
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <set>

#include "item.h"

typedef Item Ad;
typedef Item User;

map<int, string> names;
vector<Ad> ads;

using namespace std;

/// Assumes that lists are sorted
int commonElements(const vector<int> &s1, const vector<int> &s2) {

	int common = 0;

	int i=0, j=0;
	while(i<s1.size() && j<s2.size()) {
		if(s1[i] == s2[j]){
			printf("%d ", s1[i]);
		 	common++;
			i++;
			j++;
		}
		else if(s1[i]>s2[j]) j++;
		else i++;
	}

	return common;
}

int avgSimilarityScore(const vector<Item> &profiles) {

	int sum = 0, num = 0, val;
	int n = profiles.size();

	int i,j;
	for (i = 0; i < n; i++) {
		for (j = i+1; j < n; j++) {
			printf("\n%d and %d: ", i, j);
			val = commonElements(profiles[i].likes, profiles[j].likes);
			if(val) {
				sum += val;
				num += 1;
			}
		}
	}

	/// For computing average, we only consider those pairs where at least one item is similar
	int avg; /// we round off to an integer

	if(2*sum % num == 0)		avg = 2*sum/num;
	else		avg = 2*sum/num + 1;

	return avg;
}

void computeSimilarProfiles(vector<Item> &profiles, int avg) {

	int i, j, n, common = 0;
	n = profiles.size();
	float score = 0.0;

	for (i = 0; i < n; i++) {
		for (j = i+1; j < n; j++) {
			common = commonElements(profiles[i].likes, profiles[j].likes);
			if(common >= avg) score = 1.0;
			else score = float(common)/avg;
			if(score>0) {
				profiles[i].similaritems[score].push_back(j);
				profiles[j].similaritems[score].push_back(i);
			}
		}
	}

}
/// If N is too large, then the result will have < N elements
bool computeTopN(vector<Item> &profiles, int N) {

	int i, j, k;
	int n = profiles.size();
	float score = 0.0;

	for (i = 0; i < n; i++) {
		k = 0;
		printf("Top similar Profiles to %d: ", i);
		map<float, vector<int> >::reverse_iterator rit = profiles[i].similaritems.rbegin();
		for (; k<N && rit != profiles[i].similaritems.rend(); rit++) {
			score = rit->first;
			//printf("%f", score);
			for (j = 0; k<N && j < profiles[i].similaritems[score].size(); j++) {
				profiles[i].topN.push_back(profiles[i].similaritems[score][j]);
				printf("%d ", profiles[i].similaritems[score][j]);
				k++;
			}
		}
		printf("\n");
	}

	//// were we able to find the desired number?
	return (k>=N);
}


void computeTopNSimilarRecs(vector<Item> &profiles, int N) {

	int i, j, k;
	int n = profiles.size();
	float score = 0.0;
	FILE *fout;
	fout = fopen("similarAds.csv", "wb");
	fprintf(fout, "User,AdID,AdName,...", i);
	int m = 2;

	for (i = 0; i < n; i++) {

		set<int> recs;
		for (j = 0; j < profiles[i].likes.size(); j++) {
			recs.insert(ads[profiles[i].likes[j]].topN.begin(), ads[profiles[i].likes[j]].topN.end());
		}

		k=0;
		profiles[i].topNRecs.clear();
		printf("\nTop recommendations to %d: ", i);
		fprintf(fout, "\n%d", i);

		/// Remove items that have already been recommended
		for (j = 0; j < profiles[i].likes.size(); j++) {
			recs.erase(profiles[i].likes[j]);
		}

		set<int>::iterator it;

		while(k < N && !recs.empty()) {
			it = recs.begin();
			advance(it, rand()%recs.size());
			profiles[i].topNRecs.push_back(*it);
			printf("%d,%s ", *it, names[*it].c_str());
			fprintf(fout, ",%d,%s", *it, names[*it].c_str());
			recs.erase(*it);
			k++;
		}
		printf("\n");
		recs.clear();
	}
	fclose(fout);
}


void computeTopNRecs(vector<Item> &profiles, int N) {

	int i, j, k, N1, m;
	int n = profiles.size();
	float score = 0.0;

	FILE *fout;
	fout = fopen("similarUsers.csv", "wb");
	fprintf(fout, "User,AdID,AdName,...", i);

	for (i = 0; i < n; i++) {

		set<int> recs(profiles[i].likes.begin(), profiles[i].likes.end());
		N1 = N + recs.size();
		m = 5;
		while(computeTopN(profiles, m)) {

			for (j = 0; j < profiles[i].topN.size(); j++) {
				set<int> temp(profiles[profiles[i].topN[j]].likes.begin(), profiles[profiles[i].topN[j]].likes.end());
				recs.insert(temp.begin(), temp.end());
			}

			/// Perahps we need to find more similar profiles
			if(recs.size() < N1) 	m += 2;
			else 	break;
		}

		/// Remove items that have already been recommended
		for (j = 0; j < profiles[i].likes.size(); j++) {
			recs.erase(profiles[i].likes[j]);
		}

		k=0;
		printf("\nTop recommendations to %d: ", i);
		fprintf(fout, "\n%d", i);
		profiles[i].topNRecs.clear();

		set<int>::iterator it = recs.begin();
		while(k < N && !recs.empty()) {
			it = recs.begin();
			advance(it, rand()%recs.size());
			profiles[i].topNRecs.push_back(*it);
			printf("%d,%s ", *it, names[*it].c_str());
			fprintf(fout, ",%d,%s", *it, names[*it].c_str());
			recs.erase(*it);
			k++;
		}
		printf("\n");
		recs.clear();
	}
	fclose(fout);
}

int main(int argc, char* argv[]) {

	if(argc != 4) {
		printf("Command: Executable user-file ad-file items-to-recommend");
	}
	else {

		srand(time(NULL));

		FILE *fin;
		char tname[100], tword[100];
		int tuser, tad, cur=-1;

		vector<User> users;

		fin = fopen(argv[2], "rb");
		fscanf(fin, "%s\n", tword); ///ignore first line

		while(fscanf(fin, "%d,%s\n", &tad, tname) == 2) {
			/// assuming that all the characters in all the words are of the same case
			ads.push_back(Ad(tad));
			names[tad] = (string)tname;
		}
		fclose(fin);


		fin = fopen(argv[1], "rb");
		fscanf(fin, "%s\n", tword); ///ignore first line
		while(fscanf(fin, "%d,%d\n", &tuser, &tad) == 2) {
			/// assuming that all the characters in all the words are of the same case
			if(cur < tuser) {
			 	cur = tuser;
				users.push_back(User(tuser));
			}
			users[tuser].likes.push_back(tad);
			ads[tad].likes.push_back(tuser);
		}
		fclose(fin);

		int i, j;

		/// We sort the list of ads/users
		/// Not necessary, but can make things easier if data is large
		for (i = 0; i < users.size(); i++) {
			sort(users[i].likes.begin(), users[i].likes.end());
		}

		for (j = 0; j < ads.size(); j++) {
			sort(ads[j].likes.begin(), ads[j].likes.end());
		}

		printf("Computing similarities");

		int N;
		sscanf(argv[3], "%d", &N);

		int avgUserSimilarity = avgSimilarityScore(users);
		computeSimilarProfiles(users, avgUserSimilarity);
		computeTopN(users, N);

		int avgAdSimilarity = avgSimilarityScore(ads);
		computeSimilarProfiles(ads, avgAdSimilarity);
		computeTopN(ads, N);

		/// Find similar users
		computeTopNRecs(users, N);

		/// Find similar ads
		computeTopNSimilarRecs(users, N);

		printf("\nAvg User Similarity: %d\n", avgUserSimilarity);
		printf("\nAvg Ad Similarity: %d\n", avgAdSimilarity);
	}

	return 1;
}

