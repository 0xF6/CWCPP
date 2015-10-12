// Архиватор с расширение " .ar "
#include "Int.h"
class Zipper{

private:
	vector<string> files;
	string path;
	string real_bin_file;
public:
	Zipper(vector<string> &vec, string p)
	{
		if (vec.size() > 0) files.assign(vec.begin(), vec.end());
		path = p + "\\";
		real_bin_file = path + "bin.ar";
	}
	void getInfo();
	void InCompress();
	void OutCompress(string binary);

	static string get_file_name(string fn){ return fn.substr(fn.find_last_of("\\") + 1, fn.size()); }
};