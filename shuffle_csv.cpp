#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

constexpr size_t BUFFER_SIZE = 8192;

void index(std::istream &in, std::vector<size_t> &ind) {
    char buffer[BUFFER_SIZE];
    size_t i = 0;
    size_t line = 0;
    ind.push_back(i);
    do {
        in.read(buffer, sizeof(buffer));
        for (size_t j = 0; j < in.gcount(); ++j) {
            ++i;
            if (buffer[j] == '\n') {
                ind.push_back(i);
                ++line;
            }
        }
    } while (in);
    in.clear();
}

void rearrange(std::istream &in, std::ostream &out, const std::vector<size_t> &ind) {
    char buffer[BUFFER_SIZE];
    size_t line = 0;
    for (auto &&i : ind) {
        in.seekg(i);
        size_t length = 0;
        bool done = false;
        do {
            in.read(buffer, sizeof(buffer));
            size_t count = in.gcount();
            for (size_t j = 0; j < count; ++j) {
                if (buffer[j] != '\n') {
                    continue;
                }
                count = j;
                ++line;
                done = true;
                break;
            }
            length += count;
            out.write(buffer, count);
        } while (in && !done);
        in.clear();
        if (length > 0) {
            out << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    std::ifstream in;
    std::ofstream out;
    
    std::cout << "open " << argv[1] << std::endl;
    in.open(argv[1], std::ios::in | std::ios::binary);
    
    std::cout << "index" << std::endl;
    std::vector<size_t> ind;
    index(in, ind);
    
    std::cout << "shuffle" << std::endl;
    // skip first line
    std::random_shuffle(ind.begin() + 1, ind.end());
    
    std::cout << "write" << std::endl;
    out.open(argv[2], std::ios::out | std::ios::binary);
    rearrange(in, out, ind);
    
    std::cout << "close" << std::endl;
    out.close();
    in.close();

    return 0;
}

