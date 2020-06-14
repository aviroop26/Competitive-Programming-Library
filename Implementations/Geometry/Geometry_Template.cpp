f80 INF = 1e100;
f80 EPS = 1e-7;
const f80 PI = acos(-1);

typedef complex<f80> PT;
#define xx real()
#define yy imag()

// transformation
PT translate(PT a, PT v) { return a + v; }
PT scale(PT c, f80 factor, PT p) { return c + factor * (p - c); } // about c
PT rot(PT c, PT p, f80 ang) { return c + polar(f80(1), ang) * (p - c); }
PT rot(PT p, f80 ang) { return polar(f80(1), ang) * p; };
PT perp(PT p) { return {-p.yy, p.xx}; }
PT linearTrans(PT p, PT q, PT r, PT fp, PT fq) { return fp + (r - p) * (fq - fp) / (q -p); }

// dot, cross, orientation, angles
f80 dot(PT a, PT b) { return (conj(a) * b).xx; }
f80 cross(PT a, PT b) { return (conj(a) * b).yy; }
f80 sq(PT a) { return dot(a, a); }
int sgn(f80 x) { return (x > 0) - (x < 0); }
bool isPerp(PT v, PT w) { return dot(v, w) == 0; }
f80 angle(PT v, PT w) {
    f80 cosTheta = dot(v, w) / abs(v) / abs(w);
    return acos(max(f80(1), min(f80(1), cosTheta)));
}
f80 orient(PT a, PT b, PT c) { return cross(b - a, c - a); }
bool inAngle(PT a, PT b, PT c, PT p) { // checks whether p is between acute angle extended by (b-a) & (c-a)
    assert(orient(a, b, c) != 0);
    if(orient(a, b, c) < 0) swap(b, c);
    return orient(a, b, p) >= 0 && orient(a, c, p) <= 0;
}
bool half(PT p) {
    assert(p.xx != 0 || p.yy != 0);
    // PT v = {5, 1}; // returns true in range [arg(v) - PI, arg(v))
    // return cross(v, p) < 0 || (cross(v, p) == 0 && dot(v, p) < 0);
    return (p.yy > 0 || (p.yy == 0 && p.xx < 0));
}
void polarSort(PT o, vector<PT> &v) {
    sort(all(v), [o](PT v, PT w) {
        return make_tuple(half(v - o), 0) < make_tuple(half(w - o), cross(v - o, w - o));
    });
}
f80 dist2(PT p, PT q)   { return dot(p-q,p-q); }
f80 dist(PT p, PT q)    { return sqrtl(dist2(p, q)); }
PT RotateCW90(PT p)    { return PT(p.yy,-p.xx); }

// Line template

struct line {
    PT v; f80 c;
    // from direction vector and offset c
    line(PT v, f80 c) : v(v), c(c) {}
    // from line eq a*x + b*y = c
    line(f80 a, f80 b, f80 c) : v({b, -a}), c(c) {}
    // from points p, q
    line(PT p, PT q) : v(q - p), c(cross(v, p)) {}
    f80 side(PT p);
    f80 dist(PT p);
    f80 sqDist(PT p);
    line perPThrough(PT p);
    bool cmpProj(PT p, PT q);
    line translate(PT t);
    line shiftLeft(f80 dist);
    PT proj(PT p);
    PT refl(PT p);
};
f80 line :: side(PT p) { return cross(v, p) - c; }
f80 line :: dist(PT p) { return fabs(side(p)) / abs(v); }
f80 line :: sqDist(PT p) { return side(p) * side(p) / (f80)sq(v); }
line line :: perPThrough(PT p) { return {p, p + perp(v)}; }
bool line :: cmpProj(PT p, PT q) { return dot(v, p) < dot(v, q); } // ???
line line :: translate(PT t) { return {v, c + cross(v, t)}; }
line line :: shiftLeft(f80 dist) { return {v, c + dist * abs(v)}; }
bool interLine(line l1, line l2, PT &out) {
    f80 d = cross(l1.v, l2.v);
    if(d == 0) return false;
    out = (l1.c * l2.v - l2.c * l1.v) / d;
    return true;
}
PT line :: proj(PT p) { return p - perp(v) * side(p) / sq(v); }
PT line :: refl(PT p) { return p - perp(v) * f80(2) * side(p) / sq(v); }
line bisector(line l1, line l2, bool interior) {
    assert(cross(l1.v, l2.v) != 0);
    f80 sign = interior ? 1 : -1;
    return {l2.v / fabs(l2.v) + l1.v / fabs(l1.v) * sign, l2.c / fabs(l2.v) + l1.c / fabs(l1.v) * sign};
}
bool LinesParallel(line l1, line l2)
{ return fabs(cross(l1.v, l2.v)) < EPS; }
bool LinesCollinear(PT a, PT b, PT c, PT d) { 
  return LinesParallel(line(a, b), line(c, d)) && fabs(cross(a-b, a-c)) < EPS && fabs(cross(c-d, c-a)) < EPS; 
}
bool RayOnPoint(PT a, PT v, PT b){ // checks whether b in the ray starting from a in direction of v
    b = b - a;
    if(fabs(cross(b, v)) < EPS) { // A and B in same line
        if(b.xx * v.xx >= -EPS && b.yy * v.yy >= -EPS) // in same ray
            return 1;
        return 0;
    }
    return 0;
}

