#ifndef _READ_GRAPH_HH_
#define _READ_GRAPH_HH_


#include <iostream>
#include <ios>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <string>

#include <stdexcept>

#include <cerrno>
#include <cstring>

#include "graph.hh"
#include "label_list.hh"

void progress(double progress, unsigned int width=50, char label='#') {
	unsigned int ii;
	unsigned int count = progress * width;

	std::cerr << "\r[";
	for(ii=0; ii < count; ii++) {
		std::cerr << label;
	}
	for(; ii < width; ii++) {
		std::cerr << ' ';
	}
	std::cerr << "] ";

	std::cerr << std::setfill(' ') << std::setw(7) << std::fixed << std::setprecision(3) << (100*progress) << "%";
	if(progress == 1.0) {
		std::cerr << std::endl;
	}

	std::cerr.flush();
}

void read_graph(const std::string &filename, graph<std::string *> &graph, label_list<std::string> &labels) {
	std::ifstream file(filename.c_str());
	if(file) {
		unsigned int line_num;
		std::string line, src, edg, dst, period, tmp;

		std::streampos total, current;

		file.seekg(0, std::ios_base::end);
		total = file.tellg();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		current = file.tellg();
		line_num = 1;
		while(getline(file, line)) {
			current = file.tellg();

			if(line_num % 10000 == 0 || current == total) {
				progress(current/(double)total);
			}

			std::istringstream iss(line);
			if(!(iss >> src)) {
				std::ostringstream oss;
				oss << filename << ":" << line_num << ": error reading source vertex";
				throw std::runtime_error(oss.str());
			}
			else if(!(iss >> edg)) {
				std::ostringstream oss;
				oss << filename << ":" << line_num << ": error reading edge label";
				throw std::runtime_error(oss.str());
			}
			else if(!(iss >> dst)) {
				std::ostringstream oss;
				oss << filename << ":" << line_num << ": error reading destination vertex";
				throw std::runtime_error(oss.str());
			}
			else if(!(iss >> period)) {
				std::ostringstream oss;
				oss << filename << ":" << line_num << ": error reading reading end of record symbol";
				throw std::runtime_error(oss.str());
			}

			std::string *src_vertex = labels[src];
			std::string *edg_vertex = labels[edg];
			std::string *dst_vertex = labels[dst];

			graph.insert(src_vertex);
			graph.insert(dst_vertex);

			graph.insert(src_vertex, dst_vertex);

			line_num++;
		}

		if(file.bad()) {
			std::ostringstream oss;
			oss <<  filename << ": " << strerror(errno);

			throw std::runtime_error(oss.str());
		}
		else if(file.fail() && !file.eof()) {
			std::ostringstream oss;
			oss <<  filename << ": unexpected conversion error";

			throw std::runtime_error(oss.str());
		}
	}
	else {
		std::ostringstream oss;
		oss <<  filename << ": " << strerror(errno);

		throw std::runtime_error(oss.str());
	}
}

#endif



#include <iostream>

#include <string>

#include "graph.hh"

int main(int argc, char *argv[]) {
	label_list<std::string> labels;
	graph<std::string *> graph;
	
	read_graph("../data/semmedminer.nt", graph, labels);

	std::cout << graph << std::endl;

	return 0;
}
