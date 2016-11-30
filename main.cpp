#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>

using pos = std::pair<size_t,size_t>;

namespace std {
    template<>
    struct hash<pos> {
        size_t operator()(const pos & p) const {
            return std::hash<size_t>()(p.first)^std::hash<size_t>()(p.second);
        }
    };
}

namespace std {
    std::ostream & operator<<(std::ostream & os, const pos & p) {
        os << "(" << p.first << "," << p.second << ")";
        return os;
    }
}

class solution {
  public:
    void solve(std::vector<std::string> & board) {
        auto regions = findRegions(board);
    }

  private:
    std::vector<std::unordered_set<pos>> findRegions(const std::vector<std::string> & board) {
        std::vector<pos> starting;
        for (size_t i=0; i < board.size(); i++) {
            for (size_t j=0; j < board[0].size(); j++)
                if (board[i][j] == 'O')
                    starting.push_back(std::make_pair(i,j));
        }
        
        std::vector<std::unordered_set<pos>> regions;
        for (auto & p : starting) {
            if (!exist(regions, p)) {
                std::unordered_set<pos> region = grow(board, std::unordered_set<pos>{p});
                regions.push_back(region);
                std::copy(region.begin(), region.end(), std::ostream_iterator<pos>(std::cout, " "));
                std::cout << std::endl;
            }
        }
        
        return regions;
    }

    bool exist(const std::vector<std::unordered_set<pos>> & regions, const pos & p) {
        return std::any_of(regions.begin(), regions.end(),
                           [&](const std::unordered_set<pos> & s){
                               return std::any_of(s.begin(), s.end(),
                                                  [&](const pos & ps){
                                                      return ps == p;
                                                  });
                           });
    }

    std::unordered_set<pos> grow(const std::vector<std::string> & board, const std::unordered_set<pos> & curr) {
        std::unordered_set<pos> result;
        auto adj = getAdj(board, curr);
        if (adj.empty())
            result = curr;
        else {
            auto cpy = curr;
            std::copy(adj.begin(), adj.end(), std::inserter(cpy, cpy.end()));
            auto temp = grow(board, cpy);
            std::copy(temp.begin(), temp.end(), std::inserter(result, result.end()));
        }
        return result;
    }

    std::unordered_set<pos> getAdj(const std::vector<std::string> & board, const std::unordered_set<pos> & curr) {
        std::unordered_set<pos> adj;
        for (auto it = curr.begin(); it != curr.end(); ++it) {
            if (it->first > 0 && board[it->first-1][it->second] == 'O' && curr.find(std::make_pair(it->first-1,it->second)) == curr.end())
                adj.insert(std::make_pair(it->first-1, it->second));
            if (it->first < board.size()-1 && board[it->first+1][it->second] == 'O' && curr.find(std::make_pair(it->first+1,it->second)) == curr.end())
                adj.insert(std::make_pair(it->first+1, it->second));
            if (it->second > 0 && board[it->first][it->second-1] == 'O'&& curr.find(std::make_pair(it->first,it->second-1)) == curr.end())
                adj.insert(std::make_pair(it->first, it->second-1));
            if (it->second < board[0].size()-1 && board[it->first][it->second+1] == 'O' && curr.find(std::make_pair(it->first,it->second+1)) == curr.end())
                adj.insert(std::make_pair(it->first, it->second+1));
        }
        return adj;
    }
    
};

int main() {
    std::vector<std::string> board{
	"XXXX",
	    "XOOX",
	    "XXOX",
	    "XOXX"
    };
    solution soln;
    soln.solve(board);
}
