#include <iostream>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

struct Problem {
    int last_submit_status = 0;
    bool if_frozen = false;
    int error_number = 0;
    int submit_time = 0;
    int submit_time_of_each_status[4] = {};

    Problem(int status0, bool if_frozen0, int error_number0, int submit_time0) {
        last_submit_status = status0;
        if_frozen = if_frozen0;
        error_number = error_number0;
        submit_time = submit_time0;
    }
};

struct FrozenProblem {
    int submit_number = 0;
    int error_number = 0;
    int submit_time = 0;

    FrozenProblem(int submit_number0, int error_number0, int submit_time0) {
        submit_number = submit_number0;
        error_number = error_number0;
        submit_time = submit_time0;
    }
};

struct Team {
    int rank;
    int solved_number = 0;
    int solved_number_before_freeze = 0;
    int total_penalty_time = 0;
    char last_submit_problem_name = '\0';
    char last_submit_problem_name_of_each_status[4] = {};
    std::vector<Problem> problem_set;
    std::vector<FrozenProblem> frozen_problem_set;

//    struct CompareProblem {
//        bool operator()(const int &time1, const int &time2) const {
//            return time1 > time2;
//        }
//    };
//
//    std::multiset<int, CompareProblem> submit_time_rank_list;
//    使用二分查找优化multiset
////  后来改成了用sort排序

    std::vector<int> submit_time_rank_list;
};

std::unordered_map<std::string, Team> team_list;

struct CompareTeam {
    bool operator()(const std::string &team_name1, const std::string &team_name2) const {
        Team &team1 = team_list[team_name1], &team2 = team_list[team_name2];
        if (team1.solved_number != team2.solved_number) {
            return team1.solved_number > team2.solved_number;
        } else if (team1.total_penalty_time != team2.total_penalty_time) {
            return team1.total_penalty_time < team2.total_penalty_time;
        } else {
            int i = team1.solved_number - 1, j = team2.solved_number - 1;
            while (i >= 0) {
                if (team1.submit_time_rank_list[i] != team2.submit_time_rank_list[j]) {
                    return team1.submit_time_rank_list[i] < team2.submit_time_rank_list[j];
                }
                i--;
                j--;
            }
        }
        return team_name1 < team_name2;
    }
};

