#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

const double G = 6.67408e-11;

template <class T>
struct Vector3 {
    T x, y, z;

    Vector3 ();
    Vector3 (T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3<T> operator+(const Vector3<T>& v2) {
        Vector3 vec (this->x + v2.x, this->y + v2.y, this->z + v2.z);
        return vec;
    }

    void operator+=(const Vector3<T>& v2) {
        this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
    }

    Vector3<T> operator-(const Vector3<T>& v2) {
        Vector3 vec (this->x - v2.x, this->y - v2.y, this->z - v2.z);
        return vec;
    }

    Vector3<T> operator-() {
        Vector3 vec (-this->x, -this->y, -this->z);
        return vec;
    }

    T mag() {
        return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
    }

    Vector3 scaled(T s) {
        Vector3 vec (s * this->x, s * this->y, s * this->z);
        return vec;
    }

    Vector3 norm() {
        return this->scaled(1/this->mag());
    }

    string format() {
        ostringstream out;
        out << "{" << this->x << ", " << this->y << ", " << this->z << "}";
        return out.str();
    }
};

struct Body {
    Vector3<double> pos, vel, acc;
    double mass;

    Body (double mass, Vector3<double> pos, Vector3<double> vel, Vector3<double> acc) : mass(mass), pos(pos), vel(vel), acc(acc) {}

    void print() {
        cout << "pos: " << pos.format() << "vel: " << vel.format() << "acc: " << acc.format();
    }
};

class BodyManager {
    vector<Body *> bodies;

    public:
        void addBody(Body* body) {
            bodies.push_back(body);
        }

        // time delta
        void update(double t) {
            vector<Vector3<double>> accels;
            accels.reserve(bodies.size());
            for (auto body = bodies.begin(); body != bodies.end(); ++body) {
                Vector3<double> accel (0, 0, 0);
                for (auto source = bodies.begin(); source != bodies.end(); ++source) {
                    if (body == source) continue;
                    Vector3<double> r = (*body)->pos - (*source)->pos;
                    double m = (*source)->mass;
                    double r2 = r.mag() * r.mag();
                    accel += r.scaled(G*m/r2);
                }
                accels[distance(bodies.begin(), body)] = accel;
            }

            for (int i = 0; i < bodies.size(); i++) {
                Body* b = bodies.at(i);
                Vector3<double> accel = accels[i];
                b->pos = b->pos + b->vel.scaled(t) + b->acc.scaled(.5*t*t);
                b->vel = b->acc.scaled(t);
                b->acc = accel;
            }
        }

        void print() {
            for (auto body = bodies.begin(); body != bodies.end(); ++body) {
                (*body)->print();
                cout << endl;
            }
        }

        ~BodyManager() {
            for (auto it = bodies.begin(); it != bodies.end(); ++it) {
                delete (*it);
            }
        }
};

int main() {
    cout << "Hello World" << endl;
    BodyManager bm;
    bm.addBody(new Body(10, Vector3<double>(0, 0, 0), Vector3<double>(0, 0, 0), Vector3<double>(0, 0, 0)));
    bm.addBody(new Body(20, Vector3<double>(100, 0, 0), Vector3<double>(0, 0, 0), Vector3<double>(0, 0, 0)));

    long increment = 1;

    timespec ts = {0, increment * 1000000};

    while (true) {
        bm.update(increment);
        bm.print();
        nanosleep(&ts, NULL);
    }

}
