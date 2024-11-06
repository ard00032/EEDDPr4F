#ifndef UTM_H
#define UTM_H

class UTM {
private:
    float lat;
    float lon;

public:
    UTM(float lat = 0, float lon = 0): lat(lat), lon(lon) {}

    float getLat() { return lat; }
    float getLon() { return lon; }
    void setLat(float latitud) { lat = latitud; }
    void setLon(float longitud) { lon = longitud; }
};



#endif //UTM_H
