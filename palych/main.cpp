#pragma GCC target("avx,avx2")
#include <deque>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstddef>
using namespace std;

using uint_data = unsigned int;
using uint_pair = array<uint_data, 2>;
using ans_type = pair<long long, uint_pair>;
using ll = long long;
constexpr int bitlen = sizeof(uint_data) * 8;
constexpr int delta = bitlen - 1;

long long ntime() {
    using namespace chrono;
    return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
}

void check_validity(const vector<bool>& res, string reference_name) {
    ifstream in(reference_name);
    if(!in.is_open()) {
        cerr << "REFERENCE FILE NOT FOUND" << endl;
        return;
    }
    string s;
    getline(in, s);

    vector<bool> ref;
    for(long long i = 0; i < (ll)s.size(); i++)
        ref.push_back(s[i] - '0');

    if(res.size() > ref.size()) {
        cerr << "RESULT TOO BIG TO BE FULLY CHECKED" << endl;
    }
    for(long long i = 0; i < (ll)min(res.size(), ref.size()); i++)
        if(res[i] != ref[i]) {
            cerr << "RESULT DOESNT MATCH WITH REFERENCE" << endl;
            return;
        }
    cerr << "RESULT MATCHES WITH REFERENCE" << endl;
}

void dump_to_file(vector<bool>& res, string filename) {
    ofstream out(filename);
    for(long long i = 0; i < (ll)res.size(); i++)
        out << res[i];
    out << endl;
}

long long compute_range(vector<uint_data>& srcprev, vector<uint_pair>& side, vector<ans_type>& ans, long long stride_num, long long row_num) {

    const long long
            horizontal_offset = srcprev.size() * stride_num,
            diagonal_offset_default = srcprev.size() / 2,
            diagonal_offset = diagonal_offset_default - (stride_num == 0),
            diagonal_offset_cumulative = diagonal_offset_default * stride_num + side.size() * row_num - (stride_num != 0);

    vector<uint_data> curr(srcprev.size() + 2);
    vector<uint_data> prev(srcprev.size() + 2);

    for(long long i = 0; i < (ll)srcprev.size(); i++) prev[i + 2] = srcprev[i];

    for(long long k = 0; k < (ll)side.size() + diagonal_offset; k++) {
        if(k < (ll)side.size()) {
            prev[0] = side[k][0];
            prev[1] = side[k][1];
        }

#pragma GCC ivdep
        for(long long i = 2; i < (ll)curr.size(); i++) {
            uint_data
                    left_v  = prev[i - 2],
                    mid_v   = prev[i - 1],
                    right_v = prev[i    ];

            static_assert(__GNUC__ >= 8, "GCC-8 or higher is required"); //pragma unroll is introduced in g++-8
#pragma GCC unroll delta
            for(int j = 0; j < delta; j++) {
                uint_data
                        m_right = (mid_v << 1) | (left_v  >> delta),
                        m_left  = (mid_v >> 1) | (right_v << delta); // 6 iops

                uint_data
                        r_right = (right_v << 1) | (mid_v >> delta),
                        r_left  = (right_v >> 1); // 4 iops

                uint_data
                        l_right = (left_v << 1),
                        l_left  = (left_v >> 1) | (mid_v << delta); // 4 iops

                mid_v = (mid_v | m_left) ^ m_right;
                right_v = (right_v | r_left) ^ r_right;
                left_v = (left_v | l_left) ^ l_right;  // 6 iops

            }

            uint_data
                    m_right = (mid_v << 1) | (left_v  >> delta),
                    m_left  = (mid_v >> 1) | (right_v << delta); // 6 iops

            curr[i] = (mid_v | m_left) ^ m_right; // 2 iops
        }

        long long ans_location = k + diagonal_offset_cumulative;
        long long ans_ind = ans_location - horizontal_offset + 2;
        if(ans_ind > 0 && ans_ind < (ll)prev.size() && (ans_ind - 1) - (k - ((ll)side.size() - 1)) * 2 >= 0)
            ans.push_back(ans_type{ans_location, uint_pair{ prev[ans_ind - 1], prev[ans_ind] }});

        if(k >= diagonal_offset)
            side[k - diagonal_offset] = { prev[prev.size() - 2], prev[prev.size() - 1] };

        swap(prev, curr); // prev = curr; without copying or reallocating
        if(k == (ll)side.size() - 1)
            for(long long i = 0; i < (ll)srcprev.size(); i++)
                srcprev[i] = prev[i + 2];
    }
    return (8 + 20 * delta) * srcprev.size() * side.size(); //effective iops
}

vector<bool> restore_answer(const vector<uint_pair> ans) {

    vector<bool> ans_bool(ans.size() * bitlen);

    for(long long i = 0; i < (ll)ans.size(); i++) {
        uint_pair elem = ans[i];
        for(int j = 0; j < bitlen; j++) {
            ans_bool[i * bitlen + j] = elem[1] & 1;

            uint_data
                    left_v  = elem[0],
                    mid_v = elem[1];

            uint_data
                    m_right = (mid_v << 1) | (left_v  >> delta),
                    m_left  = (mid_v >> 1);

            uint_data
                    l_right = (left_v << 1),
                    l_left  = (left_v >> 1) | (mid_v << delta);

            elem[0] = (left_v | l_left) ^ l_right;
            elem[1] = (mid_v | m_left) ^ m_right;
        }
    }

    return move(ans_bool);
}

