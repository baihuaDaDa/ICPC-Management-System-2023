#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
struct Student {
    char gender;
    int class_number;
    std::vector<int> score;
    int sum_score;
    int rank;
};
std::unordered_map<std::string, Student> m;
struct cmp {
    bool operator()(const std::string & name1, const std::string & name2) const {
        if(m[name1].sum_score / 9 != m[name2].sum_score / 9) {
            return m[name1].sum_score / 9 > m[name2].sum_score / 9;
        } else {
            for(int i = 0; i < 9; i++) {
                if(m[name1].score[i] != m[name2].score[i]) {
                    return m[name1].score[i] > m[name2].score[i];
                }
            }
            return name1 < name2;
        }
    }
};

int main() {
//    std::freopen("10.in", "r", stdin);
//    std::freopen("10out.txt", "w", stdout);
    std::string instruct;
    std::set<std::string, cmp> s;
    std::vector<std::string> rank_list;
    int n = 0;
    bool if_start = false;
    while(true) {
        std::cin >> instruct;
        if(instruct == "END") {
            break;
        } else if(instruct == "ADD") {
            std::string name;
            char gender;
            int class_number,sum_score = 0;
            std::vector<int> score;
            std::cin >> name >> gender >> class_number;
            for(int i = 0; i < 9; i++) {
                int score_i;
                std::cin >> score_i;
                score.push_back(score_i);
                sum_score += score_i;
            }
            if(if_start) {
                std::cout << "[Error]Cannot add student now.\n";
            } else {
                if(m.find(name) != m.end()) {
                    std::cout << "[Error]Add failed.\n";
                } else {
                    n++;
                    Student student = {gender, class_number, score, sum_score, 0};
                    m[name] = student;
                    s.insert(name);
                }
            }
        } else if(instruct == "START") {
            if_start = true;
            int rank = 0;
            for(auto iter = s.begin(); iter != s.end(); iter++) {
                rank++;
                m[*iter].rank = rank;
                rank_list.push_back(*iter);
            }
        } else if(instruct == "UPDATE") {
            std::string name;
            int code, score;
            std::cin >> name >> code >> score;
            if(m.find(name) == m.end()) {
                std::cout << "[Error]Update failed.\n";
            } else {
                s.erase(name);
                Student student = m[name];
                student.sum_score -= student.score[code];
                student.score[code] = score;
                student.sum_score += student.score[code];
                m[name] = student;
                s.insert(name);
            }
        } else if(instruct == "FLUSH") {
            int rank = 0;
            for(auto iter = s.begin(); iter != s.end(); iter++) {
                rank++;
                m[*iter].rank = rank;
                rank_list[rank - 1] = *iter;
            }
        } else if(instruct == "QUERY") {
            std::string name;
            std::cin >> name;
            if(m.find(name) == m.end()) {
                std::cout << "[Error]Query failed.\n";
            } else {
                std::cout << "STUDENT " << name << " NOW AT RANKING " << m[name].rank << '\n';
            }
        } else if(instruct == "PRINTLIST") {
            for(auto iter = rank_list.begin(); iter != rank_list.end(); iter++) {
                Student student = m[*iter];
                std::cout << student.rank << ' ' << *iter << ' ';
                if(student.gender == 'M') {
                    std::cout << "male ";
                } else if(student.gender == 'F') {
                    std::cout << "female ";
                }
                std::cout << student.class_number << ' ' << student.sum_score / 9 << '\n';
            }
        }
    }
    return 0;
}