//void binary_search(int submit_time, int l, int r, std::vector<int> & submit_time_rank_list) {
//    if (r == -1) {
//        submit_time_rank_list.push_back(submit_time);
//    } else if (r == 0) {
//        if (submit_time >= submit_time_rank_list[0]) {
//            submit_time_rank_list.insert(submit_time_rank_list.begin(), submit_time);
//        } else {
//            submit_time_rank_list.push_back(submit_time);
//        }
//    } else if (r == 1) {
//        if (submit_time >= submit_time_rank_list[0]) {
//            submit_time_rank_list.insert(submit_time_rank_list.begin(), submit_time);
//        } else if (submit_time <= submit_time_rank_list[1]) {
//            submit_time_rank_list.push_back(submit_time);
//        } else {
//            submit_time_rank_list.insert(submit_time_rank_list.begin() + 1, submit_time);
//        }
//    } else {
//        while (l < r - 1) {
//            int mid = (l + r) / 2;
//            if (submit_time_rank_list[mid] == submit_time) {
//                submit_time_rank_list.insert(submit_time_rank_list.begin() + mid, submit_time);
//                return;
//            } else if (submit_time_rank_list[mid] < submit_time) {
//                r = mid;
//            } else {
//                l = mid;
//            }
//        }
////    std::cout << l << ' ' << r << std::endl;
//        submit_time_rank_list.insert(submit_time_rank_list.begin() + r, submit_time);
//    }
//}
//二分优化 但是WA了四个点

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    freopen("bigger.in", "r", stdin);
    freopen("biggerout.txt", "w", stdout);
    std::set<std::string, CompareTeam> rank_list;
    bool if_start = false, if_frozen = false;
    std::string instruction;
    int duration_time, problem_count;
    while (true) {
        std::cin >> instruction;
        if (instruction == "END") {
            std::cout << "[Info]Competition ends.\n";
            break;
        } else if (instruction == "ADDTEAM") {
            std::string team_name;
            std::cin >> team_name;
            if (if_start) {
                std::cout << "[Error]Add failed: competition has started.\n";
            } else {
                if (team_list.find(team_name) != team_list.end()) {
                    std::cout << "[Error]Add failed: duplicated team name.\n";
                } else {
                    std::cout << "[Info]Add successfully.\n";
                    team_list[team_name];
                    rank_list.insert(team_name);
                }
            }
        } else if (instruction == "START") {
            int temp1, temp2;
            std::cin >> instruction;
            std::cin >> temp1;
            std::cin >> instruction;
            std::cin >> temp2;
            if (if_start) {
                std::cout << "[Error]Start failed: competition has started.\n";
            } else {
                std::cout << "[Info]Competition starts.\n";
                duration_time = temp1;
                problem_count = temp2;
                if_start = true;
                for (auto iter = team_list.begin(); iter != team_list.end(); iter++) {
                    iter->second.problem_set.assign(problem_count, Problem(0, false, 0, 0));
                    iter->second.frozen_problem_set.assign(problem_count, FrozenProblem(0, 0, 0));
                }
                int rank = 0;
                for (auto iter = rank_list.begin(); iter != rank_list.end(); iter++) {
                    rank++;
                    team_list[*iter].rank = rank;
                }
            }
        } else if (instruction == "FREEZE") {
            if (if_frozen) {
                std::cout << "[Error]Freeze failed: scoreboard has been frozen.\n";
            } else {
                std::cout << "[Info]Freeze scoreboard.\n";
                if_frozen = true;
                for (auto iter = team_list.begin(); iter != team_list.end(); iter++) {
                    iter->second.solved_number_before_freeze = iter->second.solved_number;
                }
            }
        } else if (instruction == "SUBMIT") {
            char problem_name;
            std::string team_name, submit_status;
            int time;
            std::cin >> problem_name >> instruction >> team_name
                     >> instruction >> submit_status >> instruction >> time;
            Team &team = team_list[team_name];
            team.last_submit_problem_name = problem_name;
            int problem_code = problem_name - 'A';
            Problem &problem = team.problem_set[problem_code];
            FrozenProblem &frozen_problem = team.frozen_problem_set[problem_code];
            if (submit_status == "Accepted") {
                team.last_submit_problem_name_of_each_status[0] = problem_name;
                problem.last_submit_status = 1;
                problem.submit_time_of_each_status[0] = time;
                if (if_frozen) {
                    if (problem.submit_time == 0) {
                        if (!problem.if_frozen) {
                            problem.if_frozen = true;
                        }
                        if (frozen_problem.submit_time == 0) {
                            frozen_problem.submit_time = time;
                        }
                        frozen_problem.submit_number++;
                    }
                } else {
                    if (problem.submit_time == 0) {
                        rank_list.erase(team_name);
                        problem.submit_time = time;
                        team.solved_number++;
                        team.total_penalty_time += problem.submit_time + problem.error_number * 20;
                        team.submit_time_rank_list.push_back(problem.submit_time);
                        rank_list.insert(team_name);
                    }
                }
            } else if (submit_status == "Wrong_Answer") {
                team.last_submit_problem_name_of_each_status[1] = problem_name;
                problem.last_submit_status = 2;
                problem.submit_time_of_each_status[1] = time;
                if (if_frozen) {
                    if (problem.submit_time == 0) {
                        if (!problem.if_frozen) {
                            problem.if_frozen = true;
                        }
                        frozen_problem.submit_number++;
                        if (frozen_problem.submit_time == 0) {
                            frozen_problem.error_number++;
                        }
                    }
                } else {
                    if (problem.submit_time == 0) {
                        problem.error_number++;
                    }
                }
            } else if (submit_status == "Runtime_Error") {
                team.last_submit_problem_name_of_each_status[2] = problem_name;
                problem.last_submit_status = 3;
                problem.submit_time_of_each_status[2] = time;
                if (if_frozen) {
                    if (problem.submit_time == 0) {
                        if (!problem.if_frozen) {
                            problem.if_frozen = true;
                        }
                        frozen_problem.submit_number++;
                        if (frozen_problem.submit_time == 0) {
                            frozen_problem.error_number++;
                        }
                    }
                } else {
                    if (problem.submit_time == 0) {
                        problem.error_number++;
                    }
                }
            } else if (submit_status == "Time_Limit_Exceed") {
                team.last_submit_problem_name_of_each_status[3] = problem_name;
                problem.last_submit_status = 4;
                problem.submit_time_of_each_status[3] = time;
                if (if_frozen) {
                    if (problem.submit_time == 0) {
                        if (!problem.if_frozen) {
                            problem.if_frozen = true;
                        }
                        frozen_problem.submit_number++;
                        if (frozen_problem.submit_time == 0) {
                            frozen_problem.error_number++;
                        }
                    }
                } else {
                    if (problem.submit_time == 0) {
                        problem.error_number++;
                    }
                }
            }
//            std::cout << problem.if_frozen << std::endl;
        } else if (instruction == "SCROLL") {
            if (if_frozen) {
                std::cout << "[Info]Scroll scoreboard.\n";
                int rank = 0;
                for (auto iter = rank_list.begin(); iter != rank_list.end(); iter++) {
                    rank++;
                    Team &team = team_list[*iter];
                    team.rank = rank;
                    std::cout << *iter << ' ' << rank << ' '
                              << team.solved_number << ' '
                              << team.total_penalty_time;
                    for (int i = 0; i < problem_count; i++) {
                        Problem &problem = team.problem_set[i];
                        if (problem.if_frozen) {
                            if (problem.error_number == 0) {
                                std::cout << " 0/";
                            } else {
                                std::cout << " -" << problem.error_number << '/';
                            }
                            std::cout << team.frozen_problem_set[i].submit_number;
                        } else {
                            if (problem.submit_time == 0) {
                                if (problem.error_number == 0) {
                                    std::cout << " .";
                                } else {
                                    std::cout << " -" << problem.error_number;
                                }
                            } else {
                                std::cout << " +";
                                if (problem.error_number != 0) {
                                    std::cout << problem.error_number;
                                }
                            }
                        }
                    }
                    std::cout << '\n';
//                    for (int i = 0; i < team.submit_time_rank_list.size(); i++) {
//                        std::cout << team.submit_time_rank_list[i] << ' ';
//                    }
//                    std::cout << '\n';
                }
                while (!rank_list.empty()) {
                    bool flag = false;
                    auto iter = --rank_list.end();
                    std::string team_name = *iter;
//                    std::cout << team_name << std::endl;
                    Team &team = team_list[team_name];
                    for (int i = 0; i < problem_count; i++) {
                        Problem &problem = team.problem_set[i];
                        if (problem.if_frozen) {
//                            std::cout << i << ' ';
                            FrozenProblem &frozen_problem = team.frozen_problem_set[i];
                            rank_list.erase(team_name);
//                            std::cout << problem.error_number << ' ' << frozen_problem.error_number << ' ' << i << std::endl;
                            problem.error_number += frozen_problem.error_number;
                            if (frozen_problem.submit_time != 0) {
                                problem.submit_time = frozen_problem.submit_time;
                                team.total_penalty_time += problem.submit_time + problem.error_number * 20;
                                team.submit_time_rank_list.push_back(frozen_problem.submit_time);
                                std::sort(team.submit_time_rank_list.begin() + team.solved_number_before_freeze, team.submit_time_rank_list.end());
                                team.solved_number++;
//                                std::cout << i << std::endl;
                            }
                            problem.if_frozen = false;
                            frozen_problem = FrozenProblem(0, 0, 0);
                            auto iter0 = rank_list.insert(team_name).first;
                            if (++iter0 != rank_list.end()) {
                                flag = true;
                                std::cout << team_name << ' ' << *iter0 << ' '
                                          << team.solved_number << ' ' << team.total_penalty_time << '\n';
                                break;
                            }
                        }
                    }
//                    std::cout << team.total_penalty_time << std::endl;
                    if (flag) {
                        continue;
                    }
                    rank_list.erase(team_name);
                }
                for (auto iter = team_list.begin(); iter != team_list.end(); iter++) {
                    Team &team = iter->second;
                    rank_list.insert(iter->first);
                }
                rank = 0;
                for (auto iter = rank_list.begin(); iter != rank_list.end(); iter++) {
                    rank++;
                    Team &team = team_list[*iter];
                    team.rank = rank;
                    std::cout << *iter << ' ' << rank << ' '
                              << team.solved_number << ' '
                              << team.total_penalty_time;
                    for (int i = 0; i < problem_count; i++) {
                        Problem problem = team.problem_set[i];
                        if (problem.submit_time == 0) {
                            if (problem.error_number == 0) {
                                std::cout << " .";
                            } else {
                                std::cout << " -" << problem.error_number;
                            }
                        } else {
                            std::cout << " +";
                            if (problem.error_number != 0) {
                                std::cout << problem.error_number;
                            }
                        }
                    }
                    std::cout << '\n';
//                    for (auto iter0 = team.submit_time_rank_list.begin(); iter0 != team.submit_time_rank_list.end(); iter0++) {
//                        std::cout << *iter0 << ' ';
//                    }
//                    std::cout << std::endl;
                }
                if_frozen = false;
            } else {
                std::cout << "[Error]Scroll failed: scoreboard has not been frozen.\n";
            }
        } else if (instruction == "FLUSH") {
            std::cout << "[Info]Flush scoreboard.\n";
            int rank = 0;
            for (auto iter = rank_list.begin(); iter != rank_list.end(); iter++) {
                rank++;
                team_list[*iter].rank = rank;
            }
        } else if (instruction == "QUERY_RANKING") {
            std::string team_name;
            std::cin >> team_name;
            auto iter = team_list.find(team_name);
            if (iter == team_list.end()) {
                std::cout << "[Error]Query ranking failed: cannot find the team.\n";
            } else {
                std::cout << "[Info]Complete query ranking.\n";
                if (if_frozen) {
                    std::cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.\n";
                }
                std::cout << team_name << " NOW AT RANKING " << iter->second.rank << '\n';
            }
        } else if (instruction == "QUERY_SUBMISSION") {
            std::string team_name, status;
            char problem_name;
            std::cin >> team_name >> instruction >> instruction;
            if (instruction.substr(8) == "ALL") {
                std::cin >> instruction >> instruction;
                status = instruction.substr(7);
                if (team_list.find(team_name) == team_list.end()) {
                    std::cout << "[Error]Query submission failed: cannot find the team.\n";
                } else {
                    Team &team = team_list[team_name];
                    std::cout << "[Info]Complete query submission.\n";
                    if (status == "ALL") {
                        if (team.last_submit_problem_name == '\0') {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << team.last_submit_problem_name << ' ';
                            Problem &problem = team.problem_set[int(team.last_submit_problem_name - 'A')];
                            if (problem.last_submit_status == 1) {
                                std::cout << "Accepted " << problem.submit_time_of_each_status[0] << '\n';
                            } else if (problem.last_submit_status == 2) {
                                std::cout << "Wrong_Answer " << problem.submit_time_of_each_status[1] << '\n';
                            } else if (problem.last_submit_status == 3) {
                                std::cout << "Runtime_Error " << problem.submit_time_of_each_status[2] << '\n';
                            } else if (problem.last_submit_status == 4) {
                                std::cout << "Time_Limit_Exceed " << problem.submit_time_of_each_status[3] << '\n';
                            }
                        }
                    } else if (status == "Accepted") {
                        if (team.last_submit_problem_name_of_each_status[0] == '\0') {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << team.last_submit_problem_name_of_each_status[0]
                                      << " Accepted "
                                      << team.problem_set[int(team.last_submit_problem_name_of_each_status[0] -
                                                              'A')].submit_time_of_each_status[0] << '\n';
                        }
                    } else if (status == "Wrong_Answer") {
                        if (team.last_submit_problem_name_of_each_status[1] == '\0') {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << team.last_submit_problem_name_of_each_status[1]
                                      << " Wrong_Answer "
                                      << team.problem_set[int(team.last_submit_problem_name_of_each_status[1] -
                                                              'A')].submit_time_of_each_status[1] << '\n';
                        }
                    } else if (status == "Runtime_Error") {
                        if (team.last_submit_problem_name_of_each_status[2] == '\0') {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << team.last_submit_problem_name_of_each_status[2]
                                      << " Runtime_Error "
                                      << team.problem_set[int(team.last_submit_problem_name_of_each_status[2] -
                                                              'A')].submit_time_of_each_status[2] << '\n';
                        }
                    } else if (status == "Time_Limit_Exceed") {
                        if (team.last_submit_problem_name_of_each_status[3] == '\0') {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << team.last_submit_problem_name_of_each_status[3]
                                      << " Time_Limit_Exceed "
                                      << team.problem_set[int(team.last_submit_problem_name_of_each_status[3] -
                                                              'A')].submit_time_of_each_status[3] << '\n';
                        }
                    }
                }
            } else {
                problem_name = instruction[8];
                std::cin >> instruction >> instruction;
                status = instruction.substr(7);
                if (team_list.find(team_name) == team_list.end()) {
                    std::cout << "[Error]Query submission failed: cannot find the team.\n";
                } else {
                    Team &team = team_list[team_name];
                    std::cout << "[Info]Complete query submission.\n";
                    Problem &problem = team.problem_set[int(problem_name - 'A')];
                    if (status == "ALL") {
                        if (problem.last_submit_status == 0) {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << problem_name << ' ';
                            if (problem.last_submit_status == 1) {
                                std::cout << "Accepted " << problem.submit_time_of_each_status[0] << '\n';
                            } else if (problem.last_submit_status == 2) {
                                std::cout << "Wrong_Answer " << problem.submit_time_of_each_status[1] << '\n';
                            } else if (problem.last_submit_status == 3) {
                                std::cout << "Runtime_Error " << problem.submit_time_of_each_status[2] << '\n';
                            } else if (problem.last_submit_status == 4) {
                                std::cout << "Time_Limit_Exceed " << problem.submit_time_of_each_status[3] << '\n';
                            }
                        }
                    } else if (status == "Accepted") {
                        if (problem.submit_time_of_each_status[0] == 0) {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << problem_name
                                      << " Accepted "
                                      << problem.submit_time_of_each_status[0] << '\n';
                        }
                    } else if (status == "Wrong_Answer") {
                        if (problem.submit_time_of_each_status[1] == 0) {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << problem_name
                                      << " Wrong_Answer "
                                      << problem.submit_time_of_each_status[1] << '\n';
                        }
                    } else if (status == "Runtime_Error") {
                        if (problem.submit_time_of_each_status[2] == 0) {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << problem_name
                                      << " Runtime_Error "
                                      << problem.submit_time_of_each_status[2] << '\n';
                        }
                    } else if (status == "Time_Limit_Exceed") {
                        if (problem.submit_time_of_each_status[3] == 0) {
                            std::cout << "Cannot find any submission.\n";
                        } else {
                            std::cout << team_name << ' ' << problem_name
                                      << " Time_Limit_Exceed "
                                      << problem.submit_time_of_each_status[3] << '\n';
                        }
                    }
                }
            }

        }
    }
    return 0;
}