struct prof_data {
    long long iops = 0;
    long long dt = 0;
    long long tall = 0;
    long long rowlength;
    long long stridewidth;
    long long nrows;
    long long nprocs;
    void verbose_output() {
        cerr << "nrows: " << nrows << endl;
        cerr << "stridewidth: " << stridewidth << endl;
        cerr << "GIops: " << (double)iops/dt << endl;
        cerr << "Time: " << tall * 1e-9 << "s" << endl;
        cerr << "Overhead: " << (tall - dt / nprocs) * 1e-9 << "s (" << 100.0 * (tall - dt / nprocs) / tall << "%)" << endl;
        cerr.flush();
    }
    string table_output() {
        ostringstream out;
        out << iops << '\t' << dt << '\t' << tall << '\t' << rowlength << '\t' << stridewidth << '\t' << nrows << '\t' << nprocs << endl;
        return out.str();
    }
};

vector<bool> scheduler(long long depth, long long stridewidth=64, int nrows=0, int myid=-1) {

    long long tbegin = ntime();

    int nprocs;
    if(nrows == 0) nrows = nprocs;

    long long reqdepth = (depth - 1) / bitlen + 1;
    long long nstrides = (reqdepth - 1) / stridewidth + 1;
    long long rowlength = (reqdepth - stridewidth / 2 * (nstrides - 1)) / nrows + 1;

    vector<pair<long long, uint_pair>> ans;
    vector<uint_pair> side(rowlength);
    vector<uint_data> srcprev(stridewidth); // width must be even
    if(myid == 0) srcprev[0] = 1;

    prof_data data {
            .rowlength = rowlength,
            .stridewidth = stridewidth,
            .nrows = nrows,
            .nprocs = nprocs
    };

    int percentage = 0;
    for(int j = myid; j < nrows; j += nprocs) {
        fill(side.begin(), side.end(), uint_pair{0, 0});
        for(int i = 0; i < nstrides + nrows; i++) {
            if(i - j >= 0 && i - j < nstrides) {
                if(j > 0)
                    MPI_Recv(&srcprev[0], srcprev.size(), MPI_UNSIGNED, (myid - 1 + nprocs) % nprocs, 0, comm, MPI_STATUS_IGNORE);
                else if(i != 0)
                    fill(srcprev.begin(), srcprev.end(), 0);

                long long t0 = ntime();
                data.iops += compute_range(srcprev, side, ans, i - j, j);
                data.dt += ntime() - t0;

                if(j < nrows - 1) {
                    if(myid != nprocs - 1)
                        MPI_Ssend(&srcprev[0], srcprev.size(), MPI_UNSIGNED, (myid + 1) % nprocs, 0, comm);
                    else
                        MPI_Send(&srcprev[0], srcprev.size(), MPI_UNSIGNED, (myid + 1) % nprocs, 0, comm);
                }
            }

            int percentage_step = 1;
            int new_percentage = (i + j / nprocs * (nstrides + nrows) + 1) * 100 / (nstrides + nrows) / ((nrows - 1) / nprocs + 1) / percentage_step * percentage_step;
            if(myid == 0 && new_percentage > percentage) {
                percentage = new_percentage;
                cerr << "Progress: " << percentage << "%" << endl;
                cerr.flush();
            }
        }
    }


    long long buff;
    MPI_Allreduce(&data.iops, &buff, 1, MPI_LONG_LONG, MPI_SUM, comm);
    data.iops = buff;
    MPI_Allreduce(&data.dt, &buff, 1, MPI_LONG_LONG, MPI_SUM, comm);
    data.dt = buff;

    long long ans_size = ans.size();
    MPI_Allreduce(&ans_size, &buff, 1, MPI_LONG_LONG, MPI_MAX, comm);
    ans_size = buff;

    ans.resize(ans_size, {-1, {0,0}});
    vector<ans_type> buf(myid == 0 ? ans_size * nprocs : 1);
    MPI_Gather(&ans[0], ans.size(), mpi_ans_type(), &buf[0], ans.size(), mpi_ans_type(), 0, comm);
    if(myid == 0) {
        vector<uint_pair> ans_sorted(stridewidth * nstrides);
        for(int i = 0; i < buf.size(); i++)
            if(buf[i].first != -1)
                ans_sorted[buf[i].first] = buf[i].second;

        vector<bool> res = restore_answer(ans_sorted);
        res.resize(depth);

        data.tall = ntime() - tbegin;

        data.verbose_output();
        cout << data.table_output();
        cerr << endl;

        return move(res);
    }
    else {
        return vector<bool>();
    }
}

int main(int argc, char** argv) {
    long long reqdepth = (depth - 1) / bitlen + 1;
    long long nstrides = (reqdepth - 1) / stridewidth + 1;
    long long rowlength = (reqdepth - stridewidth / 2 * (nstrides - 1)) / nrows + 1;
    long long depth; long long stridewidth=64; int nrows=0; int myid=-1;
    vector<uint_data> srcprev(stridewidth);
    vector<pair<long long, uint_pair>> ans;
    vector<uint_pair> side(rowlength);
    compute_range(srcprev, side, ans, i - j, j);
}