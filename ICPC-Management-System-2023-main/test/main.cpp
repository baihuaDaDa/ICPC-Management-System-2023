//ICPC Management System
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>
#include <cassert>

struct Submit {
    char problem_name;
    int time;
    Submit() {
        problem_name = ' ';
        time = 0;
    }
};
struct Submits {
    Submit history[4]; // 0 for AC, 1 for WA, 2 for RE, 3 for TLE
    int last_submit_index;
    Submits() {
        last_submit_index = -1;
    }
};
struct Problem {
    int sub_num, sub_num_freeze, penalty, pass_time, sub_num_freeze_all;
    bool frozen, pass_freeze;
    Submits submits;
    Problem() {
        sub_num = 0;
        sub_num_freeze = 0;
        penalty = 0;
        pass_time = 0;
        sub_num_freeze_all = 0;
        frozen = false;
        pass_freeze = false;
    }
};
struct Team {
    std::string name;
    int solved, penalty, freeze_length;
    bool erased;
    std::vector<int> solved_time;
    std::vector<Problem> problems;
    Submits submits;
    Team() {
        name = "";
        solved = 0;
        penalty = 0;
        freeze_length = 0;
        erased = false;
    }
};

bool start = false, freeze = false;
std::unordered_map<std::string, int> find_index = {{"Accepted", 0}, {"Wrong_Answer", 1},
                                                   {"Runtime_Error", 2}, {"Time_Limit_Exceed", 3}};
std::unordered_map<int, std::string> find_status = {{0, "Accepted"}, {1, "Wrong_Answer"},
                                                    {2, "Runtime_Error"}, {3, "Time_Limit_Exceed"}};
std::vector<Team> teams;
std::unordered_map<std::string, int> find_team;
std::vector<int> rank;
std::vector<int> erased_team;
std::vector<int> rank_inverse;
struct Cmp {
    bool operator()(const int &lhs, const int &rhs) const {
        if (teams[lhs].solved != teams[rhs].solved) {
            return teams[lhs].solved > teams[rhs].solved;
        }
        if (teams[lhs].penalty != teams[rhs].penalty) {
            return teams[lhs].penalty < teams[rhs].penalty;
        }
        auto iter_l = teams[lhs].solved_time.rbegin();
        auto iter_r = teams[rhs].solved_time.rbegin();
        while (iter_l != teams[lhs].solved_time.rend()) {
            if (*iter_l != *iter_r) {
                return *iter_l < *iter_r;
            }
            iter_l++;
            iter_r++;
        }
        return teams[lhs].name < teams[rhs].name;
    }
};
std::set<int, Cmp> live_rank;

