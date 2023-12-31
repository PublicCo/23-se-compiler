//
// Created by leon on 2023/10/10.
//
#include "symbollist.h"

using namespace std;

/// 常量符号表，用于记录已声明的常量符号
/// 为了保证第五部分的作用域，每个block拥有一个unorder map
vector<unordered_map<string,symboltype>> VAL_MAP;

/// 检查符号表是否有值
bool HasName(vector <unordered_map<string, symboltype>> &sel_map, const string &name) {

    for (auto it = sel_map.rbegin(); it != sel_map.rend(); ++it) {
        const std::unordered_map <std::string, symboltype> &map = *it;
        auto found = map.find(name);
        if (found != map.end()) {
            return true;
        }
    }
    return false;
}

void warnerror(vector <unordered_map<string, symboltype>> &sel_map, const string &name) {
    cerr << "没有找到指定的参数" << endl;
    cerr << "想要找的参数为：" << name << endl;
    cerr << "当前符号表如下:" << endl;
    for (auto it = sel_map.rbegin(); it != sel_map.rend(); ++it) {
        cerr << "list" << endl;
        const std::unordered_map <string, symboltype> &map = *it;
        for (auto mpit = map.begin(); mpit != map.end(); ++mpit) {
            cerr << "name:" << mpit->first << ", num = " << mpit->second.num << ", type = " << int(mpit->second.type)
                 << endl;
        }
    }
    assert(0);
}

/// 获取符号表中的值
symboltype GetLvalValue(vector <unordered_map<string, symboltype>> &sel_map, const string &name) {
    for (auto it = sel_map.rbegin(); it != sel_map.rend(); ++it) {
        const std::unordered_map <std::string, symboltype> &map = *it;
        auto found = map.find(name);
        if (found != map.end()) {
            return found->second;
        }

    }
    warnerror(sel_map, name);
    assert(0);
}

void SetValue(vector <unordered_map<string, symboltype>> &sel_map, const string &name, symboltype value) {
    for (auto it = sel_map.rbegin(); it != sel_map.rend(); ++it) {
        std::unordered_map <std::string, symboltype> &map = *it;
        auto found = map.find(name);
        if (found != map.end()) {
            found->second.type = value.type;
            found->second.num = value.num;
        }
    }
    assert(0);
}

void VALMAP_push(){
    VAL_MAP.push_back(unordered_map<string, symboltype>());
    if(VAL_MAP.size()>VALMAP_LEVELREC.size()){
        //说明要加一层
        VALMAP_LEVELREC.push_back(0);
    } else{
        assert(!VALMAP_LEVELREC.empty());
        VALMAP_LEVELREC[VAL_MAP.size()-1]++;
    }
}

int retValDepth(string name) {
    int depth = VAL_MAP.size();
    for (auto it = VAL_MAP.rbegin(); it != VAL_MAP.rend(); it++) {
        unordered_map <string, symboltype> &tmpmap = *it;
        if (tmpmap.find(name) != tmpmap.end()) {
            break;
        } else {
            depth--;
        }
    }
    if (depth <= 0) {
        warnerror(VAL_MAP, name);
    }
    return depth;
}