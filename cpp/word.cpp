
#include	<iostream>
#include	<vector>
#include	<string>

using namespace std;
int main(void)
{
        string::size_type pos = 0, pos1 ;
        string::size_type minlen = 0, maxlen = 0, wordlen; /* count记录所在处单词长度 */
        size_t wordscount = 0;

        string line1 = " a We were her pride of 10 she named us:";
        string line2 = "Benjamin, phoenix, the Prodigal";
        string line3 = "and perspicacious pacific Suznanne a";

        string sentence = line1 + ' ' + line2 + ' ' + line3;

        string spapunct = "0123456789 \t,.:";
        vector<string> fuckword;

        while (pos < sentence.size()) {
                pos1 = sentence.find_first_of(spapunct, pos)) != string::npos;
                if (pos1 == pos) {
                pos++;
                continue;
        } else {
                wordscount++;
                fuckword.push_back(sentence.substr(pos, pos1 - pos));
                        pos = pos1;
                }
        }

        if (pos < sentence.size()) {
                fuckword.push_back(sentence.substr(pos, sentence.size() - pos));
        }

        for (vector<string>::const_iterator iter = fuckword.begin(); iter != fuckword.end(); iter++) {
                cout << *iter << endl;
        }

        return 0;
}