bool CmpByName(const int &lhs, const int &rhs) {
    return teams[lhs].name < teams[rhs].name;
}
bool CmpErasedTeam(const int &lhs, const int &rhs) {
    return Cmp()(lhs, rhs);
}
void InsertSolvedTime(std::vector<int> &solved_time, int time, int st) {
    int i, insert_index, lo, hi, mid;
    if (! freeze) {
        solved_time.push_back(time);
        return;
    }
    if (solved_time.empty() || st == solved_time.size() || time < solved_time[st]) {
        insert_index = st;
    }
    else if (time >= solved_time.back()) {
        insert_index = solved_time.size();
    }
    else {
        lo = st;
        hi = solved_time.size() - 1;
        while (hi > lo + 1) {
            mid = (hi + lo) / 2;
            if (solved_time[mid] <= time) {
                lo = mid;
            }
            else {
                hi = mid;
            }
        }
        insert_index = hi;
    }
    solved_time.push_back(0);
    for (i = solved_time.size() - 1; i > insert_index; i--) {
        solved_time[i] = solved_time[i - 1];
    }
    solved_time[insert_index] = time;
}
void Flush(bool print = true) {
    int ranking, i1 = 0, i2 = 0;
    std::vector<int> new_rank, empty;
    if (print) {
        std::cout << "[Info]Flush scoreboard.\n";
    }
    std::sort(erased_team.begin(), erased_team.end(), CmpErasedTeam);
    while (i1 < rank.size() && i2 < erased_team.size()) {
        if (teams[rank[i1]].erased) {
            i1++;
            continue;
        }
        if (CmpErasedTeam(rank[i1], erased_team[i2])) {
            new_rank.push_back(rank[i1]);
            i1++;
        }
        else {
            new_rank.push_back(erased_team[i2]);
            i2++;
        }
    }
    if (i1 == rank.size()) {
        while (i2 < erased_team.size()) {
            new_rank.push_back(erased_team[i2]);
            i2++;
        }
    }
    else {
        while (i1 < rank.size()) {
            if (teams[rank[i1]].erased) {
                i1++;
                continue;
            }
            new_rank.push_back(rank[i1]);
            i1++;
        }
    }
    std::swap(rank, new_rank);
    std::swap(erased_team, empty);
    for (Team &team : teams) {
        team.erased = false;
    }
    for (ranking = 1; ranking <= rank.size(); ranking ++) {
        rank_inverse[rank[ranking - 1]] = ranking - 1;
    }
}
void AddTeam() {
    Team new_team;
    std::cin >> new_team.name;
    if (start) {
        std::cout << "[Error]Add failed: competition has started.\n";
        return;
    }
    if (find_team.count(new_team.name)) {
        std::cout << "[Error]Add failed: duplicated team name.\n";
        return;
    }
    std::cout << "[Info]Add successfully.\n";
    teams.push_back(new_team);
    find_team.insert(std::make_pair(new_team.name, teams.size() - 1));
}
void Start() {
    int problem_count, i;
    std::string trash;
    Problem problem;
    std::cin >> trash >> trash >> trash >> problem_count;
    if (start) {
        std::cout << "[Error]Start failed: competition has started.\n";
        return;
    }
    std::cout << "[Info]Competition starts.\n";
    start = true;
    for (i = 0; i < teams.size(); i++) {
        teams[i].problems.assign(problem_count, problem);
        rank.push_back(i);
    }
    std::sort(rank.begin(), rank.end(), CmpByName);
    rank_inverse.assign(teams.size(),0);
    Flush(false);
}
void ChangeSubmit(Submit &sub, char problem_name, int submit_time) {
    sub.problem_name = problem_name;
    sub.time = submit_time;
}
void SubmitProblem() {
    char problem_name;
    std::string team_name, submit_status, trash;
    int submit_time, team_index, status_index;
    std::cin >> problem_name >> trash >> team_name >> trash >> submit_status >> trash >> submit_time;
    team_index = find_team[team_name];
    status_index = find_index[submit_status];
    Team &team = teams[team_index];
    Problem &problem = team.problems[problem_name - 'A'];
    ChangeSubmit(team.submits.history[status_index], problem_name, submit_time);
    team.submits.last_submit_index = status_index;
    ChangeSubmit(problem.submits.history[status_index], problem_name, submit_time);
    problem.submits.last_submit_index = status_index;
    if (freeze) {
        problem.sub_num_freeze_all++;
    }
    if (problem.pass_time == 0) {
        if (status_index == 0) {
            problem.penalty = (problem.sub_num + problem.sub_num_freeze) * 20 + submit_time;
            problem.pass_time = submit_time;
            problem.pass_freeze = freeze;
        }
        if (freeze) {
            problem.frozen = true;
            if (status_index > 0) {
                problem.sub_num_freeze++;
            }
        }
        else {
            if (status_index == 0) {
                team.penalty += problem.penalty;
                team.solved++;
                InsertSolvedTime(team.solved_time, submit_time, team.freeze_length);
                if (! team.erased) {
                    team.erased = true;
                    erased_team.push_back(team_index);
                }
            }
            else {
                problem.sub_num++;
            }
        }
    }
}
void Freeze() {
    if (freeze) {
        std::cout << "[Error]Freeze failed: scoreboard has been frozen.\n";
        return;
    }
    std::cout << "[Info]Freeze scoreboard.\n";
    freeze = true;
    for (Team &team : teams) {
        team.freeze_length = team.solved_time.size();
    }
}
void PrintProblem(Problem &problem) {
    std::cout << " ";
    if (! problem.frozen) {
        if (problem.pass_time > 0) {
            std::cout << "+";
            if (problem.sub_num > 0) {
                std::cout << problem.sub_num;
            }
        }
        else {
            if (problem.sub_num > 0) {
                std::cout << "-" << problem.sub_num;
            }
            else {
                std::cout << ".";
            }
        }
    }
    else {
        if (problem.sub_num > 0) {
            std::cout << "-" << problem.sub_num << "/" << problem.sub_num_freeze_all;
        }
        else {
            std::cout << "0/" << problem.sub_num_freeze_all;
        }
    }
}
void PrintList() {
    int ranking, i;
    for (ranking = 1; ranking <= rank.size(); ranking++) {
        Team &team = teams[rank[ranking - 1]];
        std::cout << team.name << " " << ranking << " " << team.solved << " " << team.penalty;
        for (i = 0; i < team.problems.size(); i++) {
            PrintProblem(team.problems[i]);
        }
        std::cout << "\n";
//        for (i = 0; i < team.solved_time.size(); i++) {
//            std::cout << team.solved_time[i] << ' ';
//        }
//        std::cout << '\n';
    }
}
void Scroll() {
    int i, team_index, problem_index;
    bool find_frozen_problem;
    if (! freeze) {
        std::cout << "[Error]Scroll failed: scoreboard has not been frozen.\n";
        return;
    }
    std::cout << "[Info]Scroll scoreboard.\n";
    Flush(false);
    PrintList();
    for (i = 0; i < teams.size(); i++) {
        live_rank.insert(i);
    }
    while (! live_rank.empty()) {
        auto last_iter = live_rank.end();
        last_iter--;
        find_frozen_problem = false;
        for (i = 0; i < teams[*last_iter].problems.size(); i++) {
            if (teams[*last_iter].problems[i].frozen) {
                team_index = *last_iter;
                problem_index = i;
                find_frozen_problem = true;
                break;
            }
        }
        if (! find_frozen_problem) {
            live_rank.erase(last_iter);
            continue;
        }
        Team &team = teams[team_index];
        Problem &problem = team.problems[problem_index];
        auto next_iter = live_rank.erase(live_rank.find(team_index));
        problem.frozen = false;
        problem.sub_num += problem.sub_num_freeze;
        problem.sub_num_freeze = 0;
        problem.sub_num_freeze_all = 0;
        if (problem.pass_freeze) {
            team.penalty += problem.penalty;
            team.solved++;
            InsertSolvedTime(team.solved_time, problem.pass_time, team.freeze_length);
        }
        problem.pass_freeze = false;
        auto now_next_iter = std::next(live_rank.insert(team_index).first);
        if (now_next_iter != next_iter) {
            std::cout << team.name << " " << teams[*now_next_iter].name << " " << team.solved << " " << team.penalty << "\n";
            if (! teams[team_index].erased) {
                teams[team_index].erased = true;
                erased_team.push_back(team_index);
            }
        }
    }
    Flush(false);
    PrintList();
    freeze = false;
}
void QueryRanking() {
    std::string team_name;
    std::cin >> team_name;
    if (! find_team.count(team_name)) {
        std::cout << "[Error]Query ranking failed: cannot find the team.\n";
        return;
    }
    std::cout << "[Info]Complete query ranking.\n";
    if (freeze) {
        std::cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.\n";
    }
    std::cout << team_name << " NOW AT RANKING " << rank_inverse[find_team[team_name]] + 1 << "\n";
}
void QuerySubmission() {
    std::string team_name, trash, problem_name, status;
    Submit *history;
    int status_index;
    std::cin >> team_name >> trash >> problem_name >> trash >> status;
    if (! find_team.count(team_name)) {
        std::cout << "[Error]Query submission failed: cannot find the team.\n";
        return;
    }
    std::cout << "[Info]Complete query submission.\n";
    problem_name = problem_name.substr(8);
    status = status.substr(7);
    Team &team = teams[find_team[team_name]];
    if (problem_name == "ALL") {
        history = team.submits.history;
        status_index = team.submits.last_submit_index;
    }
    else {
        history = team.problems[problem_name[0]-'A'].submits.history;
        status_index = team.problems[problem_name[0]-'A'].submits.last_submit_index;
    }
    if (status == "ALL") {
        if (status_index == -1) {
            std::cout << "Cannot find any submission.\n";
        }
        else {
            std::cout << team_name << " " << history[status_index].problem_name << " " << find_status[status_index] << " ";
            std::cout << history[status_index].time << "\n";
        }
    }
    else {
        Submit &submit = history[find_index[status]];
        if (submit.time == 0) {
            std::cout << "Cannot find any submission.\n";
        }
        else {
            std::cout << team_name << " " << submit.problem_name << " " << status << " " << submit.time << "\n";
        }
    }
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    freopen("7.in","r",stdin);
    freopen("my_output.out","w",stdout);
    std::string op;
    while (true) {
        std::cin >> op;
        if (op == "ADDTEAM") {
            AddTeam();
        }
        else if (op == "START") {
            Start();
        }
        else if (op == "SUBMIT") {
            SubmitProblem();
        }
        else if (op == "FLUSH") {
            Flush();
        }
        else if (op == "FREEZE") {
            Freeze();
        }
        else if (op == "SCROLL") {
            Scroll();
        }
        else if (op == "QUERY_RANKING") {
            QueryRanking();
        }
        else if (op == "QUERY_SUBMISSION") {
            QuerySubmission();
        }
        else if (op == "END") {
            std::cout << "[Info]Competition ends.\n";
            break;
        }
    }
    return 0;
}