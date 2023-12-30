#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
struct Activity {
    int avg;
    int level;
    std::string str;
};
bool operator<(Activity a, Activity b) {
    if(a.avg != b.avg) {
        return a.avg > b.avg;
    } else {
        if(a.level != b.level) {
            return a.level < b.level;
        } else {
            return a.str < b.str;
        }
    }
}
int main() {
//    std::freopen("test_in.txt", "r", stdin);
//    std::freopen("test_out.txt", "w", stdout);
    std::map<std::string, std::vector<int>> m;
    std::set<Activity> s;
    std::string type, instruct;
    while(true) {
        std::cin >> type;
        if(type == "end") {
            break;
        }
        if(type == "map") {
            std::cin >> instruct;
            if(instruct == "size") {
                std::cout << m.size() << std::endl;
            } else if(instruct == "insert") {
                std::string name;
                int num;
                std::vector<int> time;
                std::cin >> name >> num;
                for(int i = 0; i < num; i++) {
                    int arg;
                    std::cin >> arg;
                    time.push_back(arg);
                }
                if(m.find(name) != m.end()) {
                    std::cout << "Error" << std::endl;
                } else {
                    m[name] = time;
                }
                m[name] = time;
            } else if(instruct == "erase") {
                std::string name;
                std::cin >> name;
                if(m.find(name) == m.end()) {
                    std::cout << "Error" << std::endl;
                } else {
                    m.erase(name);
                }
            } else if(instruct == "count") {
                std::string name;
                std::cin >> name;
                std::cout << m.count(name) << std::endl;
            } else if(instruct == "traverse") {
                for(auto iter = m.begin(); iter != m.end(); iter++) {
                    std::string name = iter->first;
                    std::cout<< name;
                    for(auto iter0 = m[name].begin(); iter0 != m[name].end(); iter0++) {
                        std::cout << ' ' << *iter0;
                    }
                    std::cout << std::endl;
                }
            } else if(instruct == "visitVec") {
                std::string name;
                int index;
                std::cin >> name >> index;
                if(m.find(name) == m.end()) {
                    std::cout << "Error" << std::endl;
                } else {
                    if(m[name].size() <= index) {
                        std::cout << "Error" << std::endl;
                    } else {
                        std::cout << m[name][index] << std::endl;
                    }
                }
            } else if(instruct == "visit") {
                std::string name;
                std::cin >> name;
                if(m.find(name) == m.end()) {
                    std::cout << "Error" << std::endl;
                } else {
                    std::cout << m[name].size();
                    for(auto iter = m[name].begin(); iter != m[name].end(); iter++) {
                        std::cout << ' ' << *iter;
                    }
                    std::cout << std::endl;
                }
            } else if(instruct == "append") {
                std::string name;
                int element;
                std::cin >> name >> element;
                if(m.find(name) == m.end()) {
                    std::cout << "Error" << std::endl;
                } else {
                    m[name].push_back(element);
                }
            }
        } else if(type == "set") {
            std::cin >> instruct;
            if(instruct == "size") {
                std::cout << s.size() << std::endl;
            } else if(instruct == "insert") {
                std::string str;
                int avg, level;
//                bool flag = false;
                std::cin >> avg >> level >> str;
                Activity activity = {avg, level, str};
//                for(auto iter = s.begin(); iter != s.end(); iter++) {
//                    if(iter->str == str) {
//                        std::cout << "Error" << std::endl;
//                        flag = true;
//                        break;
//                    }
//                }
//                if(!flag) {
//                    s.insert(activity);
//                }
                if(s.count(activity) == 1) {
                    std::cout << "Error" << std::endl;
                } else {
                    s.insert(activity);
                }
            } else if(instruct == "erase") {
                std::string str;
                int avg, level;
                std::cin >> avg >> level >> str;
                Activity activity = {avg, level, str};
                if(s.count(activity) == 0) {
                    std::cout << "Error" << std::endl;
                } else {
                    s.erase(activity);
                }
            } else if(instruct == "count") {
                std::string str;
                int avg, level;
                std::cin >> avg >> level >> str;
                Activity activity = {avg, level, str};
                std::cout << s.count(activity) << std::endl;
            } else if(instruct == "traverse") {
                for(auto iter = s.begin(); iter != s.end(); iter++) {
                    std::cout << iter->avg << ' ' << iter->level << ' ' << iter->str << std::endl;
                }
            }
        }
    }
    return 0;
}
