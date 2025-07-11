#include "adj_matrix.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstring>
#include <functional>

using namespace std;

struct Task
{
    int id;
    double w;
    double d;
};

struct Core
{
    double s;
    double p;
};

struct Item
{
    int task;
    int core;
    double start;
    double end;
    double e;
};

int main()
{
    ifstream fin("input.txt");
    if (!fin)
    {
        ofstream("input.txt");
        fin.open("input.txt");
    }
    ofstream fout("output.txt");
    cin.rdbuf(fin.rdbuf());
    cout.rdbuf(fout.rdbuf());
    cerr.setf(ios::unitbuf);
    int N;
    int M;
    int K;
    cin >> N;
    Graph<int, int> g(N);
    Task T[10];
    int ids[10];
    for (int i = 0; i < N; i++)
    {
        cin >> T[i].id;
        cin >> T[i].w;
        cin >> T[i].d;
        ids[i] = T[i].id;
    }
    cin >> M;
    for (int e = 0; e < M; e++)
    {
        int u;
        int v;
        cin >> u;
        cin >> v;
        int iu = -1;
        int iv = -1;
        for (int k = 0; k < N; k++)
        {
            if (ids[k] == u)
            {
                iu = k;
            }
            if (ids[k] == v)
            {
                iv = k;
            }
        }
        if (iu >= 0 && iv >= 0)
        {
            g.A[iu][iv] = 1;
        }
    }
    cin >> K;
    Core C[10];
    for (int i = 0; i < K; i++)
    {
        cin >> C[i].s;
        cin >> C[i].p;
    }
    int orderArr[10];
    bool usedOrd[10] = { false };
    int assignArr[10];
    double bestE = numeric_limits<double>::infinity();
    int bestZero = -1;
    double bestMax = -1;
    Item bestSch[10];
    int bestCnt = 0;
    bool has2Cycle = false;
    int partnerArr[10];
    memset(partnerArr, -1, sizeof(partnerArr));
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (g.A[i][j] && g.A[j][i])
            {
                has2Cycle = true;
                partnerArr[i] = j;
                partnerArr[j] = i;
                cerr << "Warning: Cycle detected involving tasks: {"
                    << ids[i] << "," << ids[j] << "}" << endl;
            }
        }
    }
    function<void()> testSchedule = [&]()
        {
            bool done[10] = { false };
            double coreFree[10] = { 0 };
            double endT[10] = { 0 };
            Item sch[10];
            int sc = 0;
            double E = 0;
            for (int p = 0; p < N; p++)
            {
                int i = orderArr[p];
                if (done[i])
                {
                    continue;
                }
                if (partnerArr[i] != -1)
                {
                    int j = partnerArr[i];

                    if (done[j])
                    {
                        return;
                    }
                    double ts = 0;
                    for (int k = 0; k < N; k++)
                    {
                        bool di = (g.A[k][i] && k != j);
                        bool dj = (g.A[k][j] && k != i);
                        if ((di || dj) && !done[k])
                        {
                            return;
                        }
                        if (di)
                        {
                            ts = max(ts, endT[k]);
                        }
                        if (dj)
                        {
                            ts = max(ts, endT[k]);
                        }
                    }
                    int c1 = assignArr[i];
                    int c2 = assignArr[j];
                    if (c1 == c2)
                    {
                        return;
                    }
                    double d1 = T[i].w / C[c1].s;
                    double d2 = T[j].w / C[c2].s;
                    double st = max(ts, max(coreFree[c1], coreFree[c2]));
                    double e1 = st + d1;
                    double e2 = st + d2;
                    if (e1 > T[i].d || e2 > T[j].d)
                    {
                        return;
                    }
                    sch[sc++] = { j, c2, st, e2, d2 * C[c2].p };
                    sch[sc++] = { i, c1, st, e1, d1 * C[c1].p };
                    coreFree[c2] = e2;
                    coreFree[c1] = e1;
                    endT[j] = e2;
                    endT[i] = e1;
                    E += d2 * C[c2].p + d1 * C[c1].p;
                    done[j] = true;
                    done[i] = true;
                }
                else
                {
                    double ts = 0;
                    for (int k = 0; k < N; k++)
                    {
                        if (g.A[k][i] && !done[k])
                        {
                            return;
                        }
                        if (g.A[k][i])
                        {
                            ts = max(ts, endT[k]);
                        }
                    }
                    int c = assignArr[i];
                    double dur = T[i].w / C[c].s;
                    double st = max(ts, coreFree[c]);
                    double en = st + dur;
                    if (en > T[i].d)
                    {
                        return;
                    }
                    sch[sc++] = { i, c, st, en, dur * C[c].p };
                    coreFree[c] = en;
                    endT[i] = en;
                    E += dur * C[c].p;
                    done[i] = true;
                }
            }
            if (sc != N)
            {
                return;
            }
            int zc = 0;
            double ms = 0;
            for (int i = 0; i < sc; i++)
            {
                if (sch[i].start == 0)
                {
                    zc++;
                }
                ms = max(ms, sch[i].end);
            }
            if (E < bestE || (E == bestE && (zc > bestZero || (zc == bestZero && ms > bestMax))))
            {
                bestE = E;
                bestZero = zc;
                bestMax = ms;
                bestCnt = sc;
                for (int i = 0; i < sc; i++)
                {
                    bestSch[i] = sch[i];
                }
            }
            else if (has2Cycle && E == bestE && zc == bestZero && ms == bestMax)
            {
                if (sch[0].task > bestSch[0].task)
                {
                    bestCnt = sc;

                    for (int i = 0; i < sc; i++)
                    {
                        bestSch[i] = sch[i];
                    }
                }
            }
        };
    function<void(int)> genAssign = [&](int idx)
        {
            if (idx == N)
            {
                testSchedule();
                return;
            }

            for (int c = 0; c < K; c++)
            {
                assignArr[idx] = c;
                genAssign(idx + 1);
            }
        };
    function<void(int)> genOrder = [&](int idx)
        {
            if (idx == N)
            {
                genAssign(0);
                return;
            }
            for (int i = 0; i < N; i++)
            {
                if (!usedOrd[i])
                {
                    usedOrd[i] = true;
                    orderArr[idx] = i;
                    genOrder(idx + 1);
                    usedOrd[i] = false;
                }
            }
        };
    memset(usedOrd, 0, sizeof(usedOrd));
    genOrder(0);
    cout << "Scheduling Results" << endl;
    if (bestCnt == 0)
    {
        cout << "Scheduling failed. No solution found that meets all deadlines." << endl;
        return 0;
    }
    cout << "Minimum Total Energy Consumed: " << fixed << setprecision(2) << bestE << endl;
    cout << "Schedule (one optimal):" << endl;
    cout << setw(4) << "Task"
        << setw(7) << "Core"
        << setw(8) << "Start"
        << setw(8) << "End"
        << setw(8) << "Energy"
        << endl;
    for (int i = 0; i < bestCnt; i++)
    {
        auto& s = bestSch[i];

        cout << setw(4) << ids[s.task]
            << setw(7) << s.core
            << setw(8) << fixed << setprecision(2) << s.start
            << setw(8) << fixed << setprecision(2) << s.end
            << setw(8) << fixed << setprecision(2) << s.e
            << endl;
    }
    return 0;
}
