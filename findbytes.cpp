#include <boost/algorithm/string/find.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <iostream>
#include <iomanip>
using namespace boost;
using namespace boost::algorithm;
using namespace boost::iostreams;
namespace po = boost::program_options;
using namespace std;

int main(int argc, char **argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("pf", po::value<string>(), "pattern file name")
		("tf", po::value<string>(), "file to search")
		("ps", po::value<unsigned long>()->default_value(0UL), "offset into pattern file to start")
		("pe", po::value<unsigned long>()->default_value(~0UL), "offset into pattern file to end")
		("ts", po::value<unsigned long>()->default_value(0UL), "offset into target file to begin search")
		("te", po::value<unsigned long>()->default_value(~0UL), "offset into target file to end search")
		("verbose", "describe what is happening")
	;

	po::positional_options_description pd;
	pd.add("pf",1).add("tf",1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if (vm.count("help") != 0 || vm.count("pf") != 1 || vm.count("tf") != 1) {
		cerr << desc << std::endl;
		return 3;
	}

	string pf = vm["pf"].as<string>();
	string tf = vm["tf"].as<string>();

	mapped_file_source pattern(pf);
	mapped_file_source target(tf);

	unsigned long pattern_start = vm["ps"].as<unsigned long>();
	unsigned long pattern_end = min(pattern.size(), vm["pe"].as<unsigned long>());
	unsigned long target_start = vm["ts"].as<unsigned long>();
	unsigned long target_end = min(target.size(), vm["te"].as<unsigned long>());

	if (vm.count("verbose")) {
		cout << hex << setfill('0') << setw(8);

		cout << "pattern: " << pf << "(0x" << pattern.size() << ") off: 0x" << pattern_start << " to: 0x" << pattern_end << "\n";
		cout << "target: " << tf << "(0x" << target.size() << ") off: 0x" << target_start << " to: 0x" << target_end << endl;

		cout << dec;
	}

	iterator_range<const char*> p_range(pattern.data() + pattern_start, pattern.data() + pattern_end);
	iterator_range<const char*> s_range(target.data() + target_start, target.data() + target_end);
	iterator_range<const char*> result = find_first(s_range, p_range);
	if (result) {
		size_t pos = result.begin()-s_range.begin();
		cout << pos+target_start << '\n';
		return 0;
	}
	return 1;
}
