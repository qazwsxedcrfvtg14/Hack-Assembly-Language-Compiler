//#pragma GCC optimize ("O3")
#include <bits/stdc++.h>
//#pragma GCC optimize ("O3")
using namespace std;
#define f first
#define s second
typedef pair<int, int> par;
typedef pair<int, par> pr;
// 0x0000~0x05FF Mem
// 0x600~???
// 0x0F00~0x2EFF ScreenBuffer
// 0x2F00~0x3CFF //new
// 0x3D00~0x3DFF Flag
// 0x3E00~0x3FFF Stack
// 0x4000~0x5FFF Screen
// 0x6000 KBD
// const int ScreenBuffer=0x0F00;
const int ScreenBuffer = 0x4000;
const int FlagTop = 0x3D00;
const int StackTop = 0x3E00;
const int Array_Pool_Begin = 0x2F00;
const int Array_Pool_Size = 0xE00;

vector<string> cmd;
void put(string s) {
  cmd.push_back(s);
  return;
}
void output_cmd() {
  for (string &s : cmd)
    puts(s.c_str());
  return;
}
void optimize(int o = 0) {
  if (o <= 0) {
    return;
  }
  int sz = cmd.size();
  vector<string> result, optres;
  string preA = "-1";
  for (int i = 0; i < sz; i++) {
    string s = cmd[i];
    string preCmd = result.size() >= 1 ? result.back() : "~~";
    string preCmd2 = result.size() >= 2 ? result[result.size() - 2] : "~~";
    if (s.length() > 1 && s[0] == '@') {
      string nowA = s.substr(1);
      if (preA == nowA)
        continue;
      preA = nowA;
    }
    if (s.length() > 1 && s[0] == '(')
      preA = "-1";
    if (s.length() > 1 && s[0] == 'A')
      preA = "-1";
    if (s.length() > 2 && s[0] == 'A')
      preA = "-1";
    if (s == "A=A")
      continue;
    if (s == "M=M")
      continue;
    if (s == "D=D")
      continue;
    if (preCmd == "M=D" && (s == "M=D" || s == "D=M"))
      continue;
    if (preCmd == "D=M" && (s == "M=D" || s == "D=M"))
      continue;
    if (preCmd2 == "M=D" && preCmd == "A=M" && s == "D=A")
      continue;
    if (preCmd == "M=M+1" && s == "M=M-1") {
      result.pop_back();
      continue;
    }
    if (preCmd == "M=M-1" && s == "M=M+1") {
      result.pop_back();
      continue;
    }
    if (preCmd == "D=D+1" && s == "D=D-1") {
      result.pop_back();
      continue;
    }
    if (preCmd == "D=D-1" && s == "D=D+1") {
      result.pop_back();
      continue;
    }
    if (preCmd.length() > 2 && s.length() > 2 && preCmd == "M=" && s == "M=") {
      result.pop_back();
      result.push_back(s);
      continue;
    }
    if (preCmd.length() > 2 && s.length() > 2 && preCmd == "D=" && s == "D=") {
      result.pop_back();
      result.push_back(s);
      continue;
    }
    if (preCmd == "M=D" && s == "M=M+1") {
      result.pop_back();
      result.push_back("M=D+1");
      continue;
    }
    if (preCmd == "M=D" && s == "M=M-1") {
      result.pop_back();
      result.push_back("M=D-1");
      continue;
    }
    if (preCmd == "M=D" && s == "M=-M") {
      result.pop_back();
      result.push_back("M=-D");
      continue;
    }
    if (preCmd == "M=D" && s == "M=!M") {
      result.pop_back();
      result.push_back("M=!D");
      continue;
    }
    result.push_back(s);
  }
  if (o <= 1) {
    cmd = result;
    return;
  }
  sz = result.size();
  for (int i = 0; i < sz; i++) {
    string s = result[i];
    string preCmd = optres.size() >= 1 ? optres.back() : "~~!";
    if (preCmd.length() > 2 && preCmd.substr(0, 2) == "M=" && s == "D=M") {
      optres.pop_back();
      optres.push_back("MD=" + preCmd.substr(2));
      continue;
    } else if (preCmd.length() > 2 && preCmd.substr(0, 2) == "M=" &&
               s == "A=M") {
      optres.pop_back();
      optres.push_back("AM=" + preCmd.substr(2));
      continue;
    } else if (preCmd.length() > 2 && preCmd.substr(0, 2) == "A=" &&
               s == "D=A") {
      optres.pop_back();
      optres.push_back("AD=" + preCmd.substr(2));
      continue;
    } else if (preCmd.length() > 2 && preCmd.substr(0, 2) == "A=" &&
               s == "D=A") {
      optres.pop_back();
      optres.push_back("AD=" + preCmd.substr(2));
      continue;
    } else if (preCmd.length() > 2 && preCmd.substr(0, 2) == "D=" &&
               s == "M=D") {
      optres.pop_back();
      optres.push_back("MD=" + preCmd.substr(2));
      continue;
    } else if (preCmd.length() > 2 && preCmd.substr(0, 2) == "D=" &&
               s == "A=D") {
      optres.pop_back();
      optres.push_back("AD=" + preCmd.substr(2));
      continue;
    }
    optres.push_back(s);
  }
  result = optres;
  optres.clear();
  sz = result.size();
  bool setA, setM, setD, useA, useM, useD, isJ;
  auto testCMD = [&](const string &s) {
    setA = setM = setD = useA = useM = useD = isJ = false;
    if (s.size() > 0 && s[0] == '@') {
      setA = true;
      return;
    }
    isJ = s.find(";") != string::npos;
    auto pos = s.find("=");
    if (pos == string::npos) {
      return;
    }
    for (int i = 0; i < pos; i++) {
      if (s[i] == 'A')
        setA = true;
      if (s[i] == 'M')
        setM = true, useA = true;
      if (s[i] == 'D')
        setD = true;
    }
    for (int i = pos; i < s.size(); i++) {
      if (s[i] == 'A')
        useA = true;
      if (s[i] == 'M')
        useM = true, useA = true;
      if (s[i] == 'D')
        useD = true;
    }
  };
  for (int i = 0; i < sz; i++) {
    string s = result[i];
    string preCmd = optres.size() >= 1 ? optres.back() : "~~~";
    if (preCmd.length() > 3 && preCmd.substr(0, 3) == "AM=" && s == "D=A") {
      optres.pop_back();
      optres.push_back("AMD=" + preCmd.substr(3));
      continue;
    }
    if (s == "D=M" && i + 5 < sz) {
      testCMD(result[i + 1]);
      if (useD == false && isJ == false && setM == false && setD == false) {
        if (result[i + 2] == "M=D-1") {
          if (result[i + 3] == "M=M-1" || result[i + 3] == "AM=M-1") {
            bool ok = true;
            for (int j = 0; i + j + 4 < sz; j++) {
              testCMD(result[i + j + 4]);
              if (useD || isJ) {
                ok = false;
                break;
              }
              if (setD) {
                break;
              }
            }
            if (ok) {
              optres.push_back("D=M-1");
              optres.push_back(result[i + 1]);
              if (result[i + 3] == "M=M-1")
                optres.push_back("M=D-1");
              if (result[i + 3] == "AM=M-1")
                optres.push_back("AM=D-1");
              i += 3;
              continue;
            }
          }
        }
        if (result[i + 2] == "M=D+1") {
          if (result[i + 3] == "M=M+1" || result[i + 3] == "AM=M+1") {
            bool ok = true;
            for (int j = 0; i + j + 4 < sz; j++) {
              testCMD(result[i + j + 4]);
              if (useD || isJ) {
                ok = false;
                break;
              }
              if (setD) {
                break;
              }
            }
            if (ok) {
              optres.push_back("D=M+1");
              optres.push_back(result[i + 1]);
              if (result[i + 3] == "M=M+1")
                optres.push_back("M=D+1");
              if (result[i + 3] == "AM=M+1")
                optres.push_back("AM=D+1");
              i += 3;
              continue;
            }
          }
        }
      }
    }

    testCMD(s);
    if (setD && !setA && !setM) {
      bool ok = true;
      for (int j = 0; i + j + 1 < sz; j++) {
        testCMD(result[i + j + 1]);
        if (useD || isJ) {
          ok = false;
          break;
        }
        if (setD) {
          break;
        }
      }
      if (ok)
        continue;
    }
    testCMD(s);
    if (setA && !setM && !setD) {
      bool ok = true;
      for (int j = 0; i + j + 1 < sz; j++) {
        testCMD(result[i + j + 1]);
        if (useM || useA || isJ) {
          ok = false;
          break;
        }
        if (setA) {
          break;
        }
      }
      if (ok)
        continue;
    }
    optres.push_back(s);
  }

  if (o <= 2) {
    cmd = optres;
    return;
  }
  map<string, int> mp_line;
  for (int i = 0; i < optres.size(); i++) {
    string &s = optres[i];
    if (s.front() == '(' && s.back() == ')') {
      string tmp = s.substr(1, s.length() - 2);
      mp_line[tmp] = i;
    }
  }
  set<int> no_dead;
  vector<vector<int>> graph(optres.size());
  vector<vector<int>> rgraph(optres.size());
  function<void(int, string)> dig = [&](int now, string a_val) {
    if (now >= optres.size())
      return;
    if (no_dead.find(now) != no_dead.end())
      return;
    no_dead.insert(now);
    string &s = optres[now];
    testCMD(s);
    if (setA) {
      a_val = "";
      if (s[0] == '@') {
        if (s[1] < '0' || s[1] > '9') {
          a_val = s.substr(1);
        }
      }
    }
    /*if(a_val!=""&&mp_line.find(a_val)==mp_line.end()){
      throw "bad opt";
    }*/
    if (s == "0;JMP") {
      if (a_val != "") {
        int ap = mp_line[a_val];
        rgraph[ap].push_back(now);
        graph[now].push_back(ap);
        dig(mp_line[a_val], a_val);
      }
      if (now + 1 < optres.size()) {
        string &ss = optres[now + 1];
        if (ss.find("(Func_Call_") != string::npos &&
            ss.find("_after)") != string::npos) {
          if (now + 1 < optres.size()) {
            rgraph[now + 1].push_back(now);
            graph[now].push_back(now + 1);
            dig(now + 1, a_val);
          }
        }
      }
    } else if (isJ) {
      if (now + 1 < optres.size()) {
        rgraph[now + 1].push_back(now);
        graph[now].push_back(now + 1);
        dig(now + 1, a_val);
      }
      if (a_val != "") {
        int ap = mp_line[a_val];
        rgraph[ap].push_back(now);
        graph[now].push_back(ap);
        dig(mp_line[a_val], a_val);
      }
    } else {
      if (now + 1 < optres.size()) {
        rgraph[now + 1].push_back(now);
        graph[now].push_back(now + 1);
        dig(now + 1, a_val);
      }
    }
  };
  dig(0, "");
  // fprintf(stderr,"%d %d\n",optres.size(),no_dead.size());
  vector<string> dcres;
  function<void(int)> dead_code = [&](int now) {
    if (no_dead.find(now) == no_dead.end())
      return;
    no_dead.erase(now);
    dcres.push_back(optres[now]);
    for (int nex : graph[now]) {
      if (rgraph[nex].size() == 1 || nex == now + 1) {
        dead_code(nex);
      } else {
        bool ok = true;
        for (int pre : rgraph[nex]) {
          if (pre + 1 == nex)
            ok = false;
        }
        if (ok) {
          dead_code(nex);
        }
      }
    }
  };
  dead_code(0);
  result = dcres;
  optres.clear();
  sz = result.size();
  for (int i = 0; i < sz; i++) {
    string pre = i ? result[i - 1] : "?????";
    string now = result[i];
    string nex = i + 1 < sz ? result[i + 1] : "?????";
    if (pre[0] == '@' && now == "0;JMP" && nex[0] == '(') {
      if (pre.substr(1) == nex.substr(1, nex.length() - 2)) {
        continue;
      }
    }
    optres.push_back(now);
  }
  if (o <= 3) {
    cmd = optres;
    return;
  }
  result = optres;
  optres.clear();
  sz = result.size();
  for (int i = 0; i < sz; i++) {
    string &s = result[i];
    testCMD(s);
    if (setD && !setA && !setM) {
      bool ok = true;
      for (int j = 0; i + j + 1 < sz; j++) {
        testCMD(result[i + j + 1]);
        if (useD || isJ) {
          ok = false;
          break;
        }
        if (setD) {
          break;
        }
      }
      if (ok)
        continue;
    }
    testCMD(s);
    if (setA && !setM && !setD) {
      bool ok = true;
      for (int j = 0; i + j + 1 < sz; j++) {
        testCMD(result[i + j + 1]);
        if (useM || useA || isJ) {
          ok = false;
          break;
        }
        if (setA) {
          break;
        }
      }
      if (ok)
        continue;
    }
    optres.push_back(s);
  }
  set<string> used_symbol;
  for (string &s : optres) {
    if (s[0] == '@') {
      if (s[1] < '0' || s[1] > '9') {
        used_symbol.insert(s.substr(1));
      }
    }
  }
  result = optres;
  optres.clear();
  for (string &s : result) {
    if (s.front() == '(' && s.back() == ')') {
      if (used_symbol.find(s.substr(1, s.length() - 2)) == used_symbol.end()) {
        continue;
      }
    }
    optres.push_back(std::move(s));
  }
  if (o <= 4) {
    cmd = optres;
    return;
  }
  map<string, string> mapping;
  for (string &s : optres) {
    if (s.size() < 2)
      continue;
    if (s[0] == '@') {
      if (s[1] < '0' || s[1] > '9') {
        string tmp = s.substr(1);
        if (mapping[tmp] == "") {
          string rng_string;
          for (int i = 0; i < 8; i++)
            rng_string += (rand() >> 2) % 26 + 'A';
          mapping[tmp] = rng_string;
        }
        s = "@" + mapping[tmp];
      }
    } else if (s.front() == '(' && s.back() == ')') {
      string tmp = s.substr(1, s.length() - 2);
      if (mapping[tmp] == "") {
        string rng_string;
        for (int i = 0; i < 8; i++)
          rng_string += (rand() >> 2) % 26 + 'A';
        mapping[tmp] = rng_string;
      }
      s = "(" + mapping[tmp] + ")";
    }
  }
  cmd = optres;
  return;
}
void while_true() {
  string rng_string;
  for (int i = 0; i < 8; i++)
    rng_string += (rand() >> 2) % 26 + 'A';
  put("(while_" + rng_string + "_true)");
  put("@while_" + rng_string + "_true");
  put("0;JMP");
}
template <typename T> string ToStr(T x) {
  stringstream str;
  string s;
  str << x;
  str >> s;
  return s;
}
int objcnt = 0;
class var;
class obj {
private:
  int id;

public:
  int star;
  inline int get_id() { return id; }
  obj() {
    id = objcnt++;
    star = 0;
  }
  obj(int _id) {
    id = _id;
    star = 0;
  }
  obj(const obj &x) {
    id = x.id;
    star = x.star;
  }
  void resetid(int x) { id = x; }
  void run_star() const {
    for (int i = 0; i < star; i++)
      put("A=M");
  }
  void operator=(int x) {
    if (x == 0) {
      put("@" + ToStr(id));
      run_star();
      put("M=0");
    } else if (x == 1) {
      put("@" + ToStr(id));
      run_star();
      put("M=1");
    } else if (x == -1) {
      put("@" + ToStr(id));
      run_star();
      put("M=-1");
    } else if (x == -32768) {
      put("@" + ToStr(32767));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=D-1");
    } else if (x > 0) {
      put("@" + ToStr(x));
      put("D=A");
      put("@" + ToStr(id));
      run_star();
      put("M=D");
    } else {
      put("@" + ToStr(-x));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=D");
    }
  }
  void operator=(const obj &x) {
    put("@" + ToStr(x.id));
    x.run_star();
    put("D=M");
    put("@" + ToStr(id));
    run_star();
    put("M=D");
  }
  void operator=(string s) {
    put("@" + s);
    put("D=A");
    put("@" + ToStr(id));
    run_star();
    put("M=D");
  }
  void operator++(int x) {
    put("@" + ToStr(id));
    run_star();
    put("M=M+1");
  }
  void operator--(int x) {
    put("@" + ToStr(id));
    run_star();
    put("M=M-1");
  }
  void operator+=(int x) {
    if (x == 0)
      return;
    else if (x == 1) {
      put("@" + ToStr(id));
      run_star();
      put("M=M+1");
    } else if (x == -1) {
      put("@" + ToStr(id));
      run_star();
      put("M=M-1");
    } else if (x == 2) {
      put("@" + ToStr(id));
      run_star();
      put("M=M+1");
      put("M=M+1");
    } else if (x == -2) {
      put("@" + ToStr(id));
      run_star();
      put("M=M-1");
      put("M=M-1");
    } else if (x >= 0) {
      put("@" + ToStr(x));
      put("D=A");
      put("@" + ToStr(id));
      run_star();
      put("M=M+D");
    } else if (x == -32768) {
      put("@" + ToStr(32767));
      put("D=-A");
      put("D=D-1");
      put("@" + ToStr(id));
      run_star();
      put("M=M+D");
    } else {
      put("@" + ToStr(-x));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=M+D");
    }
  }
  void operator&=(int x) {
    if (x == 0) {
      put("@" + ToStr(id));
      run_star();
      put("M=0");
    } else if (x > 0) {
      put("@" + ToStr(x));
      put("D=A");
      put("@" + ToStr(id));
      run_star();
      put("M=M&D");
    } else if (x == -32768) {
      put("@" + ToStr(32767));
      put("D=-A");
      put("D=D-1");
      put("@" + ToStr(id));
      run_star();
      put("M=M&D");
    } else {
      put("@" + ToStr(-x));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=M&D");
    }
  }
  void operator|=(int x) {
    if (x == 0) {
      return;
    } else if (x > 0) {
      put("@" + ToStr(x));
      put("D=A");
      put("@" + ToStr(id));
      run_star();
      put("M=M|D");
    } else if (x == -32768) {
      put("@" + ToStr(32767));
      put("D=-A");
      put("D=D-1");
      put("@" + ToStr(id));
      run_star();
      put("M=M|D");
    } else {
      put("@" + ToStr(-x));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=M|D");
    }
  }
  void operator-=(int x) {
    if (x == 0)
      return;
    else if (x == 1) {
      put("@" + ToStr(id));
      run_star();
      put("M=M-1");
    } else if (x == -1) {
      put("@" + ToStr(id));
      run_star();
      put("M=M+1");
    } else if (x == 2) {
      put("@" + ToStr(id));
      run_star();
      put("M=M-1");
      put("M=M-1");
    } else if (x == -2) {
      put("@" + ToStr(id));
      run_star();
      put("M=M+1");
      put("M=M+1");
    } else if (x >= 0) {
      put("@" + ToStr(x));
      put("D=A");
      put("@" + ToStr(id));
      run_star();
      put("M=M-D");
    } else if (x == -32768) {
      put("@" + ToStr(32767));
      put("D=-A");
      put("D=D-1");
      put("@" + ToStr(id));
      run_star();
      put("M=M-D");
    } else {
      put("@" + ToStr(-x));
      put("D=-A");
      put("@" + ToStr(id));
      run_star();
      put("M=M-D");
    }
  }
  void operator+=(const obj &x) {
    put("@" + ToStr(x.id));
    x.run_star();
    put("D=M");
    put("@" + ToStr(id));
    run_star();
    put("M=M+D");
  }
  void operator-=(const obj &x) {
    put("@" + ToStr(x.id));
    x.run_star();
    put("D=M");
    put("@" + ToStr(id));
    run_star();
    put("M=M-D");
  }
  void operator&=(const obj &x) {
    put("@" + ToStr(x.id));
    x.run_star();
    put("D=M");
    put("@" + ToStr(id));
    run_star();
    put("M=M&D");
  }
  void operator|=(const obj &x) {
    put("@" + ToStr(x.id));
    x.run_star();
    put("D=M");
    put("@" + ToStr(id));
    run_star();
    put("M=M|D");
  }
  void nt() {
    put("@" + ToStr(id));
    run_star();
    put("M=!M");
  }
  void toA() {
    put("@" + ToStr(id));
    put("A=M");
    run_star();
  }
  void toD() {
    put("@" + ToStr(id));
    run_star();
    put("A=M");
    put("D=A");
  }
  void operator*=(int x);
  void operator*=(const obj &x);
  void operator/=(int x);
  void operator/=(const obj &x);
  void operator%=(int x);
  void operator%=(const obj &x);
  void operator^=(int x);
  void operator^=(const obj &x);
};
map<string, obj> glo;
stack<int> esp_ebp;
int var_id;
string var_string[4] = {"var1", "var2", "var3", "var4"};
class var {
private:
  int pos;
  int level;

public:
  int get_pos() { return pos; }
  var() {
    pos = esp_ebp.top()++;
    glo["esp"]++;
    level = esp_ebp.size();
  }
  var(const var &x) {
    pos = x.pos;
    level = x.level;
  }
  var(const obj &x) = delete;
  var(string s, int x) {
    if (s != "arg")
      throw "error: var init error!";
    pos = x;
    level = esp_ebp.size();
  }
  void operator=(int x) { (this->to_obj()) = x; }
  void operator=(const var &x) { (this->to_obj()) = x.to_obj(); }
  void operator=(const obj &x) { (this->to_obj()) = x; }
  void operator=(string s) { (this->to_obj()) = s; }
  obj to_obj() const {
    if (level != (int)esp_ebp.size())
      throw "error: outer var can't use in function";
    glo[var_string[var_id]] = glo["ebp"];
    glo[var_string[var_id]] += pos;
    obj ret(glo[var_string[var_id]]);
    ret.star++;
    var_id = (var_id + 1) % 4;
    return ret;
  }
  obj to_obj(string special) const {
    if (level != (int)esp_ebp.size())
      throw "error: outer var can't use in function";
    glo[special] = glo["ebp"];
    glo[special] += pos;
    obj ret(glo[special]);
    ret.star++;
    return ret;
  }
  /*explicit */ operator obj() { return to_obj(); }
  operator obj() const { return to_obj(); }
  void operator++(int x) { (this->to_obj())++; }
  void operator--(int x) { (this->to_obj())--; }
  void operator+=(int x) { (this->to_obj()) += x; }
  void operator-=(int x) { (this->to_obj()) -= x; }
  void operator&=(int x) { (this->to_obj()) &= x; }
  void operator|=(int x) { (this->to_obj()) |= x; }
  void operator*=(int x) { (this->to_obj("math_mul_op")) *= x; }
  void operator*=(const obj &x) { (this->to_obj("math_mul_op")) *= x; }
  void operator/=(int x) { (this->to_obj("math_div_op")) /= x; }
  void operator/=(const obj &x) { (this->to_obj("math_div_op")) /= x; }
  void operator%=(int x) { (this->to_obj("math_mod_op")) %= x; }
  void operator%=(const obj &x) { (this->to_obj("math_mod_op")) %= x; }
  void operator^=(int x) { (this->to_obj("math_xor_op")) ^= x; }
  void operator^=(const obj &x) { (this->to_obj("math_xor_op")) ^= x; }
  void operator+=(const obj &x) { (this->to_obj()) += x; }
  void operator-=(const obj &x) { (this->to_obj()) -= x; }
  void operator&=(const obj &x) { (this->to_obj()) &= x; }
  void operator|=(const obj &x) { (this->to_obj()) |= x; }
  void nt() { (this->to_obj()).nt(); }
  void toA() { (this->to_obj()).toA(); }
  void toD() { (this->to_obj()).toD(); }
};
class memory {
public:
  obj operator[](int x) {
    obj ret(x);
    return ret;
  }
  obj operator[](obj x) {
    obj ret(x);
    ret.star++;
    return ret;
  }
} mem;
void push() {
  mem[glo["esp"]] = glo["ebp"];
  glo["esp"]++;
  glo["ebp"] = glo["esp"];
}
void pop() {
  glo["ebp"]--;
  glo["esp"] = glo["ebp"];
  glo["ebp"] = mem[glo["ebp"]];
}
// old data
// return pc 88
// subfunction return value 89
// saved ebp 90
// subfunction arg0 91
// return pc 92
// subfunction return value 93
// saved ebp 94
// subfunction arg0 95
// return pc 96
// subfunction return value 97
// saved ebp 98
// subfunction arg0 99
// return pc 00
// subfunction return value 01
// saved ebp 02
// subfunction arg0 03
//...
map<string, int> func_args;
stack<string> func_stack;
stack<int> block_stack, loop_esp_stack, if_esp_stack;
set<string> defined_func;
void FuncBegin(string fun, int args = 0) {
  if (func_args.find(fun) != func_args.end())
    throw "error: multi define function";
  defined_func.insert(fun);
  block_stack.push(0);
  func_stack.push(fun);
  func_args[fun] = args;
  put("@Func_" + fun + "_end");
  put("0;JMP");
  put("(Func_" + fun + "_begin)");
  push();
  if (args)
    glo["esp"] += args;
  esp_ebp.push(args);
}
template <typename T> void FuncReturn(T ret) {
  if (func_stack.empty())
    throw "error: return without function";
  string fun = func_stack.top();
  glo["tmp"] = glo["ebp"];
  glo["tmp"] -= 2;
  mem[glo["tmp"]] = ret;
  put("@Func_" + fun + "_return");
  put("0;JMP");
}
void FuncEnd() {
  if (func_stack.empty())
    throw "error: end without function";
  string fun = func_stack.top();
  func_stack.pop();
  put("(Func_" + fun + "_return)");
  pop();
  glo["esp"] -= 2;
  mem[glo["esp"]].toA();
  put("0;JMP");
  put("(Func_" + fun + "_end)");
  esp_ebp.pop();
}
void FuncCallArgs() {}
template <typename T, typename... Args> void FuncCallArgs(T arg, Args... args) {
  glo["arg"]++;
  mem[glo["arg"]] = arg;
  FuncCallArgs(args...);
}
set<string> called_func;
template <typename... Args> var FuncCall(string fun, Args... args) {
  called_func.insert(fun);
  string rng_string;
  for (int i = 0; i < 8; i++)
    rng_string += (rand() >> 2) % 26 + 'A';
  mem[glo["esp"]] = ("Func_Call_" + rng_string + "_after");
  glo["esp"] += 2; // return value
  glo["arg"] = glo["esp"];
  FuncCallArgs(args...);
  put("@Func_" + fun + "_begin");
  put("0;JMP");
  put("(Func_Call_" + rng_string + "_after)");
  glo["tmp"] = glo["esp"];
  glo["tmp"]++;
  mem[glo["esp"]] = mem[glo["tmp"]];
  glo["esp"]++;
  var ret("arg", esp_ebp.top()++);
  return ret;
}
stack<string> if_stack;
template <typename T, typename S> void IfBegin(T x, string cmp, S y) {
  block_stack.push(1);
  if_esp_stack.push(esp_ebp.top());
  string rng_string;
  for (int i = 0; i < 8; i++)
    rng_string += (rand() >> 2) % 26 + 'A';
  if_stack.push(rng_string);
  glo["cmp"] = x;
  glo["cmp"] -= y;
  glo["cmp"].toD();
  put("@if_" + rng_string + "_end");
  if (cmp == "<")
    put("D;JGE");
  else if (cmp == "<=")
    put("D;JGT");
  else if (cmp == "==")
    put("D;JNE");
  else if (cmp == ">")
    put("D;JLE");
  else if (cmp == ">=")
    put("D;JLT");
  else if (cmp == "!=")
    put("D;JEQ");
  else
    throw "error: unknown if op";
}
void IfEnd() {
  if (if_stack.empty())
    throw "error: end without if";
  int d = esp_ebp.top() - if_esp_stack.top();
  if_esp_stack.pop();
  if (d)
    glo["esp"] -= d;
  esp_ebp.top() -= d;
  string rng_string = if_stack.top();
  put("(if_" + rng_string + "_end)");
  if_stack.pop();
}
stack<string> loop_stack;
void LoopBegin() {
  block_stack.push(2);
  loop_esp_stack.push(esp_ebp.top());
  string rng_string;
  for (int i = 0; i < 8; i++)
    rng_string += (rand() >> 2) % 26 + 'A';
  loop_stack.push(rng_string);
  put("(loop_" + rng_string + "_begin)");
}
void LoopBreak() {
  if (loop_stack.empty())
    throw "error: break without loop";
  string rng_string = loop_stack.top();
  int d = esp_ebp.top() - loop_esp_stack.top();
  if (d)
    glo["esp"] -= d;
  put("@loop_" + rng_string + "_end");
  put("0;JMP");
}
void LoopEnd() {
  string rng_string = loop_stack.top();
  loop_stack.pop();
  int d = esp_ebp.top() - loop_esp_stack.top();
  loop_esp_stack.pop();
  if (d)
    glo["esp"] -= d;
  esp_ebp.top() -= d;
  put("@loop_" + rng_string + "_begin");
  put("0;JMP");
  put("(loop_" + rng_string + "_end)");
}
void BlockEnd() {
  if (block_stack.empty())
    throw "error: end without block";
  int top = block_stack.top();
  block_stack.pop();
  if (top == 0)
    FuncEnd();
  else if (top == 1)
    IfEnd();
  else if (top == 2)
    LoopEnd();
}
bool Setuped = false;
void Setup() {
  if (Setuped)
    throw "error: multi setup";
  Setuped = true;
  objcnt = FlagTop;
  glo["ebp"] = StackTop;
  glo["esp"] = glo["ebp"];
  esp_ebp.push(0);
}
void StackCheck() {
  for (string s : called_func)
    if (defined_func.find(s) == defined_func.end()) {
      throw "error: call undefined function '" + s + "'";
    }
  if (!block_stack.empty())
    throw "error: exit without empty stack";
}
vector<string> ve;
// W=32*16
// H=256
#define Begin                                                                  \
  int main() {                                                                 \
    try {                                                                      \
      Setup();

#define CallFunction FuncCall
#define Call0(a) FuncCall(#a)
#define Call1(a, b) FuncCall(#a, b)
#define Call2(a, b, c) FuncCall(#a, b, c)
#define Call3(a, b, c, d) FuncCall(#a, b, c, d)
#define Call4(a, b, c, d, e) FuncCall(#a, b, c, d, e)
#define Call5(a, b, c, d, e, f) FuncCall(#a, b, c, d, e, f)
#define Call6(a, b, c, d, e, f, g) FuncCall(#a, b, c, d, e, f, g)
#define Call7(a, b, c, d, e, f, g, h) FuncCall(#a, b, c, d, e, f, g, h)
#define Call8(a, b, c, d, e, f, g, h, i) FuncCall(#a, b, c, d, e, f, g, h, i)
#define Call9(a, b, c, d, e, f, g, h, i, j)                                    \
  FuncCall(#a, b, c, d, e, f, g, h, i, j)
#define Call10(a, b, c, d, e, f, g, h, i, j, k)                                \
  FuncCall(#a, b, c, d, e, f, g, h, i, j, k)
#define Call11(a, b, c, d, e, f, g, h, i, j, k, l)                             \
  FuncCall(#a, b, c, d, e, f, g, h, i, j, k, l)
#define Call12(a, b, c, d, e, f, g, h, i, j, k, l, m)                          \
  FuncCall(#a, b, c, d, e, f, g, h, i, j, k, l, m)
#define GET_MACRO2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,     \
                   NAME, ...)                                                  \
  NAME
#define Call(...)                                                              \
  GET_MACRO2(__VA_ARGS__, Call12, Call11, Call10, Call9, Call8, Call7, Call6,  \
             Call5, Call4, Call3, Call2, Call1, Call0)                         \
  (__VA_ARGS__)
#define Loop                                                                   \
  LoopBegin();                                                                 \
  {
#define If(a, b, c)                                                            \
  IfBegin((a), #b, (c));                                                       \
  {
#define Function(a, b)                                                         \
  FuncBegin((a), (b));                                                         \
  {
#define Func0(a)                                                               \
  FuncBegin(#a, 0);                                                            \
  {
#define Func1(a, b)                                                            \
  FuncBegin(#a, 1);                                                            \
  {                                                                            \
    var b("arg", 0);
#define Func2(a, b, c)                                                         \
  FuncBegin(#a, 2);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1);
#define Func3(a, b, c, d)                                                      \
  FuncBegin(#a, 3);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2);
#define Func4(a, b, c, d, e)                                                   \
  FuncBegin(#a, 4);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3);
#define Func5(a, b, c, d, e, f)                                                \
  FuncBegin(#a, 5);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4);
#define Func6(a, b, c, d, e, f, g)                                             \
  FuncBegin(#a, 6);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5);
#define Func7(a, b, c, d, e, f, g, h)                                          \
  FuncBegin(#a, 7);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6);
#define Func8(a, b, c, d, e, f, g, h, i)                                       \
  FuncBegin(#a, 8);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6), i("arg", 7);
#define Func9(a, b, c, d, e, f, g, h, i, j)                                    \
  FuncBegin(#a, 9);                                                            \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6), i("arg", 7), j("arg", 8);
#define Func10(a, b, c, d, e, f, g, h, i, j, k)                                \
  FuncBegin(#a, 10);                                                           \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6), i("arg", 7), j("arg", 8), k("arg", 9);
#define Func11(a, b, c, d, e, f, g, h, i, j, k, l)                             \
  FuncBegin(#a, 11);                                                           \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6), i("arg", 7), j("arg", 8), k("arg", 9),       \
        l("arg", 10);
#define Func12(a, b, c, d, e, f, g, h, i, j, k, l, m)                          \
  FuncBegin(#a, 12);                                                           \
  {                                                                            \
    var b("arg", 0), c("arg", 1), d("arg", 2), e("arg", 3), f("arg", 4),       \
        g("arg", 5), h("arg", 6), i("arg", 7), j("arg", 8), k("arg", 9),       \
        l("arg", 10), m("arg", 11);
#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,      \
                  NAME, ...)                                                   \
  NAME
#define Func(...)                                                              \
  GET_MACRO(__VA_ARGS__, Func12, Func11, Func10, Func9, Func8, Func7, Func6,   \
            Func5, Func4, Func3, Func2, Func1, Func0)                          \
  (__VA_ARGS__)
#define Stop while_true();
#define End                                                                    \
  }                                                                            \
  BlockEnd();
#define Return(a) FuncReturn((a))
#define Break LoopBreak();
#define Finish                                                                 \
  while_true();                                                                \
  StackCheck();                                                                \
  }                                                                            \
  catch (char const *error) {                                                  \
    puts(error);                                                               \
    return 0;                                                                  \
  }                                                                            \
  catch (string error) {                                                       \
    puts(error.c_str());                                                       \
    return 0;                                                                  \
  }                                                                            \
  optimize(5);                                                                 \
  optimize(2);                                                                 \
  output_cmd();                                                                \
  return 0;                                                                    \
  }
#define Var var
#define Mem mem
#define Global glo

// const int ScreenBuffer=0x0F00;
const int KBD = 0x6000;