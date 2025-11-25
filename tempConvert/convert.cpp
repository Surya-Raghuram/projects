#include <bits/stdc++.h>
#include "httplib.h"
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    httplib::Server ser;

    ser.Get("/convert", [](const httplib::Request &req, httplib::Response &res) {
        double v= stod(req.get_param_value("value"));
        char t = req.get_param_value("type")[0];

         if (t == 'C') {
            double f = (v * 9.0 / 5.0) + 32;
            res.set_content(to_string(f) + " F", "text/plain");
        } else {
            double c = (v - 32) * 5.0 / 9.0;
            res.set_content(to_string(c) + " C", "text/plain");
        }
    });



    ser.listen("127.0.0.1", 5000);
}