// Segment template
bool inDisk(PT a, PT b, PT p) { return dot(a - p, b - p) <= 0; }
bool onSegment(PT a, PT b, PT p) { return orient(a, b, p) == 0 && inDisk(a, b, p); }
bool properInterSegment(PT a, PT b, PT c, PT d, PT &out) {
    f80 oa = orient(c, d, a), ob = orient(c, d, b), oc = orient(a, b, c), od = orient(a, b, d);
    if(oa * ob < 0 && oc * od < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}
vector<PT> interSegments(PT a, PT b, PT c, PT d) {
    PT out;
    if(properInterSegment(a, b, c, d, out)) return {out};
    vector<PT> s;
    if(onSegment(c, d, a)) s.pb(a);
    if(onSegment(c, d, b)) s.pb(b);
    if(onSegment(a, b, c)) s.pb(c);
    if(onSegment(a, b, d)) s.pb(d);
    sort(all(s), [](PT a, PT b) { return make_pair(a.xx, a.yy) < make_pair(b.xx, b.yy); });
    s.resize(unique(all(s)) - s.begin());
    return s;
}
f80 segPoint(PT a, PT b, PT p) {
    if(a != b) {
        line l(a, b);
        if(l.cmpProj(a, p) && l.cmpProj(p, b)) return l.dist(p);
    }
    return min(abs(p - a), abs(p - b));
}
f80 segSeg(PT a, PT b, PT c, PT d) {
    PT e; // dummy
    if(properInterSegment(a, b, c, d, e)) return 0;
    return min({segPoint(a, b, c), segPoint(a, b, d), segPoint(c, d, a), segPoint(c, d, b)});
}
// project point c onto line segment through a and b
// if projection does not exist, returns the closer endpoint
PT ProjectPointSegment(PT a, PT b, PT c) {
  f80 r = sq(b - a);
  if (fabs(r) < EPS) return a;
  r = dot(c-a, b-a)/r;
  if (r < 0) return a;
  if (r > 1) return b;
  return a + (b - a) * r;
}

// Circle Template
PT circumCenter(PT a, PT b, PT c) {
    b -= a, c -= a;
    assert(cross(b, c) != 0);
    return a + (b * sq(c) - c * sq(b)) / cross(b, c) / f80(2);
}
int circleLine(PT o, f80 r, line l, pair<PT,PT> &out) {
    f80 h2 = r * r - l.sqDist(o);
    if(h2 >= 0) {
        PT p = l.proj(o);
        PT h = l.v * sqrtl(h2) / fabs(l.v);
        out = {p - h, p + h};
    }
    return 1 + sgn(h2);
}
int circleCircle(PT o1, f80 r1, PT o2, f80 r2, pair<PT,PT> &out) {
    PT d = o2 - o1;
    f80 d2 = sq(d);
    if(d2 == 0) { assert(r1 != r2); return 0; }
    f80 pd = (d2 + r1 * r1 - r2 * r2) / f80(2); // |O1P| * d
    f80 h2 = r1 * r1 - pd * pd / d2;
    if(h2 >= 0) {
        PT p = o1 + d * pd / d2, h = perp(d) * sqrtl(h2 / d2);
        out = {p - h, p + h};
    }
    return 1 + sgn(h2);
}
bool ptsToCircle(PT p1, PT p2, f80 r, PT &c)
{
   f80 d = dist(p1, p2);
   f80 det = (r * r) / (d * d) - 0.25;
   if(det < 0.0)
      return false;
   f80 h = sqrtl(det);
   c = {(p1.xx + p2.xx) * 0.5 + (p1.yy - p2.yy) * h, (p1.yy + p2.yy) * 0.5 + (p2.xx - p1.xx) * h};
   //other centre by exchanging p1 and p2
   return true;
}
int tangents(PT o1, f80 r1, PT o2, f80 r2, bool inner, vector<pair<PT,PT>> &out) {
    if(inner) r2 *= -1;
    PT d = o2 - o1;
    f80 dr = r1 - r2, d2 = dot(d, d), h2 = d2 - dr * dr;
    if(d2 == 0 || h2 < 0) { assert(h2 != 0); return 0; }
    for(f80 sign : {-1, 1}) {
        PT v = (d * dr + perp(d) * sqrtl(h2) * sign) / d2;
        out.push_back({o1 + v * r1, o2 + v * r2});
    }
    return 1 + (h2 > 0);
}
f80 area(f80 a, f80 b, f80 c) {
   f80 s = (a + b + c) * 0.5;
   return sqrtl(s * (s - a) * (s - b) * (s - c));
}
f80 radiusCircumcircle(f80 a, f80 b, f80 c) {
   return (a * b * c) / (4 * area(a, b, c)); //R = abc / 4A
}
f80 radiusIncircle(f80 a, f80 b, f80 c) {
   return area(a, b, c) / (0.5 * (a + b + c));//r = A / s
}
f80 radiusIncircle(PT p1, PT p2, PT p3) {
	f80 a = abs(p1 - p2);
	f80 b = abs(p1 - p3);
	f80 c = abs(p2 - p3);
	return area(a, b, c) / (0.5 * (a + b + c));//r = A / s
}
bool incircle(PT p1, PT p2, PT p3, PT &c, f80 &r) {
   r = radiusIncircle(p1, p2, p3);
   if(fabs(r) < EPS)
      return false;
   f80 ratio = dist(p1, p2) / dist(p1, p3);
   PT p = p2 + (p3 - p2) * (ratio / (1 + ratio));
   line l1(p1, p);
   ratio = dist(p2, p1) / dist(p2, p3);
   p = p1 + (p3 - p1) * (ratio / (1 + ratio));
   line l2(p2, p);
   return interLine(l1, l2, c);
}

// Polygon template

// determine if point is in a possibly non-convex polygon (by William
// Randolph Franklin); returns 1 for strictly interior points, 0 for
// strictly exterior points, and 0 or 1 for the remaining points.
// Note that it is possible to convert this into an *exact* test using
// integer arithmetic by taking care of the division appropriately
// (making sure to deal with signs properly) and then by writing exact
// tests for checking point on polygon boundary
bool PointInPolygon(const vector<PT> &p, PT q) {
  bool c = 0;
  fr(i, 0, sz(p) - 1){
    int j = (i + 1) % sz(p);
    if ((p[i].yy <= q.yy && q.yy < p[j].yy || 
      p[j].yy <= q.yy && q.yy < p[i].yy) &&
      q.xx < p[i].xx + (p[j].xx - p[i].xx) * (q.yy - p[i].yy) / (p[j].yy - p[i].yy))
      c = !c;
  }
  return c;
}
// determine if point is on the boundary of a polygon
bool PointOnPolygon(const vector<PT> &p, PT q) {
  fr(i, 0, sz(p) - 1)
    if(dist2(ProjectPointSegment(p[i], p[(i+1)%sz(p)], q), q) < EPS)
      return 1;
    return 0;
}
// This code computes the area or centroid of a (possibly nonconvex)
// polygon, assuming that the coordinates are listed in a clockwise or
// counterclockwise fashion.  Note that the centroid is often known as
// the "center of gravity" or "center of mass".
f80 ComputeSignedArea(const vector<PT> &p) {
  f80 area = 0;
  fr(i, 0, sz(p) - 1){
    int j = (i+1) % sz(p);
    area += cross(p[i], p[j]);
  }
  return area / 2.0;
}
f80 ComputeArea(const vector<PT> &p) {
  return fabs(ComputeSignedArea(p));
}
PT ComputeCentroid(const vector<PT> &p) {
  PT c(0,0);
  f80 scale = 6.0 * ComputeSignedArea(p);
  fr(i, 0, sz(p) - 1){
    int j = (i + 1) % sz(p);
    c = c + (p[i] + p[j]) * cross(p[i], p[j]);
  }
  return c / scale;
}
// tests whether or not a given polygon (in CW or CCW order) is simple
bool IsSimple(const vector<PT> &p) {
  fr(i, 0, p.size() - 1){
    fr(k, i + 1, p.size() - 1){
      int j = (i+1) % p.size();
      int l = (k+1) % p.size();
      if (i == l || j == k) continue;
      if (segSeg(p[i], p[j], p[k], p[l]) < EPS) 
        return 0;
    }
  }
  return 1;
}
int sign(PT a, PT b, PT c){ return (cross(b - a, c - b) < 0 ? -1 : 1 ); }

vector<PT> CLIntersection(PT a, PT b, PT c, f80 r) {
  vector<PT> ret;
  b = b-a;
  a = a-c;
  f80 A = dot(b, b);
  f80 B = dot(a, b);
  f80 C = dot(a, a) - r*r;
  f80 D = B*B - A*C;
  if (D < -EPS) return ret;
  ret.pb(c+a+b*(-B+sqrtl(D+EPS))/A);
  if (D > EPS)
    ret.pb(c+a+b*(-B-sqrtl(D))/A);
  return ret;
}
f80 ArcArea(PT a, PT b, PT c, f80 r){
    f80 cosa = dot(a - c, b - c) / (abs(a - c) * abs(b - c));
    f80 ang = acos(cosa);
    if(ang > 2 * PI) ang = 2 * PI - ang;
    return 0.5 * r * r * ang;
}
f80 TCArea(PT a1, PT a2, PT c, f80 r){
    f80 ans = 0;
    f80 d1 = dist2(a1, c), d2 = dist2(a2, c);
    if(d1 > d2) swap(d1, d2), swap(a1, a2);
    if(d2 <= r * r) {
        ans = 0.5 * fabs(cross(a1 - c, a2 - c));
    }
    else if(d1 <= r * r){
        auto vv = CLIntersection(a1, a2, c, r);
        PT C = vv[0];
        if(vv.size() > 1 && dot(vv[1] - a1, vv[1] - a2) < 0)
            C = vv[1];
        vv = CLIntersection(a2, c, c, r);
        PT D = vv[0];
        if(vv.size() > 1 && dot(vv[1] - a2, vv[1] - c) < 0)
            D = vv[1];
        ans = ArcArea(C, D, c, r) + 0.5 * fabs(cross(C - c, a1 - c));
    }
    else{
        auto vv = CLIntersection(a1, a2, c, r);
        if(vv.size() <= 1 || dot(a1 - vv[0], a2 - vv[0]) > 0){
            ans = ArcArea(a1, a2, c, r);
        }
        else{
            if(dist2(a1, vv[1]) < dist2(a1, vv[0]))
                swap(vv[0], vv[1]);
            ans = ArcArea(a1, vv[0], c, r) + ArcArea(a2, vv[1], c, r) + 0.5 * fabs(cross(vv[0] - c, vv[1] - c));
        }
    }
    return fabs(ans);
}
f80 PolyCircArea(vector<PT> &a, PT c, f80 r){ // Calculates the intersection area of polygon and circle
    int n = a.size();
    if(n <= 2) return 0;
    f80 ans = 0;
    fr(i, 0, n - 1)
        ans += TCArea(a[i], a[(i + 1) % n], c, r) * sign(c, a[i], a[(i + 1) % n]);
    return fabs(ans);
}
/*
incentre is intersection of angle bisectors
circumcentre is meeting point of perpendicular bisectors

Cosine Law :- c^2 = a^2 + b^2 - 2 * a * b * cosC

Sine Law :- a / sinA = b / sinB = c / sinC = 2R(circumradius)
*/
//QUADRILATERALS
/*
Area of Trapezium : 0.5 * (w1 + w2) * h
w1 and w2 are parallel edges and h is height

Area of kite : 0.5 * d1 * d2
*/
