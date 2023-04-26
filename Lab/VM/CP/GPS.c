//% gcc -o gps gps.c -I/opt/homebrew/Cellar/gpsd/3.25/include -lgps
#include <stdio.h>
#include <stdlib.h>
#include <gps.h>


int main() {
    struct gps_data_t gps_data;
    int result;

    // Connect to the GPS daemon
    if (gps_open("localhost", "2947", &gps_data) == -1) {
        printf("Failed to connect to GPS daemon\n");
        return 1;
    }

    // Enable streaming of GPS data
    if (gps_stream(&gps_data, WATCH_ENABLE, NULL) == -1) {
        printf("Failed to enable streaming\n");
        return 1;
    }

    // Read GPS data
   if ((result = gps_read(&gps_data, NULL, 0)) == -1) {
        printf("Error while reading GPS data: %d\n", result);
        return 1;
    }

    // Check if the GPS has a fix
    if (gps_data.fix.mode >= MODE_2D && gps_data.fix.mode <= MODE_3D) {
        printf("Latitude: %f, Longitude: %f\n", gps_data.fix.latitude, gps_data.fix.longitude);
    } else {
        printf("GPS has no fix\n");
    }

    // Disable streaming of GPS data
    gps_stream(&gps_data, WATCH_DISABLE, NULL);

    // Close the connection to the GPS daemon
    gps_close(&gps_data);

    return 0;
}
